#include<stdio.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<netinet/ip.h>
void customer_work(int sockFD);
void transaction_detail(int tid, int sockFD){
	struct transactions{
                        int acc_no;
                        int cust_id;
                        int transID;
                        int cur_amount;
                        int updated_amount;
                        char dateNtime[1000];
                        bool deposit;
        	} trans;

	 int transdbFD = open("transaction.db", O_RDWR, 0744);

         lseek(transdbFD, tid*sizeof(trans), SEEK_SET);

	 read(transdbFD, &trans, sizeof(trans));
	 
	 if(trans.cust_id == 0){
	 	char alert[1000] = "No transaction found associated to this Transaction ID..! Please try again.\n\n~";
	 	write(sockFD, alert, sizeof(alert));
	 	customer_work(sockFD);
	 }
         else{
	 	char res[1000] = "\n---TRANSACTION DETAILS--- \n";

	 	char cid[1000];
	 	sprintf(cid, "%d", trans.cust_id);
	 	strcat(res, "CUSTOMER ID: ");
	 	strcat(res, cid);
	 	strcat(res, "\n");

	 	int diff = trans.updated_amount - trans.cur_amount;
	 
	 	char difference[1000];

	 	if(trans.deposit){
		 	strcat(res, "Amount Deposited: Rs ");

		 	sprintf(difference, "%d", diff);

		 	strcat(res, difference);
		 	strcat(res, "\n");
	 	}
	 	else{
		 	strcat(res, "Amount Withdrawn: Rs ");

		 	sprintf(difference, "%d", abs(diff));

		 	strcat(res, difference);
		 	strcat(res, "\n");
	 	}

	 	strcat(res, "Date and Time: ");

	 	strcat(res, trans.dateNtime);

	 	strcat(res, "\n\n~");

	 	write(sockFD, res, sizeof(res));
	 }
}
void details_customer(int sockFD){
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
        char msgReq2[1000] = "---Following are the Details---\n\n~";

        write(sockFD, msgReq1, sizeof(msgReq1));

        char customerID[1000];
        read(sockFD, &customerID, sizeof(customerID));

        int cID = atoi(customerID);

        int cfd = open("customer.db", O_RDWR, 0744);

        lseek(cfd, cID*sizeof(cust), SEEK_SET);

        read(cfd, &cust, sizeof(cust));

        lseek(accFD, cust.acc_no*sizeof(new_acc), SEEK_SET);

        read(accFD, &new_acc, sizeof(new_acc));

        if(!new_acc.active){
		char deMsg[1000] = "Sorry account associated with this customer ID is deactivated..!\n\n~";
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

                strcat(res, "\n\n~");
		write(sockFD, res, sizeof(res));

                return;
}

//main: CUSTOMER:

void customer_work(int sockFD){
	char msg[1000] = "---CUSTOMER PORTAL---\n\n~";
	write(sockFD, msg, sizeof(msg));
	struct customer{
                int cust_id;
                int acc_no;
                char user_name[1000];
                char password[1000];
                char gender[1000];
                char age[1000];
        } cust;
        char msgReq1[1000] = "Please LOGIN:\nEnter CUSTOMER ID: ";
        char msgReq2[1000] = "Following are the Details: \n\n~";
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
                char deMsg[1000] = "Sorry account associated with this customer ID is deactivated..!\n\n~";
                write(sockFD,deMsg, sizeof(deMsg));
                return;
        }

        char passwordIn[1000];
        write(sockFD, msgReq3, sizeof(msgReq3));
        read(sockFD, passwordIn, sizeof(passwordIn));

        if(strcmp(passwordIn, cust.password)==0){
                char msg4[1000] = "LOGGED IN SUCCESSFULLY\n\n~";
                write(sockFD, msg4, sizeof(msg4));

                write(sockFD, msgReq2, sizeof(msgReq2));
		char pres[1000] = "\n";
                char accno[1000];

                sprintf(accno, "%d", cust.acc_no);
                strcat(pres, "ACCOUNT NUMBER: ");
                strcat(pres, accno);
                strcat(pres, "\n");

		strcat(pres, "ACCOUNT ACTIVE\n");

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

                strcat(pres, "\n\n~");

                write(sockFD, pres, sizeof(pres));

		while(1){

		char menuMod[1000] = "---CUSTOMER SERVICES---\n1. DEPOSIT\n2. WITHDRAW\n3. BALANCE ENQUIRY\n4. PASSWORD CHANGE\n5. VIEW DETAILS\n6. EXIT/LOGOUT\n";

                write(sockFD, menuMod, sizeof(menuMod));
                char choiceIn[1000];
                read(sockFD, choiceIn, sizeof(choiceIn));

                int choice = atoi(choiceIn);

		int transFD = open("transaction.db", O_RDWR, 0744);

		struct transactions{
			int acc_no;
			int cust_id;
			int transID;
			int cur_amount;
			int updated_amount;
			char dateNtime[1000];
			bool deposit;
		} trans;
		int cur_trans_fd = open("cur_transID.db", O_RDWR, 0744);
		int transdbFD = open("transaction.db", O_RDWR, 0744);
		int accountFD = open("account.db", O_RDWR, 0744);
		struct flock lockingW;	
		if(choice != 6){
			char msgWait[1000] = "Please wait.. checking if a transaction is already going on..!\n\n~";
			write(sockFD, msgWait, sizeof(msgWait));
			
			lockingW.l_type = F_WRLCK;
        		//lockingW.l_whence = SEEK_SET;
        		lockingW.l_start = cust.acc_no*sizeof(new_acc);
        		lockingW.l_len = sizeof(new_acc);
        		lockingW.l_pid = getpid();
        		
			fcntl(accFD, F_SETLKW, &lockingW);
			fcntl(cur_trans_fd, F_SETLKW, &lockingW);
			fcntl(transdbFD, F_SETLKW, &lockingW);
		}
                switch(choice){
			case 1: { //deposit:
                                        char msgReq[1000] = "Enter AMOUNT you want to deposit: ";
                                        write(sockFD, msgReq, sizeof(msgReq));
					char deposit[1000];

                                        read(sockFD, &deposit, sizeof(deposit));

					int d = atoi(deposit);
					
					lseek(accountFD, cust.acc_no*sizeof(new_acc), SEEK_SET);
					
					read(accountFD, &new_acc, sizeof(new_acc));

					trans.cur_amount = new_acc.balance; //4

					new_acc.balance += d;
					
					char depCon[1000] = "Dear ";
					strcat(depCon, cust.user_name);
					strcat(depCon, " Rs ");
					strcat(depCon, deposit);
					strcat(depCon, " has been deposited in your account.\n\n~");
					write(sockFD, depCon, sizeof(depCon));

					lseek(accountFD, cust.acc_no*sizeof(new_acc), SEEK_SET);

					write(accountFD, &new_acc, sizeof(new_acc));
					
					trans.deposit = true; //7
					trans.cust_id = cust.cust_id; //2
					trans.updated_amount = new_acc.balance; //5
					trans.acc_no = new_acc.accNumber; //1
					//fetch date and time:
					time_t t = time(NULL);
    					struct tm *tm = localtime(&t);
    					char s[64];
    					size_t ret = strftime(s, sizeof(s), "%c", tm);
    					assert(ret);
					//end
					
					strcpy(trans.dateNtime,s); //6

					//fetch trans id:
					
					//open cur_trans_id:	

					int cur_trans_id;

					lseek(cur_trans_fd, 0, SEEK_SET);

					read(cur_trans_fd, &cur_trans_id, sizeof(cur_trans_id));
					
					trans.transID = cur_trans_id;

					cur_trans_id++;
					
					lseek(cur_trans_fd, 0, SEEK_SET);

					write(cur_trans_fd, &cur_trans_id, sizeof(cur_trans_id));

					//open transdbFD:

					lseek(transdbFD, (cur_trans_id-1)*sizeof(trans), SEEK_SET);

					write(transdbFD, &trans, sizeof(trans));

					char transConf[1000] = "Transaction successful! Transaction ID: ";
					char tID[1000];
					sprintf(tID, "%d", trans.transID);

					strcat(transConf, tID);

					strcat(transConf,"\n\n~");
					
					write(sockFD, transConf, sizeof(transConf));

                                        break;
                                }
			case 2: { //withdraw:
                                        char msgReq[1000] = "Enter AMOUNT you want to withdraw: ";
                                        write(sockFD, msgReq, sizeof(msgReq));

					  char withd[1000];
					
                                        read(sockFD, &withd, sizeof(withd));

                                        int w = atoi(withd);
                                        
                                        trans.cur_amount = new_acc.balance; //4

					 lseek(accountFD, cust.acc_no*sizeof(new_acc), SEEK_SET);

					 read(accountFD, &new_acc, sizeof(new_acc));

					 if(new_acc.balance < w){
						char notFunds[1000] = "Sorry not enough funds..!\n\n~";
						write(sockFD, notFunds, sizeof(notFunds));
						return;
					 }

					 new_acc.balance -= w;

					 char withCon[1000] = "Dear ";
                                        strcat(withCon, cust.user_name);
                                        strcat(withCon, " Rs ");
                                        strcat(withCon, withd);
                                        strcat(withCon, " has been withdrawn from your account.\n\n~");
                                        write(sockFD, withCon, sizeof(withCon));

                                        lseek(accountFD, cust.acc_no*sizeof(new_acc), SEEK_SET);

                                        write(accountFD, &new_acc, sizeof(new_acc));
					 trans.deposit = false; //7
					 trans.cust_id = cust.cust_id; //2
					 trans.updated_amount = new_acc.balance; //5
					 trans.acc_no = new_acc.accNumber; //1
					 //fetch date and time:
					 time_t t = time(NULL);
    					 struct tm *tm = localtime(&t);
    					 char s[64];
    					 size_t ret = strftime(s, sizeof(s), "%c", tm);
    					 assert(ret);
					 //end
					
					 strcpy(trans.dateNtime,s); //6

					 //fetch trans id:
					 //int cur_trans_fd = open("cur_transID.db", O_RDWR, 0744);

					 int cur_trans_id;

					 lseek(cur_trans_fd, 0, SEEK_SET);

					 read(cur_trans_fd, &cur_trans_id, sizeof(cur_trans_id));
					 
					 trans.transID = cur_trans_id;

					 cur_trans_id++;
					
					 lseek(cur_trans_fd, 0, SEEK_SET);

					 write(cur_trans_fd, &cur_trans_id, sizeof(cur_trans_id));

					 //int transdbFD = open("transaction.db", O_RDWR, 0744);

					 lseek(transdbFD, (cur_trans_id-1)*sizeof(trans), SEEK_SET);

					 write(transdbFD, &trans, sizeof(trans));

					 char transConf[1000] = "Transaction successful! Transaction ID: ";
					 char tID[1000];
					 sprintf(tID, "%d", trans.transID);

					 strcat(transConf, tID);

					 strcat(transConf,"\n\n~");
					
					 write(sockFD, transConf, sizeof(transConf));
					
                                       break;
                                 }
                        case 3: {
                                        char msgReq[1000] = "BALANCE: ";

					lseek(accountFD, cust.acc_no*sizeof(new_acc), SEEK_SET);

                                        read(accountFD, &new_acc, sizeof(new_acc));

					char bal[1000];

					sprintf(bal, "%d", new_acc.balance);

					strcat(msgReq ,bal);

					strcat(msgReq, "\n\n~");

                                        write(sockFD, msgReq, sizeof(msgReq));

                                        break;
                                }
                        case 4: {
                                        char msgReq[1000] = "Enter New PASSWORD: ";
                                        write(sockFD, msgReq, sizeof(msgReq));
                                        read(sockFD, &cust.password, sizeof(cust.password));                                        

					 lseek(cfd, cID*sizeof(cust), SEEK_SET);

					 write(cfd, &cust, sizeof(cust));

					 char confi[1000] = "Password Successfully changed..!\n\n~";

					 write(sockFD, confi, sizeof(confi));

                                        break;
				}
                        case 5: {
                                        char msgReq[1000] = "---Please select---\n1. Customer Details\n2. Transaction Details\n";
                                        write(sockFD, msgReq, sizeof(msgReq));

					char choiceIn[1000];
					read(sockFD, choiceIn, sizeof(choiceIn));

					if(strcmp(choiceIn, "1") == 0)
						details_customer(sockFD);

					else if(strcmp(choiceIn, "2") == 0){
						char trMsg[1000] = "Enter Transaction ID: ";
						write(sockFD, trMsg, sizeof(trMsg));

						char tIn[1000];
						
						read(sockFD, tIn, sizeof(tIn));

						int tid = atoi(tIn);
						transaction_detail(tid, sockFD); //function call..
						}

					break;
                                }
			case 6:
				{
					char msgReq[1000] = "Thank you for using our services..! Have a nice day :)\n\n~";
					write(sockFD, msgReq, sizeof(msgReq));

					return;
				}
			default:
				{
					char msgReq[1000] = "Please make a valid selection..!\n\n~";
					write(sockFD, msgReq, sizeof(msgReq));
				}
		}
		
		lockingW.l_type = F_UNLCK;
              
               fcntl(accFD, F_SETLK, &lockingW);
               fcntl(cur_trans_fd, F_SETLK, &lockingW);
		fcntl(transdbFD, F_SETLK, &lockingW);
                
		}

	}
}
