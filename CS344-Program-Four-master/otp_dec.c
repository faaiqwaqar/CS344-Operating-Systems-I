/*otp_dec.c*/
#include "universal.h"

/*translate data file, that takes the contents of the files and then converts them to strings*/
void translateData(struct otpEnc *oe){
  FILE* kPtr = fopen(oe->keyInfo, "r");
  FILE* fPtr = fopen(oe->fileInfo, "r");
  /*get the necessary transtion data*/
  fgets(oe->keyBuffer, MAX_CHARS-1, kPtr);
  fgets(oe->fileBuffer, MAX_CHARS-1, fPtr);
  oe->keyBuffer[strcspn(oe->keyBuffer, "\n")] = '\0';
  oe->fileBuffer[strcspn(oe->fileBuffer, "\n")] = '\0';
  /*set lengths to prepare for iteration*/
  oe->keyBufferLength = strlen(oe->keyBuffer);
  oe->fileBufferLength = strlen(oe->fileBuffer);
  fclose(kPtr);
  fclose(fPtr);
}

/*initialize the struct and the socket credentials vie function calls*/
void initOtpEncStruct(struct otpEnc *oe){
  initSockAddrStruct(oe);
  initSocketCred(oe);
}

/*initiale the socket address port to be attending to a port number */
void initSockAddrStruct(struct otpEnc *oe){
  /*clear out memory structure contents*/
  memset((char *)&oe->serverAddress, '\0', sizeof(oe->serverAddress));
  oe->serverAddress.sin_family = AF_INET;
  oe->serverAddress.sin_port = htons(oe->portNumber);
  oe->serverHostInfo = gethostbyname(oe->hostInfo);
  if(oe->serverHostInfo == NULL){
    perror("ERROR host info");
    exit(0);
  }
  /*set server and host addr*/
  memcpy((char *)&oe->serverAddress.sin_addr.s_addr, (char *)oe->serverHostInfo->h_addr, oe->serverHostInfo->h_length);
}

/*initiale rhe socket credentials and open the socket*/
void initSocketCred(struct otpEnc *oe){
  oe->socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if(oe->socketFD < 0){
    perror("CLIENT: ERROR opening socket");
    exit(0);
  }
}

/*runs socket processes on the client side*/
void runSocketProcess(struct otpEnc *oe){
  if (connect(oe->socketFD, (struct sockaddr*)&oe->serverAddress, sizeof(oe->serverAddress)) < 0){
    perror("CLIENT: ERROR connecting");
    exit(0);
  }
  /*send file bufer data*/
  oe->charsWritten = send(oe->socketFD, oe->fileBuffer, strlen(oe->fileBuffer), 0);
  if (oe->charsWritten < 0){
    perror("CLIENT: ERROR writing to socket");
    exit(0);
  }

  if(oe->charsWritten < strlen(oe->fileBuffer)){
    printf("CLIENT: WARNING: Not all data written to socket!\n");
    fflush(stdout);
  }
  /*recieve some data*/
  memset(oe->buffer, '\0', MAX_BUFFER);
  oe->charsRead = recv(oe->socketFD, oe->buffer, MAX_BUFFER-1, 0);
  if (oe->charsRead < 0){
    perror("CLIENT: ERROR Reading from socket");
    exit(0);
  }

  if (!strcmp(oe->buffer,"e")){
    perror("CLIENT: ERROR cant connect to opposite daemon");
    exit(1);
  }
  /*and again...this time the key buffer*/
  oe->charsWritten = send(oe->socketFD, oe->keyBuffer, strlen(oe->keyBuffer), 0);
  if (oe->charsWritten < 0){
    perror("CLIENT: ERROR writing to socket");
    exit(0);
  }

  if(oe->charsWritten < strlen(oe->keyBuffer)){
    printf("CLIENT: WARNING: Not all data written to socket!\n");
    fflush(stdout);
  }
  /*recieve the encrypted buffer*/
  memset(oe->buffer, '\0', MAX_BUFFER);
  oe->charsRead = recv(oe->socketFD, oe->buffer, MAX_BUFFER-1, 0);
  if (oe->charsRead < 0){
    perror("CLIENT: ERROR Reading from socket");
    exit(0);
  }
  printf("%s\n", oe->buffer);
  fflush(stdout);

}

/*close the listening sockets*/
void stopSocketCred(struct otpEnc *oe){
  close(oe->socketFD);
}

int main(int argc, char* argv[]){
  /*error checl the command line args*/
  if(argc < 4){
    fprintf(stderr,"USAGE: %s port\n", argv[0]);
    exit(1);
  }
  struct otpEnc otpenc;
  otpenc.hostInfo = "localhost";
  assert(argv[1]);
  otpenc.fileInfo = argv[1];
  assert(argv[2]);
  otpenc.keyInfo = argv[2];
  assert(argv[3]);
  otpenc.portNumber = atoi(argv[3]);
  /*function calls*/
  translateData(&otpenc);
  initOtpEncStruct(&otpenc);
  runSocketProcess(&otpenc);
  stopSocketCred(&otpenc);
  return 0;
}
