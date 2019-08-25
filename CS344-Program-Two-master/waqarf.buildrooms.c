/* BUILDROOMS PROGRAM */
/*library calls for function inclusions*/
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

/*structure typedefs for data storage containers*/
typedef struct Room {
  char* roomName;
  struct Room** data;
  char* roomType;
  int size;
  int capacity;
} Room;

typedef struct dynArr {
  int* data;
  int size;
  int capacity;
} dynArr;

/*Function declarations to lets each other know they exist*/
void initDynArr(struct dynArr *da, int newCapacity);
void addDynArr(struct dynArr *da, int value);
void freeDynArr(struct dynArr *da);
void _dynArrDoubleCapacity(struct dynArr *da);
int processIdRetriever();
char* directoryCreator(int processID);
void connectionReciever(struct Room *rooms);
void addConnection(struct Room *firstRoom, struct Room *secondRoom);
int usedName(struct dynArr *nameIndexer,int value);
void createRoomNames(struct Room *roomID, struct dynArr *nameIndexer);
void createRoomTypes(struct Room *roomID, struct dynArr *typeIndexer);
void initializeRooms(struct Room *roomID, struct dynArr *nameIndexer, struct dynArr *typeIndexer);
void freeRooms(struct Room *roomID);
void resultCheckerPrint(struct Room *rooms);
void writeData(struct Room *rooms);

/*initialization of memory for the dynamic array struct*/
void initDynArr(struct dynArr *da, int newCapacity){
  /*assert a non-neg capacity before passign to malloc*/
  assert(newCapacity >= 0);
  /*Initialize other portions of the data*/
  da->capacity = newCapacity;
  da->size = 0;
  da->data = (int*) malloc(sizeof(int*)*newCapacity);
  /*assert the data has been allocated*/
  assert(da->data);
}

/*function to add new data to the dynamic array, storing ints*/
void addDynArr(struct dynArr *da, int value){
  /*call double capacity if reaching the limit*/
  if(da->size >= da->capacity)
    _dynArrDoubleCapacity(da);
  /*set the new variable into memory and increment te size*/
  da->data[da->size] = value;
  da->size++;
}

/*function used to free up data used by the dynamic array*/
void freeDynArr(struct dynArr *da){
  /*assert an alloced stuct has been passed through*/
  assert(da);
  free(da->data);
  /*reset the integer data*/
  da->capacity = 0;
  da->size = 0;
}

/*dynamic arrray capacity multiplyer incase of storage overload*/
void _dynArrDoubleCapacity(struct dynArr *da){
  /*copy old memory*/
  int *oldBuffer = da->data;
  int oldSize = da->size;
  int i;
  /*reinitialize with twice the memory*/
  initDynArr(da, 2 * da->capacity);
  /*copy in old data*/
  for(i = 0; i < oldSize; i++)
    da->data[i] = oldBuffer[i];
  da->size = oldSize;
  /*free the old data*/
  free(oldBuffer);
}

/*Function used to look at local thread and return integer*/
int processIdRetriever(){
  pthread_t myThreadID = getpid();
  return (int) myThreadID;
}

/*creates the new directory for the use by user*/
char* directoryCreator(int processID){
  char dirEntry [30] = "waqarf.rooms.";
  char buffer [15];
  int directoryTrial;
  /*concatenate process ID*/
  sprintf(buffer,"%d",processID);
  strcat (dirEntry, buffer);
  /*create directory*/
  directoryTrial = mkdir(dirEntry, 0777);
  return dirEntry;
}

/*Function used to add connections between 2 passed in rooms for Roomstruct*/
void addConnection(struct Room *firstRoom, struct Room *secondRoom){
  int i;
  int firstSize = firstRoom->size;
  /*check if the connection exists already*/
  for(i = 0; i < firstSize; i++){
    if(firstRoom->data[i] == secondRoom)
      return;
  }
  /*build connections on both sides*/
  firstRoom->data[firstRoom->size] = secondRoom;
  firstRoom->size++;
  secondRoom->data[secondRoom->size] = firstRoom;
  secondRoom->size++;
}

/*function used to pass in values to build connections, and verify completion*/
void connectionReciever(struct Room *rooms){
  int upper = 7;
  int completionFlag = 1;
  int randVar1;
  int randVar2;
  int i;
  srand(time(NULL));
  /*create random variables*/
  while(completionFlag == 1){
    randVar1 = (rand() % upper);
    randVar2 = (rand() % upper);

    /*Check equavalence and dey if routing to the same node*/
    if(randVar1 != randVar2){
      addConnection(&rooms[randVar1],&rooms[randVar2]);
    }

    /*Iterate through and check that all have atleast 3 connections*/
    for(i = 0; i < 7; i++){
      if(rooms[i].size < 3){
        completionFlag = 1;
        break;
      }
      else
        completionFlag = 0;
    }
  }
}

/*Checks in a dynamic array container is a value exists*/
int usedName(struct dynArr *nameIndexer, int value){
  int i;
  int maxSize = nameIndexer->size;
  int flag = 0;
  for(i = 0; i < maxSize; i++){
    if(nameIndexer->data[i] == value)
      flag = 1;
  }
  return flag;
}

/*This is used to assign hard coded room names associated with given structures randomly*/
void createRoomNames(struct Room *roomID, struct dynArr *nameIndexer){
  int upper = 10;
  int lower = 1;
  int flag = 1;
  int randVar;
  srand(time(NULL));
  /*create random variables*/
  while (flag == 1){
    randVar = (rand() % (upper - lower + 1)) + 1;
    flag = usedName(nameIndexer, randVar);
  }
  addDynArr(nameIndexer,randVar);
  /*hardcode the names*/
  switch (randVar) {
    case 1:
      roomID->roomName = "fortress";
      break;
    case 2:
      roomID->roomName = "dungeon";
      break;
    case 3:
      roomID->roomName = "ravine";
      break;
    case 4:
      roomID->roomName = "cave";
      break;
    case 5:
      roomID->roomName = "strghold";
      break;
    case 6:
      roomID->roomName = "village";
      break;
    case 7:
      roomID->roomName = "nether";
      break;
    case 8:
      roomID->roomName = "desert";
      break;
    case 9:
      roomID->roomName = "lake";
      break;
    case 10:
      roomID->roomName = "ender";
      break;
  }
}

/*This is used to assign room types tot he respective structures*/
void createRoomTypes(struct Room *roomID, struct dynArr *typeIndexer){
  int flag = 1;
  int begend = 1;
  /*check what rooms have/have not been created*/
  while (flag == 1){
    if(begend == 1 || begend == 2){
      flag = usedName(typeIndexer, begend);
      if (flag == 1)
        begend++;
    }
    if(begend == 3)
      flag = 0;
  }
  addDynArr(typeIndexer,begend);
  /*hardcode the room types*/
  switch (begend) {
    case 1:
      roomID->roomType = "START_ROOM";
      break;
    case 2:
      roomID->roomType = "END_ROOM";
      break;
    case 3:
      roomID->roomType = "MID_ROOM";
      break;
  }
}

/*Used to initalize data to the room struct*/
void initializeRooms(struct Room *roomID, struct dynArr *nameIndexer, struct dynArr *typeIndexer){
  roomID->size = 0;
  roomID->capacity = 6;
  roomID->data = (struct Room**) malloc(sizeof(struct Room*)*6);
  createRoomNames(roomID,nameIndexer);
  createRoomTypes(roomID,typeIndexer);
}

/*free the room structure allocated data*/
void freeRooms(struct Room *roomID){
  roomID->size = 0;
  roomID->capacity = 0;
  free(roomID->data);
}

/*testing function not used in final runtime, prints out the structures to console*/
void resultCheckerPrint(struct Room *rooms){
  int i;
  int j;
  for(i = 0; i < 7; i++){
    printf("\nROOM %d NAME: %s \n", i+1 ,rooms[i].roomName);
    printf("ROOM %d TYPE: %s \n", i+1, rooms[i].roomType);
    for(j = 0; j < rooms[i].size; j++){
      printf("CONNECTION %d: %s\n", j+1, rooms[i].data[j]->roomName);
    }
  }
}

/*Write out the data to the files once out directory has been created*/
void writeData(struct Room *rooms){
  /*LOTS OF VARIABLES!*/
  DIR *directory = NULL;
  int newestDirTime = -1;
  struct stat dirAttributes;
  char *dirFilename = "./";
  char *newFilename;
  FILE *fPtr;
  int chCheck;
  int i;
  int j;
  char *fileOne = "room1.txt";
  char *fileTwo = "room2.txt";
  char *fileThree = "room3.txt";
  char *fileFour = "room4.txt";
  char *fileFive = "room5.txt";
  char *fileSix = "room6.txt";
  char *fileSeven = "room7.txt";
  char buffer[10];
  /*open the current directory*/
  directory = opendir(dirFilename);
  /*check for the wanted specifier*/
  if(directory == NULL)
    printf("ERROR: DIRECTORY\n");
  else{
    struct dirent *dirp;
    while ((dirp = readdir(directory)) != NULL){
      if(strstr(dirp->d_name, "waqarf.rooms") != NULL){
        stat(dirp->d_name, &dirAttributes);
        if((int) dirAttributes.st_mtime > newestDirTime){
          newFilename = dirp->d_name;
          newestDirTime = (int) dirAttributes.st_mtime;
        }
      }
    }
    closedir(directory);
    chCheck = chdir(newFilename);
    if(chCheck != 0)
      printf("ERROR: WRITING\n");
    /*open every file needed and input the struct data*/
    for(i = 0; i < 7; i++){
      switch (i) {
        case 0:
          fPtr = fopen(fileOne, "w");
          break;
        case 1:
          fPtr = fopen(fileTwo, "w");
          break;
        case 2:
          fPtr = fopen(fileThree, "w");
          break;
        case 3:
          fPtr = fopen(fileFour, "w");
          break;
        case 4:
          fPtr = fopen(fileFive, "w");
          break;
        case 5:
          fPtr = fopen(fileSix, "w");
          break;
        case 6:
          fPtr = fopen(fileSeven, "w");
          break;
      }
      if(fPtr == NULL)
        printf("FILEERROR\n");
      fputs("ROOM NAME: ", fPtr);
      fputs(rooms[i].roomName, fPtr);
      fputs("\n", fPtr);
      for(j = 0; j < rooms[i].size; j++){
        fputs("CONNECTION ", fPtr);
        sprintf(buffer, "%d", j+1);
        fputs(buffer, fPtr);
        fputs(": ", fPtr);
        fputs(rooms[i].data[j]->roomName, fPtr);
        fputs("\n", fPtr);
      }
      fputs("ROOM TYPE: ", fPtr);
      fputs(rooms[i].roomType, fPtr);
      fputs("\n", fPtr);
      fclose(fPtr);
    }

  }
}

/*Main, function calls and more, you know the drill*/
int main(){
  int i;
  int processID;
  struct Room *rooms = (struct Room*) malloc(sizeof(struct Room)*7);
  struct dynArr nameIndexer;
  struct dynArr typeIndexer;
  char *directory;
  initDynArr(&nameIndexer,10);
  initDynArr(&typeIndexer,10);
  assert(rooms);

  /*get the process ID and create the directory*/
  processID = processIdRetriever();
  directory = directoryCreator(processID);

  for(i = 0; i < 7; i++){
    initializeRooms(&rooms[i],&nameIndexer,&typeIndexer);
  }
  /*build connections and write the data*/
  connectionReciever(rooms);
  writeData(rooms);
/*  resultCheckerPrint(rooms); */
/* free the allocated data*/
  freeDynArr(&nameIndexer);
  freeDynArr(&typeIndexer);
  for(i = 0; i < 7; i++){
    freeRooms(&rooms[i]);
  }
  free(rooms);
  return 0;
}
