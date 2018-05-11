/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

//Alphabet size (plus '\')
#define INPUT_SIZE (27)

//converts word's current character into index
//only accept 'a' through 'z' and apostrophe " ' "
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

//trie based on www.geeksforgeeks.org
//              www.techiedelight.com
typedef struct node 
    {
        struct node *children[INPUT_SIZE];
        
        //isWord is true if the node represents end of word
        bool isWord;
    }
    node;
    
void unloadNodes(struct node *r);
void unloadNodes_rec(struct node *t);

//Returns new trie node initialised to NULL
struct node *getNode(void)
{
    //initialise new node
    struct node *newNode = NULL;
    
    newNode = calloc(1, sizeof(struct node));
    
    //proceed if enough memory to allocate
    if(newNode) 
    {
        //initialise pointers
        for(int i = 0; i < INPUT_SIZE; i++)
            newNode->children[i] = NULL;
            
        newNode->isWord = false;
    }
    else return false;
    
    return newNode;
}

//Define variables
    struct node *root;
    int word_Count = 0;
    bool fileLoaded = false;

//If word not present, inserts word into trie
//If the word is a prefix of a node, marks the "leaf node" (end-of-word node)
void insert(struct node *root, const char *word)
{
    int length = strlen(word);
    int index = 0;
    
    //start from root node
    //TODO: FIND OUT WHAT THIS DOES
    struct node *tempNode = root;
    
    //safety measures
    tempNode = malloc(sizeof(struct node));
    for(int i = 0; i < INPUT_SIZE; i++)
            tempNode->children[i] = NULL;
    
    for(int i = 0; i < length; i++)
    {
        //if the current letter in word is a letter
        if(word[i] != '\'')
        
            //convert the alphabet to it's respective index number
            index = CHAR_TO_INDEX(tolower(word[i]));
            
        else
            //assign index number 27 (for apostrophe)
            index = INPUT_SIZE;
            
        //create a new node if path doesn't exist (is NULL)
        if((tempNode->children[index]) == NULL)
            tempNode->children[index] = getNode();
        
        //go to next node  
        tempNode = tempNode->children[index];
    }
    //mark last node as leaf
    tempNode->isWord = true;
}

void unloadNodes_rec(struct node *t)
{
    //If not NULL, begin unloading
    if(t) 
    {
        for(int i = 0; i < LENGTH; i++)
        {
            if(t->children[i])
            {
                unloadNodes(t->children[i]);
                //A failsafe
                t->children[i] = NULL;
            }
        }
        free(t);
    }
}

void unloadNodes(struct node *r)
{
    //If root node not NULL, free children, then the root node
    if(r)
    {
        unloadNodes_rec(r);
        free(r);
    }
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    //initialise variables
    int wordLength = strlen(word);
    int index;
    
    //create a 'copy' of root
    struct node *tempNode = root;
    
    //convert input word to lowercase. Set last byte of input to null terminator
    char *input = (char*) malloc(wordLength +1);
    input[wordLength] = 0;
    
    for(int f = 0; f < wordLength; f++)
        input[f] = tolower(word[f]);
    
    for(int i = 0; i < wordLength; i++)
    {
        //get index of letter being "scanned"
        index = CHAR_TO_INDEX(input[i]);
        
        //assign index 27 if apostrophe
        if(input[i] == '\'')
        index = INPUT_SIZE;
        
        //if child element is NULL, word misspelled
        if(!tempNode->children[index])
            return false;
            
        //Otherwise continue checking up the trie
        else
            tempNode = tempNode->children[index];
    }
    
    //Once leaf is found, check if word exists. free memory
    free(input);
    return tempNode->isWord;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    //initialise variables
    char ch;
    char word[LENGTH] = "";
    int counter = 0;
    
    struct node *root = getNode();
    
    //open file to start inserting words
    FILE *file = fopen(dictionary, "r");
    
    //load words in dictionary into memory
    while ((ch = fgetc(file)) != EOF)
    {
        if (ferror(file) != 0)
        {
            printf("ERROR FOUND!");
            return 1;
        }
        
        while((ch = fgetc(file)) != '\n')
        {
            word[counter] = ch;
            counter++;
        }
        //whole word found, insert word and reset counter, increment word count
        word[counter + 1] = '\0';
        insert(root, word);
        counter = 0;
        word_Count++;
    }
    
    //close all open files if no errors, else loading has failed- return false
    if(ferror(file) == 0)
    {
        fileLoaded = true;
        fclose(file);
        return true;
    }
    else return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if(fileLoaded)
        return word_Count;
    else
        return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    unloadNodes(root);
    return true;
}
