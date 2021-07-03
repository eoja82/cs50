import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # add up value of stocks in portfolio
    portfolioValue = 0

    # update portfolio
    portfolio = db.execute("SELECT * FROM portfolio WHERE id = :id", id=session["user_id"])
    for row in portfolio:
        quote = lookup(row["symbol"])
        # update price
        updatePrice = db.execute("UPDATE portfolio SET price = :price WHERE id = :id AND symbol = :symbol", \
                            price=usd(quote["price"]), id=session["user_id"], symbol=quote["symbol"])
        if updatePrice == 0:
            return apology("Could not update price.")
        # updade total with current stock price
        shares = db.execute("SELECT shares FROM portfolio WHERE id = :id AND symbol = :symbol", \
                            id=session["user_id"], symbol=quote["symbol"])
        updateTotal = db.execute("UPDATE portfolio SET total = :total WHERE id = :id AND symbol = :symbol", \
                                total=usd(shares[0]["shares"] * quote["price"]), id=session["user_id"], symbol=quote["symbol"])
        if updateTotal == 0:
            return apology("Could not update total.")

        portfolioValue += shares[0]["shares"] * quote["price"]
    
    # get amount of cash
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"]) 

    # get updated portfolio
    updated = db.execute("SELECT * FROM portfolio WHERE  id = :id", id = session["user_id"])

    return render_template("index.html", portfolio=updated, cash=usd(cash[0]["cash"]), \
                            total=usd(portfolioValue + cash[0]["cash"]))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        quote = lookup(request.form.get("symbol"))
        if not quote:
            return apology("Invalid Symbol")
        quantity = int(request.form.get("quantity"))
        if quantity <= 0:
            return apology("Quantity must be greater than 0")
        
        # check user's account to see if enough cash to purchase stock
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        if not cash or float(cash[0]["cash"]) < quote["price"] * quantity:
            return apology("Not enough cash!")

        # update user's cash    
        updateCash = db.execute("UPDATE users SET cash = cash - :purchase WHERE id = :id", purchase=quote["price"] * float(quantity), id=session["user_id"])
        if updateCash == 0:
            return apology("cash error, could not complete purchase")

        # update transaction history
        history = db.execute("INSERT INTO history (symbol, shares, price, id) VALUES (:symbol, :shares, :price, :id)", \
                            symbol=quote["symbol"], shares=quantity, price=usd(quote["price"]), id=session["user_id"])
        if not history:
            return apology("history error, could not complete purchase")

        # update portfolio
        shares = db.execute("SELECT shares FROM portfolio WHERE id = :id and symbol = :symbol", \
                            id=session["user_id"], symbol=quote["symbol"])
        if not shares:
            db.execute("INSERT INTO portfolio (symbol, company, shares, price, total, id) \
                        VALUES (:symbol, :company, :shares, :price, :total, :id)", \
                        symbol=quote["symbol"], company=quote["name"], shares=quantity, price=usd(quote["price"]), total=usd(quote["price"] * quantity), id=session["user_id"])
        else:
            db.execute("UPDATE portfolio SET shares = shares + :shares WHERE id = :id AND \
                        symbol = :symbol", shares=quantity, id=session["user_id"], symbol=quote["symbol"])

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM history WHERE id = :id ORDER BY transacted DESC", \
                        id=session["user_id"])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        quote = lookup(request.form.get("symbol"))     
        if not quote:
            return apology("Invalid Symbol")
        return render_template("quote.html", quote=quote)
    else:
        return render_template("quote.html", quote={"name": "Enter Symbol Above", "price": 0, "symbol": ""})


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        name = request.form.get("name")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if password != confirmation:
            return apology("Passwords did not match.")
        else:
            hash = generate_password_hash(password)
            result = 0
            
            # catch error when someone tries to register with a username already in use
            try:
                result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=name, hash=hash)
            except:
                return apology("Username already exists.")
                
            # remember logged on user
            session["user_id"] = result

            return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        quantity = int(request.form.get("quantity"))
        if quantity <= 0:
            return apology("Quantity must be greater than 0")

        company = request.form.get("symbol")
        quote = lookup(company)
        if not quote:
            return apology("Invalid Symbol")
        
        # update portfolio
        shares = db.execute("SELECT shares FROM portfolio WHERE id = :id and symbol = :symbol", \
                            id=session["user_id"], symbol=quote["symbol"])
        if not shares or shares[0]["shares"] < quantity:
            return apology("Not enough shares to sell")
        else:
            db.execute("UPDATE portfolio SET shares = shares - :shares \
                       WHERE id = :id AND symbol = :symbol", \
                       shares=quantity, id=session["user_id"], symbol=company)       
        
        # update user's cash    
        updateCash = db.execute("UPDATE users SET cash = cash + :sale WHERE id = :id", \
                                sale=quote["price"] * float(quantity), id=session["user_id"])
        if updateCash == 0:
            return apology("cash error")

        # update transaction history
        history = db.execute("INSERT INTO history (symbol, shares, price, id) VALUES (:symbol, :shares, :price, :id)", \
                            symbol=quote["symbol"], shares="-" + str(quantity), price=usd(quote["price"]), id=session["user_id"])
        if not history:
            return apology("history error")

        # delete stock from portfolio if no shares left
        currShares = db.execute("SELECT shares FROM portfolio WHERE id = :id \
                                AND symbol = :symbol", \
                                id=session["user_id"], symbol=company)
        if currShares[0]["shares"] == 0:
            db.execute("DELETE FROM portfolio WHERE id = :id AND symbol = :symbol", \
                        id=session["user_id"], symbol=company)
        
        return redirect("/")
    else:
        symbols = db.execute("SELECT symbol FROM portfolio WHERE id = :id", \
                            id=session["user_id"])
        return render_template("sell.html", symbols=symbols)

# allow user to change password
@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    if request.method == "POST":
        # check if old password is correct
        oldPassword = request.form.get("oldPassword")
        oldHash = db.execute("SELECT hash FROM users WHERE id = :id", id=session["user_id"])
        validateOld = check_password_hash(oldHash[0]["hash"], oldPassword)
        if validateOld == False:
            return apology("Old password not correct.")
        
        # check that new password and confirmation match
        newPassword = request.form.get("newPassword")
        confirmation = request.form.get("confirmation")
        if newPassword != confirmation:
            return apology("New passwords did not match.")
        
        # update user's hash/password
        hash = generate_password_hash(newPassword)
        user = db.execute("UPDATE users SET hash = :hash WHERE id = :id", \
                          hash=hash, id=session["user_id"])
        if user == 1:
            return render_template("password.html", message="Password changed.")
        else:
            return render_template("password.html", message="Could not change password.")
    else:
        return render_template("password.html", message="")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
