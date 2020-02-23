#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#define BUF_SIZE 4096

void * threadedInput(void * socket)
{
    int sock = *((int *) socket);
    char *inputBuffer;
    int inputBufferSize = 120;

    inputBuffer = (char *)malloc(inputBufferSize * sizeof(char));

    while(1){ 
      if (getline(&inputBuffer, (size_t*) &inputBufferSize, stdin) != 0) {
            //printf("%s\n", inputBuffer);
            if(write(sock, inputBuffer, strlen(inputBuffer)) < 0) perror("send");
        }   
    }
    free(inputBuffer); 
    return NULL;
}

int main(int argc, char* argv[]){
  if (argc != 4){
    printf("%s\n", "please pass appropriate arguments... exiting");
    exit(1);
  }

  char * hostname = argv[1];    //the hostname we are looking up
  short port = atoi(argv[2]);                   //the port we are connecting on

  struct addrinfo *result;       //to store results
  struct addrinfo hints;         //to indicate information we want

  struct sockaddr_in *saddr_in;  //socket interent address

  int s, n;                       //for error checking

  int client_socket;                      //socket file descriptor

  pthread_t client_thread;

  char* client_name = argv[3];

  char response[BUF_SIZE];           //read in 4096 byte chunks

  //setup our hints
  memset(&hints, 0, sizeof(struct addrinfo));  //zero out hints
  hints.ai_family = AF_INET; //we only want IPv4 addresses

  //Convert the hostname to an address
  if( (s = getaddrinfo(hostname, NULL, &hints, &result)) != 0){
      fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(s));
      exit(1);
  }

  //convert generic socket address to inet socket address
  saddr_in = (struct sockaddr_in *) result->ai_addr;

  //set the port in network byte order
  saddr_in->sin_port = htons(port);

  //open a socket
  if( (client_socket = socket(AF_INET, SOCK_STREAM, 0))  < 0){
      perror("socket");
      exit(1);
  }

  //connect to the server
  if( connect(client_socket, (struct sockaddr *) saddr_in, sizeof(*saddr_in)) < 0 ) {
      perror("connect");
      exit(1);
  }

  //send the client_name
  if( write(client_socket, client_name, strlen(client_name)) < 0 ){
      perror("send");
  }

  //read the response until EOF
  pthread_create(&client_thread, NULL, threadedInput, &client_socket);

  while (1)
    {
        memset(response, 0, BUF_SIZE);
        n = read(client_socket, response, BUF_SIZE-1);   
        if(n <= 0){ //close the socket
            close(client_socket);            
            printf("Socket Closed %d\n:", client_socket); 
            exit(1);
            return 0;
        }
        else {
            printf("%s", response);
        } 
    }
  return 0; //success
}