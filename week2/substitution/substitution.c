#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    //the command line must only contain the file name and the substitution text(our key)
    if (argc != 2) //checking if command line argument is not equal to 2
    {
        printf("Usage: ./substitution key\n");
        return 1; //error message
    }

    int key = strlen(argv[1]); //to get the length of argv[1], the second command that will come after "./substitution on command line"

    if (key != 26) //to make sure argv length is 26 characters. we use "!" at the beginning as "not".
    {
        printf("Key must contain 26 characters.\n"); //error message
        return 1;
    }

    for (int i = 0; i < key ; i++) //iterate over argv length (key)
    {
        if (!isalpha(argv[1][i]))  //to check if argv[1] is not alphabethic
        {
            printf("Usage: ./substitution key\n");  //error message
            return 1;
        }

        for (int j = i + 1; j < key; j++) //declaring another iteration on argv length (key) to check for repeated alphabeth
        {
            if (argv[1][i] == argv[1][j]) //checking repeated alphabeth in argv[1] (key)
            {
                printf("Key must not contain repeated alphabeth"); //error message
                return 1; //error message
            }
        }
    }

    string plaintext = get_string("Plaintext: "); //Getting user's input as "plaintext"
    printf("ciphertext: "); //printing ciphertext

    int k = strlen(plaintext); //get the length of plaintext

    for (int i = 0; i < k; i++) //iterate over the plaintext length
    {
        if (isupper(plaintext[i])) //check if plaintext character is uppercase
        {
            printf("%c", toupper(argv[1][plaintext[i] - 65]));
        }

        else if (islower(plaintext[i])) //check if plaintext character is lowercase
        {
            printf("%c", tolower(argv[1][plaintext[i] - 97]));
        }

        else //if plaintext contains charachters such as ',', print it like that without any change as we can ignore that for this exercise.
        {
            printf("%c", plaintext[i]); //printing plaintext as it is
        }
    }
    printf("\n"); //printing a new line
}
