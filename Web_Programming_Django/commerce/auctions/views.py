from typing import List
from django.contrib.auth import authenticate, login, logout
from django.db import IntegrityError
from django.db.utils import Error, OperationalError
from django.http import HttpResponse, HttpResponseBadRequest, HttpResponseRedirect
from django.shortcuts import render
from django.urls import reverse

from .models import Bid, Comment, Listings, User, Watch

def get_listing(listing_id):
    try:
        return Listings.objects.get(pk=listing_id)
    except:
        return None

def get_bids(listing):
    try:
        return Bid.objects.filter(listing=listing)
    except:
        return 0

def get_watching(user):
    try:
        return Watch.objects.get(user=user)
    except Watch.DoesNotExist:
        return None

def index(request):
    return render(request, "auctions/index.html", {
        "listings": Listings.objects.all().order_by("-date_created")
    })


def login_view(request):
    if request.method == "POST":

        # Attempt to sign user in
        username = request.POST["username"]
        password = request.POST["password"]
        user = authenticate(request, username=username, password=password)

        # Check if authentication successful
        if user is not None:
            login(request, user)
            return HttpResponseRedirect(reverse("index"))
        else:
            return render(request, "auctions/login.html", {
                "message": "Invalid username and/or password."
            })
    else:
        return render(request, "auctions/login.html")


def logout_view(request):
    logout(request)
    return HttpResponseRedirect(reverse("index"))


def register(request):
    if request.method == "POST":
        username = request.POST["username"]
        email = request.POST["email"]

        # Ensure password matches confirmation
        password = request.POST["password"]
        confirmation = request.POST["confirmation"]
        if password != confirmation:
            return render(request, "auctions/register.html", {
                "message": "Passwords must match."
            })

        # Attempt to create new user
        try:
            user = User.objects.create_user(username, email, password)
            user.save()
        except IntegrityError:
            return render(request, "auctions/register.html", {
                "message": "Username already taken."
            })
        login(request, user)
        return HttpResponseRedirect(reverse("index"))
    else:
        return render(request, "auctions/register.html")


def new_auction(request):
    if request.method == "POST":
        user = request.user
        title = request.POST["title"]
        description = request.POST["description"]
        photoURL = request.POST["photoURL"]
        category = request.POST["category"]
        starting_bid = request.POST["starting_bid"]
        
        listing = Listings(user=user, title=title, description=description, photoURL=photoURL, category=category, starting_bid=starting_bid)
        listing.save()

        return HttpResponseRedirect(reverse("listing_view", args=(listing.id,)))
    else:
        categories = []
        for x in Listings.CATEGORY_CHOICES:
            categories.append(x[1])
        return render(request, "auctions/add_listing.html", {
            "categories": categories
        })


def listing_view(request, listing_id):
    if request.method == "POST":
        # place bid
        user = request.user
        bid = float(request.POST["bid"])
        listing_id = request.POST["listing_id"]

        listing = get_listing(listing_id)

        try:
            bids = listing.bids.all()
        except Listings.DoesNotExist:
            pass

        if not bids:
            if bid >= listing.starting_bid:
                # save bid
                newBid = Bid(listing=listing, user=user, bids=bid)
                newBid.save()

                # save the new high bid
                listing.highest_bid = bid
                listing.save()

                bids = listing.bids.all()
                high_bid = bids.latest("bids")
                bid_count = bids.count()
                high_bidder = bids.latest("bids").user

                return render(request, "auctions/listing_view.html", {
                    "listing": listing, "high_bid": high_bid, "high_bidder": high_bidder, "bid_count": bid_count
                })
            else:
                return render(request, "auctions/listing_view.html", {
                    "listing": listing, "message": f"Bid must be at least ${listing.starting_bid}."
                })
        else:
            high_bid = bids.latest("bids")
            bid_count = bids.count()
            high_bidder = bids.latest("bids").user

            if bid > high_bid.bids:
                # save bid
                newBid = Bid(listing=listing, user=user, bids=bid)
                newBid.save()

                # save new high bid
                listing.highest_bid = bid
                listing.save()

                bids = listing.bids.all()
                high_bid = bids.latest("bids")
                bid_count = bids.count()
                high_bidder = bids.latest("bids").user

                return render(request, "auctions/listing_view.html", {
                    "listing": listing, "high_bid": high_bid, "high_bidder": high_bidder, "bid_count": bid_count
                })
            else:
                return render(request, "auctions/listing_view.html", {
                    "listing": listing, "high_bidder": high_bidder, "high_bid": high_bid, "bid_count": bid_count, "message": f"Your bid must be greater than ${high_bid.bids}"
                })
    else:
        listing = get_listing(listing_id)
        if not listing:
            return HttpResponseRedirect(reverse("index"))
    
        high_bid = None
        bid_count = None
        high_bidder = None
        watching = None

        try:
            bids = listing.bids.all()
        except Listings.DoesNotExist:
            pass

        watchlist = get_watching(request.user)
        if watching and listing in watchlist:
            watching = True

        if bids:
            high_bid = bids.latest("bids")
            bid_count = bids.count()
            high_bidder = bids.latest("bids").user
        
        return render(request, "auctions/listing_view.html", {
            "listing": listing, "bid_count": bid_count, "high_bid": high_bid, "high_bidder": high_bidder, "watching": watching
        })


def watch(request, listing_id):
    if request.method == "POST":
        if request.POST["watch"] == "watch":
            listing = get_listing(listing_id)
            if not listing:
                return HttpResponseBadRequest("Bad Request: listing does not exist.")
            watching = get_watching(request.user)
            print("watching = ", watching, "listing = ", listing)
            if not watching:
                # save user to watch and add
                print("creating and saving to watch list")
                watch = Watch(user=request.user)
                watch.save()
                watch.listings.add(listing)
                print("watch = ", watch)
            else:
                # add to watch
                print("saving to watch list")
                watching.listings.add(listing)
                print("watching.listings = ", watching.listings)
            return HttpResponseRedirect(reverse("listing_view", args=(listing.id,)))
        else:
            # unwatch, temporary return
            return HttpResponseRedirect(reverse("index"))
    else:
        return HttpResponseRedirect(reverse("index"))


def categories(request):
    categories = []
    for x in Listings.CATEGORY_CHOICES:
            categories.append(x[0])
    return render(request, "auctions/categories.html", {
        "categories": categories
    })


def category_view(request, category):
    return render(request, "auctions/category.html")