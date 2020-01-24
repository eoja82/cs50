#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool check_argv_1(string num);
string encrypt(string str, long key);

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        //make sure only digits entered
        if (check_argv_1(argv[1]))
        {
            long key;
            char *ptr;
            key = strtol(argv[1], &ptr, 10);
            string message = get_string("plaintext: ");
            string encrypted = encrypt(message, key);
            printf("ciphertext: %s\n", encrypted);
        }
        else
        {
            printf("Usage: ./caeser key\n");
            return 1;
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
        if (num[i] >= 48 && num[i] <= 57)
        {
            continue;
        }
        else
        {
            result = false;
            break;
        }
    }
    return result;
}
string encrypt(string str, long key)
{
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        //convert uppercase letter
        if (str[i] >= 65 && str[i] <= 90)
        {
            str[i] = (str[i] + key - 65) % 26 + 65;
        }
        //convert lowercase letters
        else if (str[i] >= 97 && str[i] <= 122)
        {
            str[i] = (str[i] + key - 97) % 26 + 97;
        }
        //don't change other characters
        else
        {
            continue;
        }
    }
    return str;
}