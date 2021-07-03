from cs50 import get_float
import math


def main():
    changeOwed = -1
    coinCount = 0

    while changeOwed <= 0:
        change = get_float("Change owed: ")
        changeOwed = round(change, 2) * 100

    if changeOwed >= 25:
        coins = getCoins(changeOwed, 25)
        coinCount += coins
        changeOwed = changeOwed - (coins * 25)
    if changeOwed >= 10:
        coins = getCoins(changeOwed, 10)
        coinCount += coins
        changeOwed = changeOwed - (coins * 10)
    if changeOwed >= 5:
        coins = getCoins(changeOwed, 5)
        coinCount += coins
        changeOwed = changeOwed - (coins * 5)
    if changeOwed >= 1:
        coins = getCoins(changeOwed, 1)
        coinCount += coins
        changeOwed = changeOwed - (coins * 1)
    print(coinCount)


def getCoins(change, coin):
    coins = math.floor(change / coin)
    return coins


main()