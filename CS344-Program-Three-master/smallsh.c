/* smallsh primary program */
/*Include the header file for file references*/
#include "smallsh.h"
/*Create Universal forground variable*/
int foregroundOnly = 0;

/*Command Prompt Function ... Contains the promt and Primary function calls*/
void commandPrompt(struct commandLine *cl){
  /*Loop forever till a break or exit*/
  while(1){
    /*Display prompt*/
    printf(": ");
    fflush(stdout);
    fgets(cl->commandInput, MAX_CHARS, stdin);
    /*Check for non-exec terms*/
    if(strcmp(cl->commandInput,"\n") && strcmp(cl->commandInput,"") && cl->commandInput[0] != '#'){
      /*Change the endline char to a \0*/
      insertEndLine(cl);
      /*Convert all $$ with PID*/
      stringProcessesReplacer(cl);
      /*Parse the input*/
      commandParser(cl);

      if(!strcmp(cl->primaryCommand,"exit")){
        /*What to do upon exting*/
        exitRoutine(cl);
        break;
      }
      /*Exectute the command appropriately*/
      commandExecutor(cl);
    }
    else{
      printf("\n");
      fflush(stdout);
    }
    /*reset the command line data*/
    resetCommandLine(cl);
  }
}

/*Replaces the endline with an endstring terminator*/
void insertEndLine(struct commandLine *cl){
  int i;
  /*find the charector, replace it*/
  for(i = 0; i < MAX_CHARS; i++){
    if(cl->commandInput[i] == '\n'){
      cl->commandInput[i] = '\0';
      return;
    }
  }
    printf("\n");
    fflush(stdout);
}

/*Parse the command line input into many strings*/
void commandParser(struct commandLine *cl){
  int i;
  char *token = strtok(cl->commandInput," ");

  /*Go while the token is valid or while we havent reached all the args*/
  for(i = 0; i < MAX_ARGS && token; i++){
    /*set the primary command*/
    if(i == 0)
      cl->primaryCommand = token;

    /*File directives*/
    if(!strcmp(token, "<")){
      token = strtok(NULL, " ");
      strcpy(cl->inputFileName, token);
      cl->containsInputFile = 1;
    }
    else if(!strcmp(token, ">")){
      token = strtok(NULL, " ");
      strcpy(cl->outputFileName, token);
      cl->containsOutputFile = 1;
    }
    /*add tot he string*/
    else{
      cl->parsedInput[i] = token;
      cl->parsedSize++;
    }

    token = strtok(NULL, " ");

  }
  /*check for background process*/
  if(!strcmp(cl->parsedInput[cl->parsedSize - 1], "&")){
    cl->containsAmpersand = 1;
    cl->parsedInput[cl->parsedSize - 1] = NULL;
    cl->parsedSize--;
  }
}

/*Execute the commands based on the primary directive*/
void commandExecutor(struct commandLine *cl){
  if(!strcmp(cl->primaryCommand,"#") || !strcmp(cl->primaryCommand,"")){
    return;
  }
  else if(!strcmp(cl->primaryCommand,"exit")){
    exitRoutine(cl);
  }
  else if(!strcmp(cl->primaryCommand,"cd")){
    cdRoutine(cl);
  }
  else if(!strcmp(cl->primaryCommand,"status")){
    statusRoutine(cl);
  }
  else{
    shellRoutine(cl);
  }
}

/*This is the given routine for exiting the program*/
void exitRoutine(struct commandLine *cl){
  int parent_pid = getpid();
  resetCommandLine(cl);
  /*destroy the children*/
  signal(SIGQUIT,SIG_IGN);
  kill(-parent_pid, SIGQUIT);
}

/*The routine used when a cd command is called*/
void cdRoutine(struct commandLine *cl){
  if(cl->parsedSize > 1){
    if(chdir(cl->parsedInput[1]) == -1){
      printf("Invalid Directory.\n");
      fflush(stdout);
    }
  }
  /*if nothing is provided, go to home directory*/
  else
    chdir(getenv("HOME"));
  }

/*status routine, called when status is the primary command*/
void statusRoutine(struct commandLine *cl){
  int exitStatus;
  /*check if exited using exit cmd*/
  if(WIFEXITED(cl->childExitStatus)){
    printf("The process exited\n");
    exitStatus = WEXITSTATUS(cl->childExitStatus);
    printf("exit status was %d\n", exitStatus);
  }
  /*yeah, a signal done did it*/
  else
    printf("terminated by signal 2\n");
}

/*This is used to execute the comand routine!*/
void shellRoutine(struct commandLine *cl){
  pid_t spawnpid = -5;
  pid_t despawnpid = -5;
  int sourceFD;
  int targetFD;
  int sourceResult;
  int targetResult;

  cl->childExitStatus = -5;
  /*Fork the process for command exectution!*/
  spawnpid = fork();
  switch(spawnpid){
    case -1:
      perror("Hull Breach!");
      exit(1);
      break;
    case 0:
      /*Use default action for SIGINT*/
      cl->sigIntHandler.sa_handler = SIG_DFL;
      sigaction(SIGINT, &cl->sigIntHandler, NULL);
      /*Open the input file if it exits*/
      if(cl->containsInputFile == 1){
        sourceFD = open(cl->inputFileName, O_RDONLY);
        if(sourceFD == -1) { perror("source open()"); exit(1); }
        sourceResult = dup2(sourceFD, 0);
        if(sourceResult == -1) { perror("source dup2()"); exit(2); }
        /*close on exec*/
        fcntl(sourceFD,F_SETFD, FD_CLOEXEC);
      }
      /*Open the output file if it exists*/
      if(cl->containsOutputFile == 1){
        targetFD = open(cl->outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(targetFD == -1) { perror("target open()"); exit(1); }
        targetResult = dup2(targetFD, 1);
        if(targetResult == -1) { perror("target dup2()"); exit(2); }
        /*CLose on exectution*/
        fcntl(targetFD, F_SETFD, FD_CLOEXEC);
      }

      /*Exec, return as invalid if there is an error in execution*/
      if(execvp(cl->primaryCommand, cl->parsedInput)){
        printf("Invalid Command\n");
        fflush(stdout);
        exit(1);
      }

      exit(2);
      break;
    default:
      /*PArent process, checkin for validity of background execution*/
      if(cl->containsAmpersand == 1 && foregroundOnly == 0){
        despawnpid = waitpid(spawnpid, &cl->childExitStatus, WNOHANG);
        printf("background pid is %d\n", spawnpid);
        fflush(stdout);
      }
      else{
        /*Indicate why the amperdand request was ignored*/
        if(cl->containsAmpersand == 1){
          printf("Ignoring &, in foregroundOnly\n");
          fflush(stdout);
        }
        /*do not pass in flag WNOHANG into waitpid*/
        /*THis indicates immediate return after foreground process*/
        despawnpid = waitpid(spawnpid, &cl->childExitStatus, 0);
      }

      /*If we are running a background process, check for child process termination*/
      while((spawnpid = waitpid(-1, &cl->childExitStatus, WNOHANG)) > 0){
        printf("child %d terminated\n", spawnpid);
        statusRoutine(cl);
        fflush(stdout);
      }
      break;
  }
}

/*THis is a typset checking module for my struct data*/
/*It just essentially prints a bunch, good for error handling*/
void checkStructStatus(struct commandLine *cl){
  int i;
  printf("commandInput: %s\n", cl->commandInput);
  fflush(stdout);
  printf("inputFileName: %s\n", cl->inputFileName);
  fflush(stdout);
  printf("outputFileName: %s\n", cl->outputFileName);
  fflush(stdout);
  printf("containsAmpersand: %d\n", cl->containsAmpersand);
  fflush(stdout);
  printf("containsInputFile: %d\n", cl->containsInputFile);
  fflush(stdout);
  printf("containsOutputFile: %d\n", cl->containsOutputFile);
  fflush(stdout);
  for(i = 0; i < cl->parsedSize; i++){
    printf("parsedInput %d: %s\n", i+1, cl->parsedInput[i]);
    fflush(stdout);
  }
}

/*Resets the values used in parsin, execution and whatever*/
/*Making it valid for the next comand*/
void resetCommandLine(struct commandLine *cl){
  int i;
  strcpy(cl->commandInput, "");
  strcpy(cl->inputFileName, "");
  strcpy(cl->outputFileName, "");
  cl->containsAmpersand = 0;
  cl->containsInputFile = 0;
  cl->containsOutputFile = 0;
  cl->parsedSize = 0;

  for(i = 0; i < MAX_ARGS; i++)
    cl->parsedInput[i] = NULL;
  cl->primaryCommand = NULL;
}

/*Initialize how we are handling signals in the parent thread, and the handler fincions*/
void initSigStructures(struct commandLine *cl){
  cl->sigIntHandler.sa_handler = SIG_IGN;
  sigfillset(&cl->sigIntHandler.sa_mask);
  cl->sigIntHandler.sa_flags = 0;

  cl->sigStpHandler.sa_handler = catchSIGSTP;
  sigfillset(&cl->sigStpHandler.sa_mask);
  cl->sigStpHandler.sa_flags = 0;

  sigaction(SIGINT, &cl->sigIntHandler, NULL);
  sigaction(SIGTSTP, &cl->sigStpHandler, NULL);

}

/*make sure we have the process ID in string form*/
void initProcessString(struct commandLine *cl){
  pthread_t myThreadID = getpid();
  int pid = (int) myThreadID;
  sprintf(cl->processID, "%d", pid);
}

/*replace all $$ with PID*/
void stringProcessesReplacer(struct commandLine *cl){
  const char *oldPhrase = "$$";
  char result [50];
  int i;
  int pos;
  int count;

  /*Keep doing this till I beak it, and it WILL break*/
  while(1){
    count = 0;
    strcpy(result,"");
    for(i = 0; cl->commandInput[i] != '\0'; i++){
      if(strstr(&cl->commandInput[i], oldPhrase) == &cl->commandInput[i]){
        count++;
        pos = i;
        break;
      }
    }

    if(count == 0)
      return;
    /*do the magic, provided this is the case*/
    strncpy(result, cl->commandInput, pos);
    result[pos] = '\0';
    strcat(result, cl->processID);
    strcat(result, cl->commandInput + (pos + 2));
    strcpy(cl->commandInput,result);
  }
}

/*VAtch the stop signal*/
void catchSIGSTP(int signo){
  if(foregroundOnly == 0){
    foregroundOnly = 1;
    write(1, "\nEntering foreground-only mode (& is now ignored)\n", 50);
    fflush(stdout);
  }
  /*if we are in foregroung only mode*/
  else if(foregroundOnly == 1){
    foregroundOnly = 0;
    write(1, "\nExiting foreground-only mode\n", 30);
    fflush(stdout);
  }
}
