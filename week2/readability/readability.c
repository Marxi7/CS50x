#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


int main(void)
{
    string text;
    text  = get_string("Text :");

    int letters = 0;
    //here we start from one as the words will be counted from the first space, which start after the first word. S we need to already cound that first word.
    int words = 1;
    int sentence = 0;


    //Calculating the nb of letters
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }

        //Calculating the nb of words
        else if (isspace(text[i]))
        {
            words++;
        }

        //Calculating the nb of sentences
        else if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentence++;
        }
    }

    //calculating the average number of letters and sentences
    //we define L & S as float to be precise, but then, we'll round up
    float L = (float) letters / (float) words * 100;
    float S = (float) sentence / (float) words * 100;


    //Index formula - plus, we add the round function to round up the result:
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index >= 16)
    {
        printf("Grade 16+\n");
    }

    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }

    else
    {
        printf("Grade %i\n", index);
    }

}
