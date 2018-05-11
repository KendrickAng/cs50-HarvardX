/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

#define SIZE 200000

unsigned int hashWord(const char *word);

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//Create array of nodes pointing to NULL
node *hashTable[SIZE] = {NULL};

//Global variable for dictionary size
int dictionarySize = 0;

/** 
 * Hash function from djb2.
 * Hashes a string then returns an unsigned int.
 */
unsigned int hashWord(const char *word)
{
    int counter;
    unsigned int hashAddress = 0;
    
    for(counter = 0; word[counter] != '\0'; counter++)
        hashAddress = word[counter] + (hashAddress << 6) + (hashAddress << 16) - hashAddress;
 
    return (hashAddress % SIZE);
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    //convert word to lowercase
    char lowerWord[LENGTH + 1];
    int lengthWord = strlen(word);
    
    for(int i = 0; i < lengthWord; i++)
        lowerWord[i] = tolower(word[i]);
        
    //insert back '\0'
    lowerWord[lengthWord] = '\0';
    
    //Find index in array to check
    int index = hashWord(lowerWord);

    //Check if linked list exists
    if(hashTable[index] == NULL)
        return false;
        
    /**
     * Begin the real checking
     */
    node *cursor = hashTable[index];
    
    while(cursor != NULL)
    {
        //compare strings to find a match
        //strcasecmp is case-insensitive
        if(strcmp(lowerWord, cursor->word) == 0)
            return true;
        
        //otherwise move on to the next node in the current linked list
        else
            cursor = cursor->next;
    }
    
    //If no matches found, exit
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    //array for output word to be stored in
    //int to store hashed word's index
    char word[LENGTH + 1];
    int index;
    
    //open dictionary file
    FILE *file = fopen(dictionary, "r");
    
    //insert words into hash table
    while(fscanf(file, "%s", word) != EOF)
    {
        //increment dictionary size
        dictionarySize++;
        
        //malloc new node for new word
        node *newNode = malloc(sizeof(node));
        
        //If not enough memory to allocate, exit
        if(newNode == NULL)
        {
            unload();
            return false;
        }
        
        //copy word into new node
        strcpy(newNode->word, word);
        
        //find which index in the array it should go to (by hashing)
        index = hashWord(newNode->word);
        
        //insert word into the linked list
        //Condition #1: Hash table is empty, insert new link
        if(hashTable[index] == NULL)
        {
            hashTable[index] = newNode;
        }
        //Condition #2: Hash table not empty, append new link
        else
        {
            newNode->next = hashTable[index];
            hashTable[index] = newNode;
        }
    }
    
    //close dictionary file
    fclose(file);

    //If function completes, it is successful
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    //If dictionarySize has been incremented, return size
    if(dictionarySize > 0)
        return dictionarySize;
    
    else
        return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    
    //initialise variables
    int index = 0;
    node *cursor = hashTable[index];
    
    //iterate through each linked list in the hash table
    while(index <= SIZE)
    {
        //move to next linked list if current one is empty
        if(hashTable[index] == NULL)
            index++;
        
        //otherwise unload linked list
        else
        {
            while(cursor != NULL)
            {
                node *temp = cursor;
                cursor = cursor->next;
                free(temp);
            }
            
            //current linked list is unloaded, move to the next
            index++;
        }
    }
    
    //if function completes, success!
    return true;
}
