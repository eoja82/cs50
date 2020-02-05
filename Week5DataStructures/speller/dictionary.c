// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 17576;
unsigned int word_count = 0;

// Hash table
node *table[N];

void initialise(node *hashtable[])
{
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //convert word to lowercase becase hash uses ascii numbers
    int length = strlen(word);
    char to_lowercase[length + 1];
    for (int i = 0; i < length; i++)
    {
        to_lowercase[i] = tolower(word[i]);
    }
    to_lowercase[length] = '\0';

    unsigned int index = hash(to_lowercase);
    node *current = table[index];
    while (current != NULL)
    {
        if (strcasecmp(current->word, word) == 0)
        {
            return true;
        }
        else
        {
            current = current->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash = 0;
    for (int i = 0; i < strlen(word) && i < 3; i++)
    {
        hash += toupper(word[i]);
    }
    return (hash % N) - 65;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    initialise(table);
    // open file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Cannot read %s.\n", dictionary);
        return 1;
    }
    // read into hash table
    char buffer[LENGTH + 1];
    while (fscanf(file, "%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        //set node so no valgrind errors
        memset(n, 0, sizeof(node));
        strncpy(n->word, buffer, sizeof(buffer));
        unsigned int index = hash(buffer);
        
        //insert node at start of list
        if (table[index] == NULL)
        {
            table[index] = n;
        }
        else
        {
            n->next = table[index];
            table[index] = n;
        }
        word_count++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }
        else
        {
            while (table[i] != NULL)
            {
                node *tmp = table[i]->next;
                free(table[i]);
                table[i] = tmp;
            }
        }
    }
    return true;
}
