#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

struct customer{
	int id;
};

bool admin_login_work(int sockFD){
	char msg[1000] = "---ADMIN SERVICES---\nPlease Enter login details:\nID: ";
	int writeStatus = write(sockFD, msg, sizeof(msg));

	char reply[1000];
	printf("Here\n");
        int readStatus = read(sockFD, reply, sizeof(reply));
		
		
	if(readStatus == -1){
	perror("Read\n");
	}

	if (strcmp(reply, "tushar_s_nagpal") == 0){
		char msg2[1000] = "\nPassword: ";
		writeStatus = write(sockFD, msg2, sizeof(msg2));
		char reply2[1000];
		readStatus = read(sockFD, reply2, sizeof(reply2));
		
		if(strcmp(reply2, "systemD") == 0){
			return true;
		}

		else{
			char msgFail[1000] = "Incorrect Password.. Try Again!\n\n~";
			write(sockFD, msgFail, sizeof(msgFail));
			return false;
		}
	}
	else{
		char msgFail[1000] = "No such user found..! Try Again!\n\n~";
		write(sockFD, msgFail, sizeof(msgFail));
		return false;
	}

	return false;
}
