#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/ip.h>

void main(){
	printf("CLIENT");
	struct sockaddr_in serverAdd;

	//int socket(int domain, int type, int protocol);
	//socket creates an endpoint and returns its file descriptor..
	
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	//Domain AF_INET means: Internet Protocol IPv4
	//SOCK_STREM means: sequential and reliable 2 way communication..
	
	if(fd<0){
		perror("Socket System Call Failed.\n");
		return;
	}

	serverAdd.sin_family = AF_INET;
	serverAdd.sin_addr.s_addr = INADDR_ANY; //auto assignment of ip address of current m/c
	int port;
	printf("Enter port number: ");
	scanf("%d", &port);

	serverAdd.sin_port = htons(port);

	//Connect SYNTAX:
	// int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	
	connect(fd, (void*)(&serverAdd), sizeof(serverAdd));

	//send msg:
	int readStatus;
	do{
		char buffer[1000];
		readStatus = read(fd, buffer, sizeof(buffer));
		if(strchr(buffer, '~')!=NULL){	
			write(1, buffer, strlen(buffer)-1);
		}	
		else{
			printf("%s",buffer);
			char userIn[1000];
			scanf("%s", userIn);
			write(fd, userIn, sizeof(userIn));
		}
	}
	while(readStatus>0);
}
