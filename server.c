#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<netinet/ip.h>

//admin:
#include "./admin.h"

//customer:
#include "./customer.h"

void client_work(int newSocketFD){
	printf("WELCOME CLIENT..! Connection Successful..!\n");
	char buffer[1000];
	char msgS[1000] = " _______________________________________ \n|                                       |\n|    ---PLEASE SELECT ACCESS TYPE---    |\n|               1. ADMIN                |\n|               2. CUSTOMER             |\n|_______________________________________|\n";
	write(newSocketFD, msgS, sizeof(msgS));
	int readStatus;

	readStatus = read(newSocketFD, &buffer, sizeof(buffer));

	if(strcmp("1", buffer) == 0){
		admin_work(newSocketFD);
		client_work(newSocketFD);
	}
	
	else if(strcmp("2", buffer) == 0){
		customer_work(newSocketFD);
		client_work(newSocketFD);
	}
	else{
		write(newSocketFD, "Restart(Wrong I/P)\n", 19);
		client_work(newSocketFD);
	}
}

void main(){
	printf("Server..!\n");

	struct sockaddr_in server, client;

	int socketFD = socket(AF_INET, SOCK_STREAM, 0);

	if(socketFD<0){
		perror("Error in socket system call..!\n");
		return;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;

	int port;

	printf("Enter the port number on which you want to start the server: NOTE- Please use same port number for client ");
	scanf("%d", &port);


	server.sin_port = htons(port);

	//bind:
	int bindStatus = bind(socketFD, (void*) (&server), sizeof(server));

	if(bindStatus == -1){
		perror("Error in bind()\n");
		return;
	}

	//listen
	int listenStatus = listen(socketFD, 5);

	if(listenStatus == -1){
		perror("Error in listen()\n");
		return;
	}

	while(1){
		int sizeClient = sizeof(client);

		int newSocketFD = accept(socketFD, (void*)(&client), &sizeClient);

		if(newSocketFD < 0){
			perror("Error in accept()\n");
			return;
		}

		if(fork() == 0){
			client_work(newSocketFD);
		}
		else{
			close(newSocketFD);
		}
	}
	close(socketFD);
}
