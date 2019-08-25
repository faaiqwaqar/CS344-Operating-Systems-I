/* ADVENTURE PROGRAM */
/*Library Inclusions Used for Multiple Functionalities*/
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

/*Structure typedefs used for object storage*/
typedef struct Room {
  char roomName[10];
  char** data;
  char roomType[10];
  int size;
  int capacity;
} Room;

typedef struct dynArr {
  char** data;
  int size;
  int capacity;
} dynArr;

/*Function Declaration (so they know of one another)*/
void initDynArr(struct dynArr *da, int newCapacity);
void addDynArr(struct dynArr *da, char* value);
void freeDynArr(struct dynArr *da);
void _dynArrDoubleCapacity(struct dynArr *da);
void initRoomStruct(struct Room *rooms);
void freeRoomStruct(struct Room *rooms);
void readFiles(struct Room *rooms);
void resultCheckerPrint(struct Room *rooms);
int primaryMenu(struct Room* rooms, struct Room *currentRoom, struct dynArr *da);
struct Room* transferMenu(struct Room *rooms, char *transferString);
void *writeTime();
void readTime();

/*Initializing memory for a dynamic array structure*/
void initDynArr(struct dynArr *da, int newCapacity){
  /*assert propper capacity*/
  assert(newCapacity >= 0);
  da->capacity = newCapacity;
  da->size = 0;
  da->data = (char**) malloc(sizeof(char*)*newCapacity);
  /*assert allocation*/
  assert(da->data);
}

/*Used to add elements to the dynamic array structure*/
void addDynArr(struct dynArr *da, char* value){
  /*check to see if there is still enough variable space*/
  if(da->size >= da->capacity)
    _dynArrDoubleCapacity(da);
  da->data[da->size] = value;
  da->size++;
}

/*used to deallocate memory of the dynamic array*/
void freeDynArr(struct dynArr *da){
  /*assert that this is an allocated dynamic array*/
  assert(da);
  free(da->data);
  /*reset integer data*/
  da->capacity = 0;
  da->size = 0;
}

/*Used in the case that we have exceeded bounds of the dynamic array*/
void _dynArrDoubleCapacity(struct dynArr *da){
  /*copy old data*/
  char **oldBuffer = da->data;
  int oldSize = da->size;
  int i;
  /*create new memorey*/
  initDynArr(da, 2 * da->capacity);
  /*copy in new data*/
  for(i = 0; i < oldSize; i++)
    da->data[i] = oldBuffer[i];
  da->size = oldSize;
  free(oldBuffer);
}

/*Used to intialize memory for the room stucture*/
void initRoomStruct(struct Room *rooms){
  int i;
  int j;
  /*Initialize all room dynamic memory*/
  for(i = 0; i < 7; i++){
    rooms[i].data = (char **) malloc(sizeof(char *) * 6);
    for(j = 0; j < 6; j++){
      rooms[i].data[j] = (char*) malloc(sizeof(char) * 10);
    }
    rooms[i].capacity = 6;
    rooms[i].size = 0;
  }
}

/*used to deallocate the used memory of the room structure*/
void freeRoomStruct(struct Room *rooms){
  int i;
  int j;
  /*free all dynamic room memory*/
  for(i = 0; i < 7; i++){
    for(j = 0; j < 6; j++)
      free(rooms[i].data[j]);
    free(rooms[i].data);
    rooms[i].capacity = 0;
    rooms[i].size = 0;
  }
}

/*Function to read data into our structre*/
void readFiles(struct Room *rooms){
  /*Directory and File Pointers for access*/
  DIR *directory = NULL;
  FILE *fPtr;
  int newestDirTime = -1;
  /*current andaccess directory name locatoers*/
  char *dirFilename = "./";
  char *newFilename;
  struct stat dirAttributes;
  /*indexers*/
  int i;
  int j;
  int chCheck;
  /*Filenames*/
  char *fileOne = "room1.txt";
  char *fileTwo = "room2.txt";
  char *fileThree = "room3.txt";
  char *fileFour = "room4.txt";
  char *fileFive = "room5.txt";
  char *fileSix = "room6.txt";
  char *fileSeven = "room7.txt";
  /*strings to read to*/
  char room[15];
  char secondary[15];
  char ch;
  int lines;

  /*open the current directory to find the subdirectory*/
  directory = opendir(dirFilename);
  /*error handling*/
  if(directory == NULL)
    printf("ERROR: DIRECTORY\n");
  /*what to do upon opening, look for filename(dir)*/
  else{
    struct dirent *dirp;
    while ((dirp = readdir(directory)) != NULL){
      if(strstr(dirp->d_name, "waqarf.rooms") != NULL){
        stat(dirp->d_name, &dirAttributes);
        if((int) dirAttributes.st_mtime > newestDirTime){
          newestDirTime = (int) dirAttributes.st_mtime;
          newFilename = dirp->d_name;
        }
      }
    }
    closedir(directory);

    /*open the found subdirectory for modification and reading*/
    chCheck = chdir(newFilename);
    /*error handling*/
    if(chCheck != 0)
      printf("ERROR: READING\n");
    /*some testing stuff I need to remove*/
    fPtr = fopen(fileOne, "r");
    if(fPtr == NULL)
      printf("FILEERROR\n");

    fclose(fPtr);
    /*The real magic, this is where we do all the readings*/
   for(i = 0; i < 7; i++){
      switch (i) {
        case 0:
          fPtr = fopen(fileOne, "r");
          break;
        case 1:
          fPtr = fopen(fileTwo, "r");
          break;
        case 2:
          fPtr = fopen(fileThree, "r");
          break;
        case 3:
          fPtr = fopen(fileFour, "r");
          break;
        case 4:
          fPtr = fopen(fileFive, "r");
          break;
        case 5:
          fPtr = fopen(fileSix, "r");
          break;
        case 6:
          fPtr = fopen(fileSeven, "r");
          break;
      }

      if(fPtr == NULL)
        printf("FILEERROR\n");
      /*Deternmine the number of lines*/
      lines = 0;
      while(!feof(fPtr)){
        ch = fgetc(fPtr);
        if(ch == '\n')
          lines++;
      }
      /*reset the pointer*/
      fseek(fPtr, 0, SEEK_SET);

      /*scan, and use the line numbers as an iterator*/
      fscanf(fPtr, "%s %s %s", room, secondary, rooms[i].roomName);
      for(j = 0; j < (lines-2); j++){
        fscanf(fPtr, "%s %s %s", room, secondary, rooms[i].data[rooms[i].size]);
        rooms[i].size++;
      }
      fscanf(fPtr, "%s %s %s", room, secondary, rooms[i].roomType);

      fclose(fPtr);
    }
    chCheck = chdir("..");
    /*error handling*/
    if(chCheck != 0)
      printf("ERROR: EXITING\n");
  }
}
/*This is just a verifying module to be used in testing*/
void resultCheckerPrint(struct Room *rooms){
  int i;
  int j;
  /*print data of each structure*/
  for(i = 0; i < 7; i++){
    printf("\nROOM %d NAME: %s \n", i+1 ,rooms[i].roomName);
    printf("ROOM %d TYPE: %s \n", i+1, rooms[i].roomType);
    for(j = 0; j < rooms[i].size; j++){
      printf("CONNECTION %d: %s\n", j+1, rooms[i].data[j]);
    }
  }
}

/*All menu functions are run here, plays the game*/
int primaryMenu(struct Room *rooms, struct Room *currentRoom, struct dynArr *da){
  int i;
  char buffer[10];
  int flag = 0;
  int endGame = 0;
  int steps = 0;
  /*Createign new thread and mutex lock to be used*/
  pthread_t time_thread;
  pthread_mutex_t time_mutex;
  /*Initialize the thread*/
  pthread_mutex_init(&time_mutex,NULL);
  pthread_mutex_lock(&time_mutex);
  pthread_create(&time_thread,NULL,writeTime,NULL);
  pthread_mutex_unlock(&time_mutex);
  /*Loops till game completion*/
  while(endGame == 0){
    flag = 0;
    while(flag == 0){
      printf("\nCURRENT LOCATION: %s\n", currentRoom->roomName);
      printf("POSSIBLE CONNECTIONS: ");
      for(i = 0; i < currentRoom->size; i++){
        printf("%s", currentRoom->data[i]);
        if(i != currentRoom->size - 1)
          printf(", ");
        else
          printf(".");
      }

      printf("\nWHERE TO? >");
      scanf("%s", buffer);
      /*case on locking with mutex and using pthread*/
      if(strcmp(buffer,"time") == 0){
        pthread_mutex_unlock(&time_mutex);
        pthread_join(time_thread,NULL);
        pthread_mutex_lock(&time_mutex);
        pthread_create(&time_thread,NULL,writeTime,NULL);
        pthread_mutex_unlock(&time_mutex);
        readTime();
        flag = 1;
      }
      /*check for file and begin changing the current pointer*/
      else{
        for(i = 0; i < currentRoom->size; i++){
          if(strcmp(buffer,currentRoom->data[i]) == 0){
            flag = 1;
            currentRoom = transferMenu(rooms, buffer);
            steps++;
            addDynArr(da,currentRoom->roomName);
            if(strcmp(currentRoom->roomType,"END_ROOM") == 0){
              return steps;
              break;
            }
          }
        }
      }
      if(flag == 0)
        printf("\nHUH? I DONT UNDERSTAND THAT ROOM, TRY AGAIN.\n");
    }

  }
  return steps;
}
/*Function used to change where the curent pointer looks at for data*/
struct Room* transferMenu(struct Room *rooms, char *transferString){
  int i;
  for(i = 0; i < 7; i++){
    if(strcmp(transferString,rooms[i].roomName) == 0){
      return &rooms[i];
    }
  }
  return &rooms[0];
}

/*file used to write tyme, called on protocal for thread*/
void *writeTime(){
  time_t computerTime;
  struct tm *stats;
  char buffer[50];
  /*set file pointer*/
  FILE *fPtr = fopen("currentTime.txt","w");
  if(fPtr == NULL)
    printf("TIMEFILE WRITE ERROR\n");
    /*enter propper time*/
  time(&computerTime);
  stats = localtime(&computerTime);
  strftime(buffer,50,"%l:%M%P, %A, %B %d, 2019",stats);
  fputs(buffer,fPtr);

  fclose(fPtr);
  return NULL;
}
/*function used to read time*/
void readTime(){
  FILE *fPtr;
  char ch;
  fPtr = fopen("currentTime.txt","r");
  if(fPtr == NULL)
    printf("TIMEFILE READ ERROR\n");

  /*print to screen from read*/
  printf("\n");
  while((ch = fgetc(fPtr)) != EOF)
    printf("%c",ch);
  printf("\n");

  fclose(fPtr);

}

/*Main, calls to functions and deallocation*/
int main(){
  int steps;
  int i;
  struct dynArr da;
  struct Room *rooms = (struct Room *) malloc(sizeof(struct Room) * 7);
  struct Room *currentRoom = &rooms[0];
  assert(rooms);
  /*structure intiialization calls*/
  initDynArr(&da,20);
  initRoomStruct(rooms);

  /*read and play*/
  readFiles(rooms);
  steps = primaryMenu(rooms,currentRoom, &da);

  /*Endgame, step printout and path*/
  printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
  printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", steps);
  for(i = 0; i < da.size; i++){
    printf("%s\n",da.data[i]);
  }

  /*Free allocated memory*/
  freeRoomStruct(rooms);
  freeDynArr(&da);
  free(rooms);
  return 0;
}
