#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

string to_lower(string str);
bool check_argv_1(string str);
int checkif_duplicates(string str, char x);
int alpha_index(char x);

string alphabet = "abcdefghijklmnopqrstuvwxyz";

int main(int argc, string argv[])
{
    if (argc == 1)
    {
        printf("You must enter one key 26 letter characters in lenght with no spaces\n");
        return 1;
    }
    string key = to_lower(argv[1]);
    int len = strlen(argv[1]);

    if (argc == 2 && check_argv_1(key))
    {
        string text = get_string("plaintext: ");
        int textlen = strlen(text);
        
        //convert key into array of ascii numbers
        int key_arr[len];
        for (int i = 0; i < len; i++)
        {
            key_arr[i] = key[i];
        }
        
        //map text to key
        for (int j = 0; j < textlen; j++)
        {
            //check if it is a letter
            if (isupper(text[j]) || islower(text[j]))
            {
                int index = alpha_index(text[j]);
                if (isupper(text[j]))
                {
                    text[j] = key[index] - 32;
                }
                else
                {
                    text[j] = key[index];
                }
            }
            //if not a letter leave as is
            else
            {
                continue;
            }
        }
        printf("ciphertext: %s\n", text);
        return 0;
    }
    else
    {
        printf("Key must only be 26 letter characters in lenght with no spaces\n");
        return 1;
    }

}
string to_lower(string str)
{
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (isupper(str[i]))
        {
            str[i] = tolower(str[i]);
        }
        else
        {
            continue;
        }
    }
    return str;
}
bool check_argv_1(string str)
{
    bool result = true;
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        int count = checkif_duplicates(str, str[i]);
        if (strlen(str) == 26 && islower(str[i]) && count <= 1)
        {
            continue;
        }
        else
        {
            result = false;
            printf("break\n");
            break;
        }
    }
    return result;
}
int checkif_duplicates(string str, char x)
{
    int count = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (str[i] == x)
        {
            count++;
        }
    }
    return count;
}
int alpha_index(char x)
{
    int index = 0;
    int len = strlen(alphabet);
    if (isupper(x))
    {
        x = (int) x + 32;
    }
    for (int i = 0; i < len; i++)
    {
        if (x == alphabet[i])
        {
            index = i;
            break;
        }
    }
    return index;
}