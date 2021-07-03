#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

int main(void) 
{
    long long cardNum = get_long("Card Number: \n");
    long long tempNum = cardNum;
    //get lenght of card number
    int numLen = 0;
    while (tempNum > 0) 
    {
        tempNum /= 10;
        numLen++;
    }
    //make card number an array, reversed
    long long tempNum2 = cardNum;
    int arr[numLen];
    for (int i = 0; i < numLen; i++)
    {
        int last = tempNum2 % 10;
        arr[i] = last;
        tempNum2 /= 10;
    }
    //accumulator sum of every other digit not multied by 2
    int added = 0;
    //accumulator for every other digit multipiled by 2 starting with last number
    int addMult = 0;
    //add every other even index
    for (int i = 0; i < numLen; i += 2)
    {
        added += arr[i];
    }
    //multiply every other odd index by 2, if result is more than one digit then add digits 
    for (int i = 1; i < numLen; i += 2)
    {
        int multLen = 0;
        int mult = arr[i] * 2;
        int tempMult = mult;
        while (tempMult > 0)
        {
            tempMult /= 10;
            multLen++;
        }
        if (multLen > 0) 
        {
            int tempArr[multLen];
            for (int j = 0; j < multLen; j++)
            {
                int last = mult % 10;
                tempArr[j] = last;
                mult /= 10;
            }
            for (int k = 0; k < multLen; k++)
            {
                addMult += tempArr[k];
            }
        }
        else
        {
            addMult += mult;
        }
    }
    int shouldBeZero = (added + addMult) % 10;
    if (shouldBeZero != 0) 
    {
        printf("INVALID\n");
    }
    //check if AmEx
    else if (arr[numLen - 1] == 3 && numLen == 15)
    {
        if (arr[numLen - 2] == 4 || arr[numLen - 2] == 7)
        {
            printf("AMERICAN EXPRESS\n");
        }
    }
    //check if mastercard
    else if (arr[numLen - 1] == 5 && numLen == 16)
    {
        if (arr[numLen - 2] == 1 || arr[numLen - 2] == 2 || arr[numLen - 2] == 3 || arr[numLen - 2] == 4 || arr[numLen - 2] == 5)
        {
            printf("MASTERCARD\n");
        }
    }
    //check if visa
    else if ((arr[numLen - 1] == 4 && numLen == 13) || (arr[numLen - 1] == 4 && numLen == 16))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}