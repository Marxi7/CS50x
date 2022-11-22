#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // here we check if the name of the candidate is valid and matches with the name in the candidate array
    //then, the ranks are updated with each candidate name that has been found. 1D array here for each rank.
    // one rank for example will look like that if there are 3 candidates : [0, 2, 1]
    for (int i = 0; i < candidate_count; i++)
    {
        if(strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // references will be a 4x4 table if there are 4 candidates per example as it is a 2D array. As we have more than one candidate, this explains why it is a 2D array.
    // one rank for example will look like that if there are 3 candidates : [0, 2, 1] qui veut dire : 1ère Alice, 2ème charlie (3ème candidat), 3ème Bob (2ème candidat)
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++) //j coniste à nous faire ajouté 1 points (de pénalité) à tous ceux qui viennent après i car dans l'ordre du vote ils sont après le 1er
        {
            // for example here, imaginons [0, 2, 1] ou 0 est i est le premier candidat voté,
            //et j est le candidat juste après (carj=  i+1), donc (2), à l'intersection de ces deux pointeurs dans le tableau, un point sera ajouté.
            //Car l'array est la préférence des voteurs dans l'ordre.
            //C'est pourquoi on compare d'abord de gauche à droite (car la gauche est préférée à la droite)
            //preferences [i][j] = le nombre de personne qui préfère i à j (de gauche à droite.)
            preferences[ranks[i]][ranks[j]]++;
        }

    }
    return;
}

// Record pairs of candidates where one is preferred over the other
//Here we weill compare in the 2D preference table which candidate has more vote than another one.
//to do so, we will compare for example, in the table,  0-1 with 1-0. If for example (0-1) is 2 and (1-0) is 0, (0-1) get one more count added as it is > (1-0)
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int candi_1 = preferences[i][j];
            int candi_2 = preferences[j][i];

            if (candi_1 > candi_2)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                ////ici, dès que le programme a choisis la paire la plus grande, le ++ signifie simplement qu'il faut passer à la next step dès que c'est fait.
                //En l'occurence ici, réitérer jusqu'é ce que toutes les paires soient à jour.
                pair_count++;
            }
            else if (candi_1 < candi_2)
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
// maintenant ici on va se retrouver avec des paires créées avant qui ressemblent style à ça : (6 -2)
// on va devoir les trier dans un l'ordre de la force de victoire. Par exemple si on a :
// (10-5) (6-5) (8-1) -> [5, 1, 7] -> [7, 5, 1]  (ordre du plus grand au plus petit de la différence de vote; force de victoire)
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < (pair_count -1 - i); j++)
        {
            if ((preferences[pairs[j].winner][pairs[j].loser]) < (preferences[pairs[j + 1].winner][pairs[j + 1].loser])) //remember, preferences for for example (10-5) is 5. As it is the number of people who have a preference for i over j.
            {
                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
            }
        }
    }
    return;
}


bool has_a_cycle(int loser, int winner)
{
    //this will return true if a cycle is created, if true, it will continue until it's false.
    if (loser == winner)
    {
        return true;
    }
    //recursion happens here
    for (int i = 0; i < candidate_count; i++)
    {
        if(locked[loser][i] && (has_a_cycle(i, winner)))
            {
                return true;
            }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        if(!has_a_cycle(loser, winner))
        {
            locked[winner][loser] = true;
        }
    }

}



// Print the winner of the election
void print_winner(void)
{
    // TODO
    //Looking for a candidate that has no arrows pointing at them
    for (int i = 0; i < candidate_count; i++)
    {
        bool candidate_with_less_count = true;

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true) //if there is a cycle between the loser and the winner
            {
                candidate_with_less_count = false;
                break;
            }
        }

        if(candidate_with_less_count == true)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}