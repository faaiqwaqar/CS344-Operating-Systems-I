/* universal.h */
#ifndef WAQARF_UNIVERSAL_H
#define WAQARF_UNIVERSAL_H
/*LOTS of libreary inclusions, I pretty much included everything from every project*/
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/*add defintions for substitutions, the h_Addr one was important to correct an old standard*/
#define MAX_CHARS 2048
#define MAX_BUFFER 256
#define APH_CHARS 27
#define h_addr h_addr_list[0]

/*otpDec struct used as a container for all decryption elements*/
typedef struct otpDec{
  struct sockaddr_in serverAddress;
  struct sockaddr_in clientAddress;
  struct hostent* serverHostInfo;
  int listenSocketFD;
  int socketFD;
  int establishedConnectionFD;
  int portNumber;
  int charsRead;
  int charsWritten;
  int childExitStatus;
  int keyBufferLength;
  int fileBufferLength;
  socklen_t sizeOfClientInfo;
  char buffer[MAX_BUFFER];
  char keyBuffer[MAX_CHARS];
  char fileBuffer[MAX_CHARS];
  char encodedBuffer[MAX_CHARS];
  char keyValue[APH_CHARS];
  char *hostInfo;
  char *keyInfo;
  char *fileInfo;
  pid_t pid;
  pid_t despawnpid;
} otpDec;

/*otpEnc struct used as a container for all encryption elements*/
typedef struct otpEnc{
  struct sockaddr_in serverAddress;
  struct sockaddr_in clientAddress;
  struct hostent* serverHostInfo;
  int listenSocketFD;
  int socketFD;
  int establishedConnectionFD;
  int portNumber;
  int charsRead;
  int charsWritten;
  int childExitStatus;
  int keyBufferLength;
  int fileBufferLength;
  socklen_t sizeOfClientInfo;
  char buffer[MAX_BUFFER];
  char keyBuffer[MAX_CHARS];
  char fileBuffer[MAX_CHARS];
  char encodedBuffer[MAX_CHARS];
  char keyValue[APH_CHARS];
  char *hostInfo;
  char *keyInfo;
  char *fileInfo;
  pid_t pid;
  pid_t despawnpid;
} otpEnc;

/*keygen struct used for key generation*/
typedef struct keyGen{
  int keyLength;
  char keyValue[APH_CHARS];
  char *keyInterp;
} keyGen;

/*function prototyping to be used in non header implemeentations*/
void initOtpDecStruct(struct otpDec *od);
void decryptData(struct otpEnc *oe);

void initOtpEncStruct(struct otpEnc *oe);
void initSockAddrStruct(struct otpEnc *oe);
void initSocketCred(struct otpEnc *oe);
void stopSocketCred(struct otpEnc *oe);
void runSocketProcess(struct otpEnc *oe);
void encryptData(struct otpEnc *oe);
void translateData(struct otpEnc *oe);
void dataValidator(struct otpEnc *oe);

void initKeyGenStruct(struct keyGen *kg);
void freeKeyGenStruct(struct keyGen *kg);
void keyGeneration(struct keyGen *kg);

#endif
