from cs50 import get_string


""" get card number """
cardNum = get_string("Number: ")
length = len(cardNum)

""" make card number an array of ints, reversed """
cardArr = []
for i in range(length):
    cardArr.append(int(cardNum[i]))
cardArr.reverse()

""" accumulator sum of every other digit not multied by 2 """
added = 0
""" add every other even index """
i = 0
while i < length:
    added += cardArr[i]
    i += 2

""" accumulator for every other digit multipiled by 2 starting with last number """
addMult = 0
""" multiply every other odd index by 2, if result is more than one digit then add digits """
j = 1
while j < length:
    mult = cardArr[j] * 2
    strMlt = str(mult)
    tempTotal = 0
    for k in range(len(strMlt)):
        tempTotal += int(strMlt[k])
    addMult += tempTotal
    j += 2

shouldBeZero = (added + addMult) % 10

if cardNum[0] == "3":
    print("first ", cardNum[0])
if length == 15:
    print("lenght matches")

if shouldBeZero != 0:
    print("INVALID")
elif cardNum[0] == "3" and length == 15:
    print("AMEX")
elif cardNum[0] == "5" and length == 16:
    if cardNum[1] == "1" or cardNum[1] == "2" or cardNum[1] == "3" or cardNum[1] == "4" or cardNum[1] == "5":
        print("MASTERCARD")
elif cardNum[0] == "4" and length == 13 or length == 16:
    print("VISA")
else:
    print("INVALID")