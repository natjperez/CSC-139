// ConsoleApplication1.c : Simple Command Line Interpreter
// Name(s): Jose Ordaz, Jessica Nguyen, Viktoriya Penkova, Natalia Perez, Lauren Prather, Ben Pennington
//Group 17

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>

int system(const char *command);

/* Pass commands as parameters */
DWORD WINAPI argThread(LPVOID in) { 
  system(in);
 }

int main(void) {
  char userInput[100];             // Array that holds user input up to 100 characters long (tokens)
  char userInputCopy[100];         // Copy the values from userInput to this array            
  char *arguments[4];              // Array that holds a total of 4 tokens, referred to as arguments
  char *token;                     // Points to a token
  int tokenNum = 0;                // Total number of tokens
  int i;
  DWORD ThreadId;
  HANDLE ThreadHandle;
  printf("Welcome to myShell\n");

  while (1) {
    printf("\n==> ");

    /* Read string from user and store into the userInput array.
       Copy the values from userInput to userInputCopy. */
    fgets(userInput, 100, stdin);
    strcpy(userInputCopy, userInput);

    /* Retrieves the first token, which is separated by a space. */
    token = strtok(userInput, " ");

    /* Write tokens into the arguments array. */
    i = 0;
    while (token != NULL) {
      arguments[i] = token;
      token = strtok(NULL, " ");
      tokenNum++; 
      i++;
    }

    /* Removing \n allows arguments to be entered. */ 
    if(strcmp(arguments[0], "exit\n") == 0 || strcmp(arguments[0], "quit\n") == 0) {
      printf("Thanks for using myShell!\n");
      /* Resets the arguments array. */
      for(int j = 0; j < sizeof(&arguments); j++) {
        arguments[j] = NULL;
      }
      exit(EXIT_SUCCESS);
    }

    /* Resets the number of tokens. */
    tokenNum = 0; 
    
    /* Compares the list of commands with strings. */
    if(strcmp(arguments[0], "dir\n") == 0 || strcmp(arguments[0], "help\n") == 0 || strcmp(arguments[0], "vol\n") == 0
    ||strcmp(arguments[0], "path\n") == 0 || strcmp(arguments[0], "tasklist\n") == 0 || strcmp(arguments[0], "notepad\n") == 0||
    strcmp(arguments[0], "echo\n") == 0 || strcmp(arguments[0], "color\n") == 0 || strcmp(arguments[0], "ping\n") == 0) {
      ThreadHandle = CreateThread(NULL, 0, argThread, arguments[0], 0, &ThreadId);
      WaitForSingleObject(ThreadHandle, INFINITE);
      CloseHandle(ThreadHandle);
    }

    /* Compares the list of commands with strings that require multiple parameters. */
    else if (strcmp(arguments[0], "dir") == 0 || strcmp(arguments[0], "help") == 0 || strcmp(arguments[0], "vol") == 0 ||strcmp(arguments[0], "path") == 0 ||
      strcmp(arguments[0], "tasklist") == 0 || strcmp(arguments[0], "notepad") == 0 || strcmp(arguments[0], "echo") == 0 || 
      strcmp(arguments[0], "color") == 0 || strcmp(arguments[0], "ping") == 0) {
      ThreadHandle = CreateThread(NULL, 0, argThread, userInputCopy, 0, &ThreadId);
      WaitForSingleObject(ThreadHandle, INFINITE);
      CloseHandle(ThreadHandle);
    }

    /* Prints an error message if a non-existing command is typed or certain parameters are excluded. */
    else {
      printf("ARGUMENT NOT FOUND OR MISSING PARAMETERS.\n\n");
    }
  }
  return 0;
}
