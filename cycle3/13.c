#include <stdio.h>
#include <string.h>

// Your original global variables
int n;
char prods[50][50];
char firsts[26][50];
int is_first_done[26], is_follow_done[26];
char follows[26][50];

// Your original isTerminal function
int isTerminal(char c)
{
    if (c < 65 || c > 90)
        return 1;
    return 0;
}

// Your original first function
void first(char nonterm)
{
    int index = 0;
    char curr_firsts[50];
    for (int i = 0; i < n; i++)
    {
        if (prods[i][0] == nonterm)
        {
            int curr_prod_index = 2;
            int flag = 0;
            while (prods[i][curr_prod_index] != '\0' && flag == 0)
            {
                flag = 1;
                if (isTerminal(prods[i][curr_prod_index]))
                {
                    curr_firsts[index] = prods[i][curr_prod_index]; // Corrected from prods[i][2]
                    index++;
                    break;
                }
                if (!is_first_done[prods[i][curr_prod_index] - 65])
                    first(prods[i][curr_prod_index]);

                int in = 0;
                while (firsts[prods[i][curr_prod_index] - 65][in] != '\0')
                {
                    curr_firsts[index] =
                        firsts[prods[i][curr_prod_index] - 65][in];
                    if (firsts[prods[i][curr_prod_index] - 65][in] == 'e')
                    {
                        curr_prod_index++;
                        flag = 0;
                    }

                    index++;
                    in++;
                }
            }
        }
    }
    curr_firsts[index] = '\0';
    strcpy(firsts[nonterm - 65], curr_firsts);
    is_first_done[nonterm - 65] = 1;
}

// Your original follow function
void follow(char nonterm)
{
    int index = 0;
    char curr_follows[50];
    if (nonterm == prods[0][0])
    {
        curr_follows[index] = '$';
        index++;
    }
    for (int j = 0; j < n; j++)
    {
        int k = 2;
        int include_lhs_flag;
        while (prods[j][k] != '\0')
        {
            include_lhs_flag = 0;
            if (prods[j][k] == nonterm)
            {
                if (prods[j][k + 1] != '\0')
                {
                    if (isTerminal(prods[j][k + 1]))
                    {
                        curr_follows[index] = prods[j][k + 1];
                        index++;
                        // break; // Removed break to handle cases like A=bA'a
                    }
                    int in = 0;
                    while (firsts[prods[j][k + 1] - 65][in] != '\0')
                    {
                        if (firsts[prods[j][k + 1] - 65][in] == 'e')
                        {
                            include_lhs_flag = 1;
                            in++;
                            continue;
                        }
                        int temp_flag = 0;
                        for (int z = 0; z < index; z++)
                            if (firsts[prods[j][k + 1] - 65][in] == curr_follows[z])
                            {
                                temp_flag = 1;
                                break; // Broke inner loop, not while
                            }
                        if (temp_flag) {
                            in++; // Still need to advance 'in'
                            continue;
                        }
                        curr_follows[index] = firsts[prods[j][k + 1] - 65][in];
                        index++;
                        in++;
                    }
                }
                if (prods[j][k + 1] == '\0' || include_lhs_flag == 1)
                {
                    if (prods[j][0] != nonterm)
                    {
                        if (!is_follow_done[prods[j][0] - 65])
                            follow(prods[j][0]);
                        int x = 0;
                        while (follows[prods[j][0] - 65][x] != '\0')
                        {
                            int temp_flag = 0;
                            for (int z = 0; z < index; z++)
                                if (follows[prods[j][0] - 65][x] == curr_follows[z])
                                {
                                    temp_flag = 1;
                                    break; // Broke inner loop
                                }
                            if (temp_flag) {
                                x++;
                                continue;
                            }
                            curr_follows[index] = follows[prods[j][0] - 65][x];
                            index++;
                            x++;
                        }
                    }
                }
            }
            k++;
        }
    }
    curr_follows[index] = '\0';
    strcpy(follows[nonterm - 65], curr_follows);
    is_follow_done[nonterm - 65] = 1;
}

// ****** THE ONLY PART ADDED ******
int main()
{
    printf("Enter the number of productions: ");
    scanf("%d", &n);

    printf("Enter the productions (e.g., E=E+T, use 'e' for epsilon):\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%s", prods[i]);
    }

    // Find all unique non-terminals to process
    char non_terminals[26];
    int nt_count = 0;
    for (int i = 0; i < n; i++)
    {
        int found = 0;
        for (int j = 0; j < nt_count; j++)
        {
            if (non_terminals[j] == prods[i][0])
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            non_terminals[nt_count++] = prods[i][0];
        }
    }

    // Calculate and print FIRST sets using your function
    printf("\n--- FIRST Sets ---\n");
    for (int i = 0; i < nt_count; i++)
    {
        first(non_terminals[i]);
        printf("FIRST(%c) = { %s }\n", non_terminals[i], firsts[non_terminals[i] - 65]);
    }

    // Calculate and print FOLLOW sets using your function
    printf("\n--- FOLLOW Sets ---\n");
    for (int i = 0; i < nt_count; i++)
    {
        follow(non_terminals[i]);
        printf("FOLLOW(%c) = { %s }\n", non_terminals[i], follows[non_terminals[i] - 65]);
    }

    return 0;
}