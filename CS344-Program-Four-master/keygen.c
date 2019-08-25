/*keygen.c*/
/*I learned a lot of this from a stackoverflow article, but the code is all my own*/
#include "universal.h"

/*this is used to allocate data dynamically and prepare for key generation*/
void initKeyGenStruct(struct keyGen *kg){
  strcpy(kg->keyValue, "ABCDEFGHIJKLMNOPQRSTUVWXYZ ");
  /*mallos call to set the amount of bits from command line arg*/
  kg->keyInterp = (char*) malloc(sizeof(char)*(kg->keyLength + 1));
  assert(kg->keyInterp);
  /*set terminator value*/
  kg->keyInterp[kg->keyLength] = '\0';
  keyGeneration(kg);
}

/*free dynamic data to prevent memory leaks*/
void freeKeyGenStruct(struct keyGen *kg){
  free(kg->keyInterp);
  kg->keyLength = 0;
}

/*create a random key and print out to commandline*/
void keyGeneration(struct keyGen *kg){
  int i;
  int key;
  srand(time(NULL));
  for(i = 0; i < kg->keyLength; i++){
    key = rand() % 27;
    kg->keyInterp[i] = kg->keyValue[key];
  }
  printf("%s\n", kg->keyInterp);
  fflush(stdout);
}

/*main with function calls*/
int main(int argc, char* argv[]){
  struct keyGen keygen;
  /*assert that there must be a second command*/
  assert(argv[1]);

  keygen.keyLength = atoi(argv[1]);
  /*call the functions*/
  initKeyGenStruct(&keygen);
  freeKeyGenStruct(&keygen);

  return 0;
}
