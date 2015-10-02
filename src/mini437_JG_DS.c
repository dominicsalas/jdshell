/**
 * @file   mini437_JG_DS.c
 * @author Jayson Grace (jaysong@unm.edu)
 * @author Dominic Salas
 * @date   9/27/2015
 * @brief  Shell for PA04.
 */

#include <stdio.h>
#include <stdlib.h>
// Gives us strtok
#include <string.h>
// Gives us fork and execvp
#include <unistd.h>
// Gives us waitpid
#include <sys/wait.h>
// Gives us rusage
#include <sys/resource.h>
// Gives us signal for signal handling
#include <signal.h>

void printLastTen();

typedef int bool;
enum { false, true };

typedef struct
{
    // Tokens the container will hold
    char** tokens;

    // Number of tokens the container has
    int tokenCount;
} TokenContainer;

typedef struct
{
    // Store background job pids
    pid_t backgroundJobs[1024];

    // Number of background jobs running
    int bgJobCounter;
} bgContainer;

typedef struct
{
    //A 2d array to record the last 10 entries
    char last10[10][64];
    // Count of commands
    int commandCount;
} CommandContainer;

enum timeType {USER, SYSTEM};

// Specify whether or not debug mode should be run
bool DEBUG = false;

CommandContainer commands;

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
  @brief Print required pre-run information line
  @param tokens The token array with the information we want to print
  */
void preRun(TokenContainer *tc)
{
    if (DEBUG)
        printf("Number of tokens in the TokenContainer: %d\n", tc->tokenCount);

    printf("PreRun: %s ", tc->tokens[0]);

    int i;
    for (i = 1; i < tc->tokenCount; i++)
    {
        printf("%d:%s", i, tc->tokens[i]);
        // Ensure we don't print a comma at the end of the print output
        if (i != tc->tokenCount-1)
            printf(",");
    }
    // Used for aesthetics
    printf("\n");
}

/**
  @brief Get the completion time for the current command
  @param type system or user time
  @param startTime The start time for the current command
  */
void getCompletionTime(enum timeType type, struct rusage* startTime)
{
    struct rusage currentTime;
    getrusage(RUSAGE_CHILDREN, &currentTime);

    if (type == USER)
    {
        printf("user time %ld.%06ld ", currentTime.ru_utime.tv_sec - startTime->ru_utime.tv_sec,
                (long)currentTime.ru_utime.tv_usec - startTime->ru_utime.tv_usec);
    }
    else
    {
        printf("system time %ld.%06ld\n", currentTime.ru_stime.tv_sec - startTime->ru_stime.tv_sec,
                (long)currentTime.ru_stime.tv_usec - startTime->ru_stime.tv_usec);
    }
}

/**
  @brief Print required post-run information line
  @param tc TokenContainer struct with both the tokens and their count
  @param start The starting time of the program
  @param child The child process id
  */
void postRun(TokenContainer *tc, struct rusage* start, pid_t child)
{
    printf("PostRun(PID:%d): %s -- ", child, tc->tokens[0]);
    enum timeType type = USER;
    getCompletionTime(type, start);
    type = SYSTEM;
    getCompletionTime(type, start);
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
  @brief Check to see if the input command should be a background job
  @param tc TokenContainer struct with both the tokens and their count
  @return true if background job, false otherwise
  */
bool checkBackgroundJob(TokenContainer *tc)
{
    if (strcmp(tc->tokens[tc->tokenCount-1], "&") == 0)
    {
        if (DEBUG)
            printf("Background job requested\n");
        return true;
    }
    return false;
}

/**
@brief Add command to history
@param tc TokenContainer struct with both the tokens and their count
*/
void addToHistory(TokenContainer *tc)
{
    int i = 0;

    // If ten commands are in history, delete the first one and shift the rest up
    if (commands.commandCount == 10)
    {
        commands.commandCount = 9;
        for (i = 1; i < 10; i++)
        {
            *commands.last10[i-1] = '\0';
            strcpy(commands.last10[i-1], commands.last10[i]);
        }
        *commands.last10[commands.commandCount] = '\0';
    }
    else
        if (DEBUG)
            printLastTen();

    strcpy(commands.last10[commands.commandCount++], *tc->tokens);

    if (DEBUG)
    {
        printf("\nAfter Override: \n");
        printLastTen();
    }

    if (DEBUG)
        printf("Count: %d\n", commands.commandCount);

}

/**
  @brief Launch program made up of tokens in parameter TokenContainer and terminate when done
  @param tc TokenContainer struct with both the tokens and their count
  @return true to continue execution of the shell
  */
bool launchCommands(TokenContainer *tc)
{
    pid_t child;
    struct rusage start;
    //    bool bg = checkBackgroundJob(tc);

    preRun(tc);
    getrusage(RUSAGE_CHILDREN, &start);

    addToHistory(tc);
    child = fork();

    // Make sure pid is child process
    if (child == 0)
    {
        if (strcmp(tc->tokens[0], "last10") == 0)
            printLastTen();
        else if (execvp(tc->tokens[0], tc->tokens) == -1)
        {
            perror("Failed on child process in launchCommands");
        }
        exit(EXIT_FAILURE);
    }
    else if (child < 0)
    {
        perror("Error forking in launchCommands");
    }
    // Parent processes code
    else
    {
        int returnStatus;
        waitpid(child, &returnStatus, 0);
        postRun(tc, &start, child);
    }
    return true;
}

/**
 * @brief Print the last ten commands input
 */
void printLastTen()
{
    int i;
    // If no commands exist yet
    if (commands.commandCount == 0)
        printf("\n");
    for (i = 0; i < commands.commandCount; i++)
    {
        printf("%d:%s\n", i, commands.last10[i]);
    }
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
  @brief Used to handle SIG INTs
  @param signalNumber The associated SIG INT
  */
void signalHandler(int signalNumber)
{
    signal(SIGINT, signalHandler);
    printLastTen();
    fflush(stdout);
}

/**
  @brief Entry into program
  @param argc Argument count
  @param argv Argument vector
  @return Success (or lackthereof) of program's execution
  */
int main(int argc, char **argv)
{
    signal(SIGINT, signalHandler);
    commands.commandCount = 0;
    shellLoop();
    return EXIT_SUCCESS;
}

