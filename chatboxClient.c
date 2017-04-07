#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<netdb.h>
#include<pthread.h>

#define PORT 4444
#define MESSAGE_SIZE 2000

void * receiveMessage(void * socket){
    int sock, conditional;
    char message[MESSAGE_SIZE];
    
    sock = (int) socket;
    memset(message,0,MESSAGE_SIZE);
    while(1){
        conditional = recvfrom(sock,message,MESSAGE_SIZE,0,NULL,NULL);
        if(conditional < 0){
            printf("Error receiving data..");
        }
        else{
            fputs(message,stdout);   
            printf("\n");
        }
    }
}

int main(int argc, char**argv){
    struct sockaddr_in address, client_address;
    int socket, conditional;
    char message[MESSAGE_SIZE];
    char * serverAddress;
    pthread_t rThread;

    if(argc < 2){
        printf("usage: client < ip address >\n");
        exit(1);
    }
    printf("Socket Created...\n");

    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(serverAddress);
    address.sin_port = PORT;
    printf("cool\n");
    conditional = connect(socket,(struct sockaddr *) &address,sizeof(address));
    if(conditional < 0){
        printf("Error connecting to the server..\n");
        exit(1);
    }
    printf("connected to the server..\n");

    memset(message,0,MESSAGE_SIZE);
    printf("Enter your message and press return key!\n");

    conditional = pthread_create(&rThread,NULL,receiveMessage,(void *)socket);
    if(conditional){
        printf("ERROR: pThread_create returned code %d.\n",conditional);
        exit(1);
    }
    while(fgets(message,MESSAGE_SIZE,stdin) != NULL){
        conditional = sendto(socket,message,MESSAGE_SIZE,0,(struct sockaddr *) &address,sizeof(address));
        if(conditional < 0){
            printf("Error sending data..\n");
        }
    }

    close(socket);
    pthread_exit(NULL);
}
                    
