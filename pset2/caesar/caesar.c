#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{

    int key = 0;
    int alphaIndex = 0;
    int asciiIndex = 0;

    // Only accept one command-line argument
    if (argc != 2)
    {
        printf("You MUST input only ONE command-line argument! :(\n");
        printf("Usage: ./caesar k\n");
        return 1;
    }
    else
    {
        // Get the key
        key = atoi(argv[1]);
        printf("Your key is %i\n", key);
    }

    // Get the plaintext
    string plaintext = get_string("Enter plaintext: ");

    // Init string to hold cipher
    char *ciphertext = malloc(strlen(plaintext) * sizeof(char));

    // Encipher
    for (int i = 0; i < strlen(plaintext); i++)
    {

        // check for alphabet
        if (isalpha(plaintext[i]))
        {

            // Uppercase letter
            if (isupper(plaintext[i]))
            {

                // convert ascii to alphabet index, calculate
                alphaIndex = (int)plaintext[i] - 65;
                asciiIndex = ((alphaIndex + key) % 26) + 65;
                ciphertext[i] = (char)asciiIndex;
                printf("New UPPERCASE letter: %c\n", (char)asciiIndex);
            }
            // Lowercase letter
            else
            {

                // convert ascii to alphabet index, calculate
                alphaIndex = (int)plaintext[i] - 97;
                asciiIndex = ((alphaIndex + key) % 26) + 97;
                ciphertext[i] = (char)asciiIndex;
                printf("New lowercase letter: %c\n", (char)asciiIndex);
            }
        }
        else
        {
            printf("Not alphabet\n");
            ciphertext[i] = plaintext[i];
            continue;
        }
    }

    //PRINT CIPHER TEXT
    printf("Output ciphertext: %s\n", ciphertext);
    return 0;
}