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
int get_index(int x, int ranks[]);
int get_diff(int i, int j);
bool cycle(int i, int j);
bool find_winner(int i);

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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
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
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (strcmp(candidates[i], candidates[j]) == 0)
            {
                continue;
            }
            else
            {
                int i_rank = get_index(i, ranks);
                int j_rank = get_index(j, ranks);
                //if i or j not found in ranks
                if (i_rank < 0 || j_rank < 0)
                {
                    continue;
                }
                else if (i_rank > j_rank)
                {
                    preferences[j][i]++;
                }
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count - 1; i++)
    {   
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (i == j || preferences[i][j] == preferences[j][i])
            {
                continue;
            }
            else if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else
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
void sort_pairs(void)
{
    bool swapped = true;
    while (swapped)
    {
        for (int k = 0; k < pair_count - 1; k++)
        {   
            int swap_count = 0;
            int diff_k = get_diff(pairs[k].winner, pairs[k].loser);
            int diff_kplus1 = get_diff(pairs[k + 1].winner, pairs[k + 1].loser);
            if (diff_kplus1 > diff_k)
            {
                int temp_winner = pairs[k].winner;
                int temp_loser = pairs[k].loser;
                pairs[k].winner = pairs[k + 1].winner;
                pairs[k].loser = pairs[k + 1].loser;
                pairs[k + 1].winner = temp_winner;
                pairs[k + 1].loser = temp_loser;
                swap_count++;
            }
            if (swap_count == 0)
            {
                swapped = false;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{   
    for (int i = 0; i < candidate_count; i++)
    {
        if (find_winner(i))
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}
//get position candidate was voted for
int get_index(int x, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (x == ranks[i])
        {
            return i;
        }
    }
    return -1;
}
//get the spread between winner and loser
int get_diff(int i, int j)
{
    int diff = preferences[i][j] - preferences[j][i];
    return diff;
}
bool cycle(int i, int j)
{
    if (locked[j][i])
    {
        return true;
    }
    for (int k = 0; k < pair_count; k++)
    {
        if (locked[k][i])
        {
            if (cycle(k, j))
            {
                return true;
            }
        }
    }
    return false;
}
//if no locks (true) in candidates COLUMN they are the winner
bool find_winner(int i)
{
    for (int j = 0; j < candidate_count; j++)
    {
        if (locked[j][i])
        {
            return false;
        }
    }
    return true;
}