#include <cs50.h>
#include <stdio.h>
#include <math.h>

int getCoins(int change, int coin);

//when user enters positive amount, returned will be the number of coins need to make change for that amount
int main(void) 
{
    int changeOwed = -1;
    int coinCount = 0;
    while (changeOwed <= 0) 
    {
        float change = get_float("Change owed: \n");
        changeOwed = round(change * 100);
    }
    if (changeOwed >= 25)
    {
        int coins = getCoins(changeOwed, 25);
        coinCount += coins;
        changeOwed = changeOwed - (coins * 25);
    }
    if (changeOwed >= 10)
    {
        int coins = getCoins(changeOwed, 10);
        coinCount += coins;
        changeOwed = changeOwed - (coins * 10);
    }
    if (changeOwed >= 5)
    {
        int coins = getCoins(changeOwed, 5);
        coinCount += coins;
        changeOwed = changeOwed - (coins * 5);
    }
    if (changeOwed >= 1)
    {
        int coins = getCoins(changeOwed, 1);
        coinCount += coins;
        changeOwed = changeOwed - (coins * 1);
    }
    printf("%i\n", coinCount);
}

int getCoins(int change, int coin)
{
    int coins = floor(change / coin);
    return coins;
}