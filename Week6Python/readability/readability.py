from cs50 import get_string
import math

""" use Coleman-Liau formula to test grade level of input text """


def main():
    text = get_string("Text: ")
    letters = countLetters(text)
    words = countWords(text)
    sentences = countSentences(text)

    l = (letters / words) * 100
    s = (sentences / words) * 100
    index = (0.0588 * l) - (0.296 * s) - 15.8
    grade = math.ceil(index) if index - math.floor(index) > 0.5 else math.floor(index)
    if grade >= 16:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print("Grade: ", grade)


def countLetters(text):
    letters = 0
    i = 0
    while i < len(text):
        if ord(text[i]) >= 65 and ord(text[i]) <= 90 or ord(text[i]) >= 97 and ord(text[i]) <= 122:
            letters += 1
        i += 1
    return letters


def countWords(text):
    words = 0
    i = 0
    while i < len(text):
        if ord(text[i]) in [32, 0]:
            words += 1
        elif i == len(text) - 1:
            words += 1
        i += 1
    return words


def countSentences(text):
    sentences = 0
    i = 0
    while i < len(text):
        if ord(text[i]) in[46, 33, 63]:
            sentences += 1
        i += 1
    return sentences


main()