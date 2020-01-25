//substitution cipher, user enters key of 26 letters, then message, output is encrypted per key

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool check_argv_1(string str);
string to_lower(string str);
int alpha_index(char x);

string alphabet = "abcdefghijklmnopqrstuvwxyz";

int main(int argc, string argv[])
{
    if (argc == 2 && check_argv_1(argv[1])) 
    {
        //printf("good\n");
        string text = get_string("plaintext: ");
        int len = strlen(argv[1]);
        string key = to_lower(argv[1]);
        //convert key into array of ascii numbers
        int key_arr[strlen(key)];
        for (int i = 0; i < strlen(key); i++)
        {
            key_arr[i] = key[i];
            //printf("key: %i\n", key_arr[i]);
        }
        for (int j = 0; j < len; j++)
        {   
            //check if it is a letter
            if (isupper(text[j]) || islower(text[j]))
            {
                if (isupper(text[j]))
                {
                    int index = alpha_index(text[j]);
                    //printf("UC text[j] %c, key[index]: %i\n", text[j], key[index]);
                    //printf("index: %i, key[index] - 32: %i\n", index, key[index] - 32);
                    text[j] = key[index] - 32;
                }
                else
                {
                    int index = alpha_index(text[j]);
                    //printf("else text[j] %c, key[index]: %i\n", text[j], key[index]);
                    text[j] = (char) key[index];
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

//make sure only letters entered
bool check_argv_1(string str)
{
    bool result = true;
    int len = strlen(str);
    int arr[len];
    for (int i = 0; i < len; i++)
    {
        if ((isupper(str[i]) || islower(str[i])) && strlen(str) == 26)
        {
            arr[i] = 
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