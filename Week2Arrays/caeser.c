#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool check_argv_1(string num);

int main(int argc, string argv[])
{
    printf("%lu\n", strlen(argv[1]));
    printf("argc: %i\n", argc);
    if (argc == 2)
    {
        if (check_argv_1(argv[1]))
        {
            printf("good\n");
        }
        else
        {
            printf("Usage: ./caeser key\n");
        }
    }
    else
    {
        printf("Usage: ./caeser key\n");
        return 1;
    }
}

bool check_argv_1(string num)
{
    bool result = true;
    for (int i = 0, n = strlen(num); i < n; i++)
    {
        if ((int) num[i] >= 48 && (int) num[i <= 57])
        {
            printf("break\n");
            result = false;
            break;
        }
        else
        {
             printf("num[i] %c\n", num[i]);
        }
    }
    return result;
}