/*otp_enc_d.c*/
#include "universal.h"
/*encrypt data, takes the key string and the string and converys it into cryptic*/
void encryptData(struct otpEnc *oe){
  /*initialize local variables used in local computation*/
  int i;
  int localBuffer1;
  int localBuffer2;
  int localBuffer3;
  /*set the length of the buffers so that we can iterate over them*/
  oe->fileBufferLength = strlen(oe->fileBuffer);
  oe->keyBufferLength = strlen(oe->keyBuffer);
  /*utility key value string used in cypher text*/
  strcpy(oe->keyValue, "ABCDEFGHIJKLMNOPQRSTUVWXYZ ");
  /*convert the alphabetc data to a location in the text value*/
  for(i = 0; i < oe->fileBufferLength; i++){
    switch(oe->fileBuffer[i]){
      case 'A':
        localBuffer1 = 0;
        break;
      case 'B':
        localBuffer1 = 1;
        break;
      case 'C':
        localBuffer1 = 2;
        break;
      case 'D':
        localBuffer1 = 3;
        break;
      case 'E':
        localBuffer1 = 4;
        break;
      case 'F':
        localBuffer1 = 5;
        break;
      case 'G':
        localBuffer1 = 6;
        break;
      case 'H':
        localBuffer1 = 7;
        break;
      case 'I':
        localBuffer1 = 8;
        break;
      case 'J':
        localBuffer1 = 9;
        break;
      case 'K':
        localBuffer1 = 10;
        break;
      case 'L':
        localBuffer1 = 11;
        break;
      case 'M':
        localBuffer1 = 12;
        break;
      case 'N':
        localBuffer1 = 13;
        break;
      case 'O':
        localBuffer1 = 14;
        break;
      case 'P':
        localBuffer1 = 15;
        break;
      case 'Q':
        localBuffer1 = 16;
        break;
      case 'R':
        localBuffer1 = 17;
        break;
      case 'S':
        localBuffer1 = 18;
        break;
      case 'T':
        localBuffer1 = 19;
        break;
      case 'U':
        localBuffer1 = 20;
        break;
      case 'V':
        localBuffer1 = 21;
        break;
      case 'W':
        localBuffer1 = 22;
        break;
      case 'X':
        localBuffer1 = 23;
        break;
      case 'Y':
        localBuffer1 = 24;
        break;
      case 'Z':
        localBuffer1 = 25;
        break;
      case ' ':
        localBuffer1 = 26;
        break;
    }
    /*DO the same conversion for the key to used for math later on*/
    switch(oe->keyBuffer[i]){
      case 'A':
        localBuffer2 = 0;
        break;
      case 'B':
        localBuffer2 = 1;
        break;
      case 'C':
        localBuffer2 = 2;
        break;
      case 'D':
        localBuffer2 = 3;
        break;
      case 'E':
        localBuffer2 = 4;
        break;
      case 'F':
        localBuffer2 = 5;
        break;
      case 'G':
        localBuffer2 = 6;
        break;
      case 'H':
        localBuffer2 = 7;
        break;
      case 'I':
        localBuffer2 = 8;
        break;
      case 'J':
        localBuffer2 = 9;
        break;
      case 'K':
        localBuffer2 = 10;
        break;
      case 'L':
        localBuffer2 = 11;
        break;
      case 'M':
        localBuffer2 = 12;
        break;
      case 'N':
        localBuffer2 = 13;
        break;
      case 'O':
        localBuffer2 = 14;
        break;
      case 'P':
        localBuffer2 = 15;
        break;
      case 'Q':
        localBuffer2 = 16;
        break;
      case 'R':
        localBuffer2 = 17;
        break;
      case 'S':
        localBuffer2 = 18;
        break;
      case 'T':
        localBuffer2 = 19;
        break;
      case 'U':
        localBuffer2 = 20;
        break;
      case 'V':
        localBuffer2 = 21;
        break;
      case 'W':
        localBuffer2 = 22;
        break;
      case 'X':
        localBuffer2 = 23;
        break;
      case 'Y':
        localBuffer2 = 24;
        break;
      case 'Z':
        localBuffer2 = 25;
        break;
      case ' ':
        localBuffer2 = 26;
        break;
    }
    /*compute an index of the cryptic message using mod and place into the file buffer*/
    localBuffer3 = (localBuffer1 + localBuffer2) % 27;
    oe->fileBuffer[i] = oe->keyValue[localBuffer3];
  }

}

/*Used to initialize the address structure and the socker credentials*/
void initOtpEncStruct(struct otpEnc *oe){
  initSockAddrStruct(oe);
  initSocketCred(oe);
}

/*Set up the socket address struct information*/
void initSockAddrStruct(struct otpEnc *oe){
  /*set memory to a cleared out struct*/
  memset((char *)&oe->serverAddress, '\0', sizeof(oe->serverAddress));
  /*set the accessability of the stuct and the portnumber*/
  oe->serverAddress.sin_family = AF_INET;
  oe->serverAddress.sin_port = htons(oe->portNumber);
  oe->serverAddress.sin_addr.s_addr = INADDR_ANY;
}

/*INitializre the socket credentials*/
void initSocketCred(struct otpEnc *oe){
  /*set the socket listening properties*/
  oe->listenSocketFD = socket(AF_INET, SOCK_STREAM, 0);
  if(oe->listenSocketFD < 0){
    perror("ERROR opening socket");
    exit(1);
  }
}

/*THis function contains the loop with all the functionalities carried by the daemon*/
void runSocketProcess(struct otpEnc *oe){
  /*use the bind function to bind to the serveradderessm and error hande it*/
  if (bind(oe->listenSocketFD, (struct sockaddr *)&oe->serverAddress, sizeof(oe->serverAddress)) < 0){
    perror("ERROR on binding");
    exit(1);
  }
  /*listen for up to 5 connec!*/
  listen(oe->listenSocketFD, 5);

  /*loop waiting for a connection*/
  while(1){
    oe->sizeOfClientInfo = sizeof(oe->clientAddress);
    oe->establishedConnectionFD = accept(oe->listenSocketFD, (struct sockaddr *)&oe->clientAddress, &oe->sizeOfClientInfo);
    if (oe->establishedConnectionFD < 0){
      perror("ERROR on accept");
      exit(1);
    }

    /*fork the process to allow for multiprocessing and multiplee connections*/
    oe->pid = fork();
    switch(oe->pid){
      /*error in forking: hull breach*/
      case -1:
        perror("Hull Breach");
        exit(1);
        break;
      /*child case, where all the work pretty much happens*/
      case 0:
        /*recieve intiale communicated filebuffer*/
        memset(oe->buffer, '\0', 256);
        oe->charsRead = recv(oe->establishedConnectionFD, oe->fileBuffer, MAX_BUFFER-1, 0);
        if (oe->charsRead < 0){
          perror("ERROR reading from socket");
          exit(1);
        }
        /*signal back to break recv*/
        oe->charsRead = send(oe->establishedConnectionFD, "e", 39, 0);
        if (oe->charsRead < 0){
          perror("ERROR writing to socket");
          exit(1);
        }
        /*read the key values from the client*/
        oe->charsRead = recv(oe->establishedConnectionFD, oe->keyBuffer, MAX_BUFFER-1, 0);
        if (oe->charsRead < 0){
          perror("ERROR reading from socket");
          exit(1);
        }
        /*encrypt and send the data back*/
        encryptData(oe);
        oe->charsRead = send(oe->establishedConnectionFD, oe->fileBuffer, 39, 0);
        if (oe->charsRead < 0){
          perror("ERROR writing to socket");
          exit(1);
        }

        break;
      default:
        /*stop the socket and wait on the child case process*/
        stopSocketCred(oe);
        waitpid(oe->pid, &oe->childExitStatus, 0);
        break;
    }
  }
}

/*this is a function used to close the file descriptors and socket connections*/
void stopSocketCred(struct otpEnc *oe){
  close(oe->establishedConnectionFD);
  close(oe->listenSocketFD);
}

/*main, used to set up and call to connections*/
int main(int argc, char* argv[]){
  /*set up the port*/
  if(argc < 2){
    fprintf(stderr,"USAGE: %s port\n", argv[0]);
    exit(1);
  }
  /*create the struct needed for all variables*/
  struct otpEnc otpenc;
  assert(argv[1]);
  otpenc.portNumber = atoi(argv[1]);
  /*function calling*/
  initOtpEncStruct(&otpenc);
  runSocketProcess(&otpenc);
  stopSocketCred(&otpenc);
  return 0;
}
