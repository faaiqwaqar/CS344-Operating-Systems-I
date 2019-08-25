/* main program */
#include "smallsh.h"

int main(){
  /*Create a data struct*/
  struct commandLine command_line;
  /*Initialize dtaa!!!*/
  resetCommandLine(&command_line);
  initSigStructures(&command_line);
  initProcessString(&command_line);
  /*Move into the command prompt*/
  commandPrompt(&command_line);
  return 0;
}
