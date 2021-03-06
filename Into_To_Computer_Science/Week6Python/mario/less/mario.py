""" print a hash pyramid like the end of Mario Bros with height and width between 1 and 8 based on user input """
""" cs50 library required for this file """
from cs50 import get_int

height = 0
while height < 1 or height > 8:
    height = get_int("Height: ")

h = height
w = 1

while w < height + 1:
    print(" " * (h - 1), end="")
    print("#" * w)
    h -= 1
    w += 1

