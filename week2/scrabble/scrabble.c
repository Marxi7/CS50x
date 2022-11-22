#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }

    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }

    else
    {
        printf("Tie!\n");
    }


}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    //creating score
    int score = 0;

    //Iterating through each charachter
    for (int i = 0; i < strlen(word); i++)
    {

        //Checking if lowercase
        if (islower(word[i]))
        {
            score += POINTS[ word[i] - 97 ]; //the differnce is always 97 between the index given here and the ascii table.
        }

        //Checking if uppercase
        else if (isupper(word[i]))
        {
            score += POINTS[ word[i] - 65 ]; //65 because a is in the 65th position in the ascii table and in here, it's in the 0 index position. B is 66 and 1 in the asci.. and so on.. so the difference is always 65
        }
        else
        {
            continue;
        }
    }
    return score;
}
