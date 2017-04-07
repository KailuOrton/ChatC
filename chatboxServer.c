#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<sys/socket.h>

#define PORT 4444
#define MESSAGE_SIZE 2000
#define ADDER 100

void * receiveMessage(void * socket){
    int serverSock, conditional;
    char message[MESSAGE_SIZE];
    serverSock = (int) socket;
    memset(message,0,MESSAGE_SIZE);
    while(1){
        conditional = recvfrom(serverSock,message,MESSAGE_SIZE,0,NULL,NULL);
        if(conditional < 0){
            printf("Error procuring data!\n");
        }
        else{
            printf("client: ");
            fputs(message, stdout);
            printf("\n");
        }
    }
}

int main(){
    struct sockaddr_in address, client_address;
    int sock, newSocket, len, conditional;
    char message[MESSAGE_SIZE];
    pid_t childpid;
    char clientAddress[ADDER];
    pthread_t rThread;

    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0){
        printf("Error creating socket!\n");
    }
    printf("Socket Successfully created...\n");

    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = PORT;

    conditional = bind(sock,(struct sockaddr *)&address,sizeof(address));
    if(conditional < 0){
        printf("Error binding!\n");
        exit(1);
    }
    printf("Binding done...\n");
    
    printf("Waiting for a connection...\n");
    listen(sock,5);

    len = sizeof(client_address);
    newSocket = accept(sock,(struct sockaddr *) &client_address,&len);
    if(newSocket < 0){
        printf("error accepting connection...\n");
        exit(1);
    }

    inet_ntop(AF_INET,&(client_address.sin_addr),clientAddress,ADDER);
    printf("Connection accepted from %s...\n",clientAddress);
    
    memset(message,0,MESSAGE_SIZE);
    printf("Enter your message and hit the return key!\n");

    //create a new thread to receive messages from the client
    
    conditional = pthread_create(&rThread,NULL,receiveMessage,(void *) newSocket);

    if(conditional){
        printf("error return code from pThread: %d",conditional);
        exit(1);
    }

    while(fgets(message,MESSAGE_SIZE,stdin) != NULL){
        conditional = sendto(newSocket,message,MESSAGE_SIZE,0,(struct sockaddr *) &client_address,len);
        if(conditional < 0){
            printf("Error sending data!\n");
            exit(1);
        }
    }
    
    close(newSocket);
    close(sock);

    pthread_exit(NULL);
    return 0;
}

