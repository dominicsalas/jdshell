/**
 * @file   mini437_JG_DS.c
 * @author Jayson Grace (jaysong@unm.edu)
 * @author Dominic Salas
 * @date   9/27/2015
 * @brief  Shell for PA04.
 */

#include <stdio.h>
#include <stdlib.h>
// Gives us isspace
#include <ctype.h>
// Gives us strtok
#include <string.h>

typedef int bool;
enum { false, true };

typedef struct
{
    // Tokens the container will hold
    char** tokens;

    // Number of tokens the container has
    int tokenCount;
} TokenContainer;

bool DEBUG = true;

/**
  @brief Used to get the line input by the user.
  */
char *getInput(int buffer)
{
    char *lineInput = NULL;
    size_t bufferSize = buffer;

    getline(&lineInput, &bufferSize, stdin);  
    return lineInput;
}

/**
  @brief Debug function used to print the contents of a token array within a TokenContainer
  @param tokens The token array we want to print
  @param tokenArrSize The size of the token array
  */
void printTokens(TokenContainer *tc)
{
    printf("Number of tokens in the TokenContainer: %d\n", tc->tokenCount);

    int i;
    for (i = 0; i < tc->tokenCount; i++)
        printf ("token[%d] = %s\n", i, tc->tokens[i]);
}

/**
  @brief Parse the input into a token array, which will continually resize
  @param input Line input that we want to parse
  */
TokenContainer parseInput(char *input)
{
    TokenContainer tc;
    tc.tokens = NULL;
    tc.tokenCount = 0;
    int loc = 0;
    char *token = strtok(input, " ");

    while (token)
    {
        // Dynamically resize
        tc.tokens = realloc (tc.tokens, ++loc * sizeof (char*));

        // Check for failed memory allocation
        if (tc.tokens == NULL)
        {
            fprintf(stderr, "Unable to allocate memory for parsing\n");
            exit (EXIT_FAILURE);
        }
        tc.tokens[loc-1] = token;
        tc.tokenCount++;

        token = strtok(NULL, " ");
    }

    tc.tokens = realloc (tc.tokens, (loc+1) * sizeof (char*));
    tc.tokens[loc] = 0;

    return tc;
}

int launchCommands(TokenContainer *tc)
{
    // Check for empty input
    if (tc->tokens[0] == NULL)
        return 1;
    return 0;
}

/**
  @brief Used to run the shell loop and subsequently execute commands.
  */
void shellLoop()
{
    char *input;
    TokenContainer tc;
    int bufferSize = 80;
    bool running = true;

    while(running)
    {
        printf("mini437sh-JG-DS: ");
        input = getInput(bufferSize);
        tc = parseInput(input);
        if (DEBUG)
            printTokens(&tc);
        launchCommands(&tc);
        free(input);
        free(tc.tokens);
    }
}
/** 
  @brief Entry into program
  @param argc Argument count
  @param argv Argument vector
  @return Status code
  */
int main(int argc, char **argv) 
{
    shellLoop();
    return 0; 
}

