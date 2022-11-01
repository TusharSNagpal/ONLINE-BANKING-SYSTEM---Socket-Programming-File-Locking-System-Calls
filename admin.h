#include<string.h>
#include "./login.h"
#include <stdio.h>    
#include <unistd.h>    
#include <string.h>  
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>     
#include <stdlib.h>
#include <errno.h>

void search_customer(int sockFD){
	struct customer{
                int cust_id;
                int acc_no;
                char user_name[1000];
                char password[1000];
                char gender[1000];
                char age[1000];
        } cust;
	int accFD = open("account.db", O_RDWR, 0744);
	struct account{
                        int accNumber;
                        int jointOwners[2];
                        int balance;
                        bool active;
        } new_acc;

        char msgReq1[1000] = "Please Enter CUSTOMER ID: ";
        char msgReq2[1000] = "Following are the Details: \n~";
	
	write(sockFD, msgReq1, sizeof(msgReq1));

        char customerID[1000];
        read(sockFD, &customerID, sizeof(customerID));

        int cID = atoi(customerID);

        int cfd = open("customer.db", O_RDWR, 0744);

        lseek(cfd, cID*sizeof(cust), SEEK_SET);

        read(cfd, &cust, sizeof(cust));

	lseek(accFD, cust.acc_no*sizeof(new_acc), SEEK_SET);

        read(accFD, &new_acc, sizeof(new_acc));
        
        printf("%i\n", new_acc.active);
        printf("%d\n", new_acc.accNumber);

        if(!new_acc.active){
                char deMsg[1000] = "Sorry account associated with this customer ID is deactivated..!\n~";
                write(sockFD,deMsg, sizeof(deMsg));
                return;
        }
	
	write(sockFD, msgReq2, sizeof(msgReq2));

                char res[1000] = "\n";
                char accno[1000];

                sprintf(accno, "%d", cust.acc_no);
                strcat(res, "ACCOUNT NUMBER: ");
                strcat(res, accno);
		strcat(res, "\n");

		strcat(res, "ACCOUNT: ACTIVE\n");

                char custID[1000];
                sprintf(custID, "%d", cust.cust_id);
                strcat(res, "CUSTOMER ID: ");
                strcat(res, custID);
		strcat(res, "\n");

                strcat(res, "FULL NAME: ");
                strcat(res, cust.user_name);
		strcat(res, "\n");

                strcat(res, "GENDER: ");
                strcat(res, cust.gender);
		strcat(res, "\n");

                strcat(res, "AGE: ");
                strcat(res, cust.age);
		strcat(res, "\n");

                strcat(res, "\n~");

                write(sockFD, res, sizeof(res));

		return;
}

void delete_account(int sockFD){
	char msgReq[1000] = "Enter Account Number: (If you don't remember you can use Customer Details option in MENU and then use your Customer ID to access Account Number\n";
	write(sockFD, msgReq, sizeof(msgReq));
	int accFD = open("account.db", O_RDWR, 0744);
	struct account{
                int accNumber;
                int jointOwners[2];
                int balance;
                bool active;
        } new_acc;

        lseek(accFD, new_acc.accNumber*sizeof(new_acc), SEEK_SET);

        read(accFD, &new_acc, sizeof(new_acc));

	if(new_acc.balance > 0){
		char deMsg[1000] = "Please withdraw your money before requesting for deactivation..!(A/C Balance should be 0)\n";
		write(sockFD, deMsg, sizeof(deMsg));
		return;
	}

	new_acc.active = false;

	lseek(accFD, new_acc.accNumber*sizeof(new_acc), SEEK_SET);

        write(accFD, &new_acc, sizeof(new_acc));

	char confirm[1000] = "Account Deleted Successfully..!~\n";

	write(sockFD, confirm, sizeof(confirm));
	return;
}

void modify_account(int sockFD){
	struct customer{
		int cust_id;
                int acc_no;
                char user_name[1000];
                char password[1000];
                char gender[1000];
                char age[1000];
	} cust;
	char msgReq1[1000] = "Please Enter CUSTOMER ID: ";
	char msgReq2[1000] = "Following are the Details: \n~";
	char msgReq3[1000] = "Please Enter Password: ";

	write(sockFD, msgReq1, sizeof(msgReq1));
	
	char customerID[1000];
	read(sockFD, &customerID, sizeof(customerID));

	int cID = atoi(customerID);
	
	int cfd = open("customer.db", O_RDWR, 0744);
	
	lseek(cfd, cID*sizeof(cust), SEEK_SET);	

	read(cfd, &cust, sizeof(cust));

	int accFD = open("account.db", O_RDWR, 0744);
        struct account{
                        int accNumber;
                        int jointOwners[2];
                        int balance;
                        bool active;
        } new_acc;

        lseek(accFD, cust.acc_no*sizeof(new_acc), SEEK_SET);

        read(accFD, &new_acc, sizeof(new_acc));

        if(!new_acc.active){
                char deMsg[1000] = "Sorry account associated with this customer ID is deactivated..!\n~";
                write(sockFD,deMsg, sizeof(deMsg));
                return;
        }

	
	char passwordIn[1000];
	write(sockFD, msgReq3, sizeof(msgReq3));
	read(sockFD, passwordIn, sizeof(passwordIn));

	if(strcmp(passwordIn, cust.password)==0){
		char msg4[1000] = "LOGGED IN SUCCESSFULLY\n~";
		write(sockFD, msg4, sizeof(msg4));

		write(sockFD, msgReq2, sizeof(msgReq2));

		char pres[1000] = "\n";
       	 	char accno[1000];

        	sprintf(accno, "%d", cust.acc_no);
        	strcat(pres, "ACCOUNT NUMBER: ");
        	strcat(pres, accno);
		strcat(pres, "\n");

        	char custID[1000];
        	sprintf(custID, "%d", cust.cust_id);
        	strcat(pres, "CUSTOMER ID: ");
        	strcat(pres, custID);
		strcat(pres, "\n");

        	strcat(pres, "FULL NAME: ");
       		strcat(pres, cust.user_name);
		strcat(pres, "\n");

        	strcat(pres, "GENDER: ");
       	 	strcat(pres, cust.gender);
		strcat(pres, "\n");

        	strcat(pres, "AGE: ");
        	strcat(pres, cust.age);
		strcat(pres, "\n");

        	strcat(pres, "\n~");

        	write(sockFD, pres, sizeof(pres));
		

		char menuMod[1000] = "Choose what you want to modify: \n1. FULL NAME\n2. GENDER\n3. AGE\n4. PASSWWORD\n";
		write(sockFD, menuMod, sizeof(menuMod));
		char choiceIn[1000];
		read(sockFD, choiceIn, sizeof(choiceIn));

		int choice = atoi(choiceIn);

		switch(choice){
			case 1: {
					char msgReq[1000] = "Enter New NAME: ";
					write(sockFD, msgReq, sizeof(msgReq));

					read(sockFD, &cust.user_name, sizeof(cust.user_name));
					break;
				}
			case 2: {
                                        char msgReq[1000] = "Enter New GENDER: ";
                                        write(sockFD, msgReq, sizeof(msgReq));

                                        read(sockFD, &cust.gender, sizeof(cust.gender));
					break;
                                }
			case 3: {
                                        char msgReq[1000] = "Enter New AGE: ";
                                        write(sockFD, msgReq, sizeof(msgReq));

                                        read(sockFD, &cust.age, sizeof(cust.age));
					break;
                                }
			case 4: {
                                        char msgReq[1000] = "Enter New PASSWORD: ";
                                        write(sockFD, msgReq, sizeof(msgReq));

                                        read(sockFD, &cust.password, sizeof(cust.password));
					break;
                                }
			default:{
					char msgReq[1000] = "Please make a valid Selection..!\n~";
					write(sockFD, msgReq, sizeof(msgReq));
				}

		}

		lseek(cfd, cID*sizeof(cust), SEEK_SET);
		write(cfd, &cust, sizeof(cust));
		
		struct customer custUp;
		lseek(cfd, cID*sizeof(cust), SEEK_SET);
		read(cfd, &custUp, sizeof(custUp));

		char upres[1000] = "Account Details Updated Successfully..!\nPlease remember these details to access your account:\n\n";
		char accnoUp[1000];		
        	sprintf(accnoUp, "%d", custUp.acc_no);
	        strcat(upres, "ACCOUNT NUMBER: ");
       		strcat(upres, accnoUp);
		strcat(upres, "\n");
		
		char custIDUp[1000];
        	sprintf(custIDUp, "%d", custUp.cust_id);
       		strcat(upres, "CUSTOMER ID: ");
        	strcat(upres, custIDUp);
		strcat(upres, "\n");

        	strcat(upres, "FULL NAME: ");
        	strcat(upres, custUp.user_name);
		strcat(upres, "\n");

        	strcat(upres, "GENDER: ");
        	strcat(upres, custUp.gender);
		strcat(upres, "\n");

        	strcat(upres, "AGE: ");
        	strcat(upres, custUp.age);
		strcat(upres, "\n");

        	strcat(upres, "PASSWORD: ");
        	strcat(upres, custUp.password);
		strcat(upres, "\n");

        	strcat(upres, "\n~");

        	write(sockFD, upres, sizeof(upres));

	}
	else{
		char wrong[1000] = "Incorrect CUSTOMER ID/PASSWORD\n\n~";
		write(sockFD, wrong, sizeof(wrong));
	}
}
int add_customer(int sockFD, int accNumber, bool isfirst){
	int custFD = open("customer.db", O_RDWR, 0744);
	int cur_cust_fd = open("cur_cust_id.db", O_RDWR, 0744);

	struct customer{
		int cust_id;
		int acc_no;
		char user_name[1000];
		char password[1000];
		char gender[1000];
		char age[1000];
	} new_cust;

		new_cust.acc_no = accNumber;

		int cur_cust_id;

		read(cur_cust_fd, &cur_cust_id, sizeof(cur_cust_id));

		new_cust.cust_id = cur_cust_id;

		cur_cust_id++;

		lseek(cur_cust_fd, 0, SEEK_SET);

		write(cur_cust_fd, &cur_cust_id, sizeof(cur_cust_id));

		lseek(custFD,new_cust.cust_id*(sizeof(new_cust)), SEEK_SET);

		char msg_det[1000] = "Please Enter the following Details: ";

		if(isfirst)
			strcat(msg_det, "for first owner: \n~");
		else
			strcat(msg_det, "for second owner: \n~");
		char msg_name[1000] = "FULL NAME: ";
		char msg_gender[1000] = "GENDER:\nM -> MALE\nF -> FEMALE\nO -> OTHERS\n";
		char msg_age[1000] = "AGE: ";
		char msg_pass[1000] = "SET PASSWORD: ";	

		write(sockFD, msg_det, sizeof(msg_det));

		write(sockFD, msg_name, sizeof(msg_name));
		read(sockFD, &new_cust.user_name, sizeof(new_cust.user_name));

		write(sockFD, msg_gender, sizeof(msg_gender));
        	read(sockFD, &new_cust.gender, sizeof(new_cust.gender));

		write(sockFD, msg_age, sizeof(msg_age));
        	read(sockFD, &new_cust.age, sizeof(new_cust.age));

		write(sockFD, msg_pass, sizeof(msg_pass));
        	read(sockFD, &new_cust.password, sizeof(new_cust.password));
	
        	new_cust.cust_id++;

		lseek(custFD, new_cust.cust_id*(sizeof(new_cust)), SEEK_SET);

        	write(custFD, &new_cust, sizeof(new_cust));

		lseek(custFD, 0, SEEK_SET);

		lseek(custFD, new_cust.cust_id*(sizeof(new_cust)), SEEK_SET);

		read(custFD, &new_cust, sizeof(new_cust));

		//reading accountNo Associated Details:

		int accFD = open("account.db", O_RDWR, 0744);

	        struct account{
        	        int accNumber;
                	int jointOwners[2];
                	int balance;
                	bool active;
       	 	} new_acc;

		lseek(accFD, new_acc.accNumber*sizeof(new_acc), SEEK_SET);

        	read(accFD, &new_acc, sizeof(new_acc));

		new_acc.active = true;

		lseek(accFD, new_acc.accNumber*sizeof(new_acc), SEEK_SET);
		
		write(accFD, &new_acc, sizeof(new_acc));

		char res[1000] = "Account Created Successfully..!\nPlease remember these details to access your account:\n\n";
		char accno[1000];

       		sprintf(accno, "%d", new_cust.acc_no);
		strcat(res, "ACCOUNT NUMBER: ");
		strcat(res, accno);
		strcat(res, "\n");

		char custID[1000];
		sprintf(custID, "%d", new_cust.cust_id);
		strcat(res, "CUSTOMER ID: ");
		strcat(res, custID);
		strcat(res, "\n");

		strcat(res, "FULL NAME: ");
		strcat(res, new_cust.user_name);
		strcat(res, "\n");

		strcat(res, "GENDER: ");
		strcat(res, new_cust.gender);
		strcat(res, "\n");

		strcat(res, "AGE: ");
		strcat(res, new_cust.age);
		strcat(res, "\n");

		strcat(res, "PASSWORD: ");
		strcat(res, new_cust.password);
		strcat(res, "\n");
	
		strcat(res, "\n~");

		write(sockFD, res, sizeof(res));

		return new_cust.cust_id;
}

void add_account(int socFD){
	int accFD = open("account.db", O_RDWR, 0744);
	
	struct account{
		int accNumber;
		int jointOwners[2];
		int balance;
		bool active;
	} new_acc;

	int cur_accFD = open("cur_ac.db", O_RDWR, 0744);
	int cur_ac_no;
	lseek(cur_accFD, 0, SEEK_SET);
	read(cur_accFD, &cur_ac_no, sizeof(int));

	lseek(accFD, new_acc.accNumber*sizeof(new_acc), SEEK_SET);

        read(accFD, &new_acc, sizeof(new_acc));

        new_acc.accNumber = cur_ac_no;

	cur_ac_no++;

        char menu[1000] = "Select Account Type: \n1. Regular A/C\n2. Joint A/C\n";

        write(socFD, menu, sizeof(menu));

        char accType[1000];
        read(socFD, accType, sizeof(accType));

        if(strcmp(accType,"1") == 0){
                new_acc.jointOwners[0] = add_customer(socFD, new_acc.accNumber, 1);
                new_acc.jointOwners[1] = -1;
        }
        else{
                if(strcmp(accType,"2") == 0){
                        new_acc.jointOwners[0] = add_customer(socFD, new_acc.accNumber, 1);
                        new_acc.jointOwners[1] = add_customer(socFD, new_acc.accNumber, 0);
                }
        }

        lseek(accFD, new_acc.accNumber*sizeof(new_acc), SEEK_SET);

        write(accFD, &new_acc, sizeof(new_acc));

	lseek(cur_accFD, 0, SEEK_SET);
	write(cur_accFD, &cur_ac_no, sizeof(int));


}

void admin_work(int socFD){
	if(admin_login_work(socFD)){
		char welMsg[1000] = "ADMIN LOGGED IN SUCCESSFULLY..!\n~";
		write(socFD, welMsg, sizeof(welMsg));
		
		while(1){
			char menu[1000] = "Welcome! Please Select one option from the below menu: \n1. Add Account\n2. Delete Account\n3. Modify Account\n4. Search Customer\n5. LOGOUT\n";
			write(socFD, menu, sizeof(menu));
			char buffer[1000];
			read(socFD, buffer, sizeof(buffer));

			char text[1000] = "Selected Option is ";

			strcat(text, buffer);
			strcat(text,"\n~");
			
			write(socFD, text, sizeof(text));

			//switching to that particular case:
			int choice = buffer[0] - '0';
			switch(choice){
				case 1:
					add_account(socFD);
					break;
				case 2:
					delete_account(socFD);
					break;
				case 3:
					modify_account(socFD);
					break;
				case 4:
					search_customer(socFD);
					break;
					
				case 5:{
					char logout[1000] = "Logged Out Successfully..!\n\n~";
					write(socFD, logout, sizeof(logout));
					return;
					}
					
				default:{
						char tempmsg[1000] = "Please Enter a valid choice..!\n\n~";
						write(socFD,tempmsg, sizeof(tempmsg));
						admin_work(socFD);
					}
			}
		}		
	}
	else{
		admin_login_work(socFD);
		char sec[1000] = "For security reasons please login again..!\n~";
		write(socFD, sec, sizeof(sec));
	}
}
