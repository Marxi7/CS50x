#include <cs50.h>
#include <stdio.h>
#include<ctype.h>
#include<string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        //we make sure that the user does'nt input more than 2 things (1 is the message 2 is the key)
        printf("Usage: ./caesar key\n");
        return 1; //we return 1 to signal an error
    }

    //making sure that the user input a digit for the key
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i])) //we use i as there can be more than one digit as input. So we need the loop to go through all the i in argv[1]
        {
            printf("Usage: ./caesar key");
            return 1; //we return 1 to signal an error
        }
    }

    //Converting from string to int with the fun atoi
    int k = atoi(argv[1]);

    string plaintext = get_string("Plaintext: ");

    //we put the input ciphertext here because we want to print here before the output result that comes next.
    //If had we put it at the end, we would have ended up with something like : "bciphertext: " instead of "ciphertext: b"
    printf("ciphertext: ");


    for (int j = 0; j < strlen(plaintext); j++)
    {
        //Checking if uppercase
        if (isupper(plaintext[j]))
        {
            printf("%c", (plaintext[j] - 65 + k) % 26 + 65);
        }

        //Checking if lowercaser
        else if (islower(plaintext[j]))
        {
            printf("%c", (plaintext[j] - 97 + k) % 26 + 97);
        }

        else
        {
            printf("%c", plaintext[j]);
        }
    }
    printf("\n");
}