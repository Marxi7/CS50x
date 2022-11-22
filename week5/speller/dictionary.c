// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

//variable added
unsigned int count_word, value_of_hash;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    //to obtain a hash value, we have to hash the word.
    value_of_hash = hash(word);

    //pointing the cursor to the first element of the charachter array
    node *cursor = table[value_of_hash];

    //Going through the whole linked list, checking that the cursor is valid with != 0
    while (cursor != 0)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}


// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    //Ref idea : https://www.youtube.com/watch?v=KyUTuwz_b7Q&t=610s (address = key Mod n)
    //https://www.youtube.com/watch?v=FsfRsGFHuv4 the beginning of the vide with the animation helped me visualize how it works better.
    unsigned long sum_ascii_values = 0;
    int address;
    for (int i = 0; i < strlen(word); i++)
    {
        //additioning every letter of the word in terms of ascii value to get the key
        sum_ascii_values = sum_ascii_values + tolower(word[i]);
        address = sum_ascii_values % N;
    }
    return address; //this will return the reminder of the sum of the ascii values
}



// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    //opening the dictionary using the function fopen
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }

    //declaring the variable 'word'
    char word[LENGTH + 1];

    //while loop to read the entire file until we reach EOF
    while (fscanf(file, "%s", word) != EOF)
    {
        //asking the computer for some memory space
        node *n = malloc(sizeof(node));

        //making sure than n is not NULL before continuing
        if (n == NULL)
        {
            return false;
        }

        //Copying the word into the charachter array which is the word field of this node we just created some space for above.
        strcpy(n->word, word);

        //Hash value using the hash function provided
        value_of_hash = hash(word);

        //setting the pointer of the new node to the current head of the table
        n->next = table[value_of_hash];

        //set the new node to the head of the list
        table[value_of_hash] = n;

        //keeping track of the count of the number of words
        count_word++;
    }
    fclose(file);
    return true;
}


// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (count_word > 0)
    {
        return count_word;
    }
    return 0;
}


// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    //Iterating trhough the buckets
    for (int i = 0; i < N; i++)
    {
        //Setting the cursor created to the beginning of the linked list
        node *cursor = table[i];

        //Again, checking if the cursor is valid with ==NULL, if it's not NULL, we free the memory used
        while (cursor != NULL)
        {
            //Create tmp
            node *tmp = cursor;

            //the cursor then move to the next node
            cursor = cursor->next;

            //Free up temp
            free(tmp);
        }

        //checking if we arrive at the end of the hash table and if cursor is NULL
        if (cursor == NULL && i == N - 1) //if cursor is null and we are at the end of the "dictionary" by saying N-1
        {
            return true;
        }
    }
    return false;
}
