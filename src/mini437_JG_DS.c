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
  @brief Debug function used to print the contents of a token array
  */
void printTokens(char **tokens, int bufferSize)
{
    int i;
    for (i = 0; i < (bufferSize); i++)
        printf ("token[%d] = %s\n", i, tokens[i]);
}

/**
  @brief Parse the input into a token array, which will continually resize
  @param input Line input that we want to parse
  */
char **parseInput(char *input)
{
    char **tokens = NULL;
    int loc = 0;
    char *token = strtok(input, " ");

    while (token)
    {
        // Dynamically resize
        tokens = realloc (tokens, ++loc * sizeof (char*));

        // Check for failed memory allocation
        if (tokens == NULL)
        {
            fprintf(stderr, "Unable to allocate memory for parsing\n");
            exit (EXIT_FAILURE);
        }
        tokens[loc-1] = token;

        token = strtok(NULL, " ");
    }

    tokens = realloc (tokens, (loc+1) * sizeof (char*));
    tokens[loc] = 0;

    if (DEBUG)
        printTokens(tokens, loc+1);

    return tokens;
}

int launchCommands(char **command)
{
    int i;

    // Check for empty input
    if (command[0] == NULL)
        return 1;
    return 0; 
}



/**
  @brief Used to run the shell loop and subsequently execute commands.
  */
void shellLoop()
{
    char *input;
    char **parsedInput;
    int bufferSize = 80;
    bool running = true;

    while(running)
    {
        printf("mini437sh-JG-DS: ");
        input = getInput(bufferSize);
        parsedInput = parseInput(input);
        launchCommands(parsedInput);
        free(input);
        free(parsedInput);
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

