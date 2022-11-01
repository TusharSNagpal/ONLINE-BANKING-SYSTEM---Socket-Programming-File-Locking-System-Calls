#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdbool.h>
void main(){

	//Initializing account database..
	struct account{
		int account_no;
		int owners[2];
		int balance;
		bool active;
	} newAcc;
	
	newAcc.account_no = 111111;
	newAcc.balance = 0;
	newAcc.active = true;
	int fd = open("account.db", O_RDWR|O_CREAT|O_EXCL, 0744);
	lseek(fd, 0, SEEK_SET);

	write(fd, &newAcc, sizeof(newAcc));
	
	
	//Initializing customer database..
	
	int custFD = open("cur_cust_id.db", O_RDWR|O_CREAT|O_EXCL, 0744);

        struct customer{
                int cust_id;
        } new_cust;
	
	new_cust.cust_id = 444444;

	lseek(custFD, 0, SEEK_SET);

	write(custFD, &new_cust, sizeof(new_cust));
	
	int custdb_fd = open("customer.db", O_RDWR|O_CREAT|O_EXCL, 0744);
	
	//Intializing current account id..
	
	int cFD = open("cur_ac.db", O_RDWR|O_CREAT|O_EXCL, 0744);
	
	int cur_id = 111111;

	lseek(cFD, 0, SEEK_SET);

	write(cFD, &cur_id, sizeof(int));
	
	
	//Intializing transaction database..
	
	int transFD = open("transaction.db", O_RDWR|O_CREAT|O_EXCL, 0744);
	
	struct transactions{
		int acc_no;
		int cust_id;
		int transID;
		int cur_amount;
		int updated_amount;
		char dateNtime[1000];
		bool deposit;
	}transDB;

	lseek(transFD, transDB.transID*sizeof(transDB), SEEK_SET);

	write(transFD, &transDB, sizeof(transDB));
	
	//Initializing current transaction ID..
	
	int curTransFD = open("cur_transID.db", O_RDWR|O_CREAT|O_EXCL, 0744);
	
	int curT_id = 97698;

	lseek(curTransFD, 0, SEEK_SET);

	write(curTransFD, &curT_id, sizeof(int));
	
	return;
}
