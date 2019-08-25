/* smallsh header program */
/*Include a BUNCH of libraries, I used most of these in the last assignment*/
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/*Define max arguements so I can easily forget about them*/
#define MAX_CHARS 2048
#define MAX_ARGS 512
#define MAX_FILE 200
#define MAX_PID 6

/*Struct packaging all my important values*/
typedef struct commandLine{
  char commandInput[MAX_CHARS];
  char *parsedInput[MAX_ARGS];
  char inputFileName[MAX_FILE];
  char outputFileName[MAX_FILE];
  char *primaryCommand;
  int containsAmpersand;
  int containsInputFile;
  int containsOutputFile;
  int parsedSize;
  int childExitStatus;
  char processID[MAX_PID];
  struct sigaction sigIntHandler;
  struct sigaction sigStpHandler;
} commandLine;

/*Function IMplementation*/
void commandPrompt(struct commandLine *cl);
void commandParser(struct commandLine *cl);
void commandExecutor(struct commandLine *cl);
void resetCommandLine(struct commandLine *cl);
void checkStructStatus(struct commandLine *cl);
void insertEndLine(struct commandLine *cl);
void exitRoutine(struct commandLine *cl);
void cdRoutine(struct commandLine *cl);
void statusRoutine(struct commandLine *cl);
void shellRoutine(struct commandLine *cl);
void initSigStructures(struct commandLine *cl);
void initProcessString(struct commandLine *cl);
void stringProcessesReplacer(struct commandLine *cl);
void catchSIGSTP(int signo);
