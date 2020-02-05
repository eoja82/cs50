// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//strcasecmp
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
const unsigned int N = 676;
unsigned int word_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //convert word to lowercase becase hash uses ascii numbers
    int length = strlen(word);
    printf("length: %i\n", length);
    char to_lowercase[length + 1];
    for (int i = 0; i < length; i++)
    {
        to_lowercase[i] = tolower(word[i]);
    }
    to_lowercase[length] = '\0';
    printf("lower: %s\n", to_lowercase);
    unsigned int index = hash(to_lowercase);
    printf("index: %i\n", index);
    node *current = table[index];
    //printf("current word: %s, current next: %p\n", table[67]->word, table[67]->next);
    /* while (current->next != NULL)
    {
        printf("in while\n");
        if (strcasecmp(current->word, word) == 0)
        {
            return true;
        }
        else
        {
            current = current->next;
        }
    } */
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash = 0;
    for (int i = 0; i < strlen(word) && i < 2; i++)
    {
        hash += toupper(word[i]);
    }
    
    return (hash % N) - 65;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
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
        strcpy(n->word, buffer);
        n->next = NULL;
        printf("%s, %p\n", n->word, n->next);
        unsigned int index = hash(n->word);
        printf("index: %i, %s\n", index, n->word);
        //find node at end of list
        node *prev = NULL;
        //int counter = 0;
        for (node *curr = table[index]; curr != NULL; curr = curr->next)
        {
            //counter++;
            prev = curr;
        }
        //if list is empty
        if (prev == NULL)
        {
            //printf("prev == NULL\n");
            table[index] = n;
        }
        //else add to end of list
        else
        {
            //printf("prev: %s\n", prev->word);
            prev->next = n;
        }
        word_count++;
        //printf("counter: %i\n", counter);
        if (table[0])
        {
            printf("table0: %s\n", table[0]->word);
        }
        free(n);
    }
    printf("word count: %i\n", word_count);
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
    // TODO
    return false;
}
