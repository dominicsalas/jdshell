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
// Gives us fork and execvp
#include <unistd.h>
// Gives us waitpid
#include <sys/wait.h>
// Gives us rusage
#include <sys/resource.h>

typedef int bool;
enum { false, true };

typedef struct
{
    // Tokens the container will hold
    char** tokens;

    // Number of tokens the container has
    int tokenCount;
} TokenContainer;

bool DEBUG = false;



/**
  @brief Used to get the line input by the user.
  */
#define BUFFER_SIZE 64
char *getInput()
{
    char *lineInput = NULL;
    size_t bufferSize = BUFFER_SIZE;

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
  @return TokenContainer struct with both the tokens and their count
  */
#define DELIMITER " \t\r\n\a"
TokenContainer parseInput(char *input)
{
    TokenContainer tc;
    tc.tokens = NULL;
    tc.tokenCount = 0;
    int loc = 0;
    char *token = strtok(input, DELIMITER);


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

        token = strtok(NULL, DELIMITER);
    }

    tc.tokens = realloc (tc.tokens, (loc+1) * sizeof (char*));
    tc.tokens[loc] = 0;

    return tc;
}


/**
  @brief Launch program made up of tokens in parameter TokenContainer and terminate when done
  @param tc TokenContainer struct with both the tokens and their count
  @return true to continue execution of the shell
  */
bool launchCommands(TokenContainer *tc)
{
    pid_t pid;
    pid = fork();

    // Make sure pid is child process
    if (pid == 0)
    {
        if (execvp(tc->tokens[0], tc->tokens) == -1)
        {
            perror("Failed on child process in launchCommands");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("Error forking in launchCommands");
    }
    // Parent processes code
    else
    {
        int returnStatus;
        waitpid(pid, &returnStatus, 0);
    }
    return true;
}

/**
  @brief Validate if a command has been input
  @param tc TokenContainer struct with both the tokens and their count
  @return true if no input, false otherwise
  */
bool emptyInput(TokenContainer *tc)
{
    if (tc->tokens[0] == NULL)
    {
        printf("\n");
        if (DEBUG)
            printf("You didn't enter anything. Try again.\n");
        return true;
    }
    return false;
}

/**
  @brief Determine if the exit command has been input
  @param tc TokenContainer struct with both the tokens and their count
  @return true if exit requested, false otherwise
  */
bool exitRequested(TokenContainer *tc)
{
    if (strcmp(tc->tokens[0], "exit") == 0)
        return true;
    return false;
}

/**
  @brief Loop getting input and executing it.
  */
void shellLoop()
{
    char *input;
    TokenContainer tc;
    bool running = true;

    while(running)
    {
        printf("mini437sh-JG-DS: ");
        input = getInput();
        tc = parseInput(input);
        if (!emptyInput(&tc))
        {
            if (exitRequested(&tc))
                running = false;
            else
            {
                running = launchCommands(&tc);
            }
        }
        free(input);
        free(tc.tokens);
    }
    //  terminateBackgroundJobs
}

/**
  @brief Entry into program
  @param argc Argument count
  @param argv Argument vector
  @return Success (or lackthereof) of program's execution
  */
int main(int argc, char **argv)
{
    shellLoop();
    return EXIT_SUCCESS;
}
