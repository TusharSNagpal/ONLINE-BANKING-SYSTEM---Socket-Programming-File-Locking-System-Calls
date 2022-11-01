# ONLINE-BANKING-SYSTEM---Socket-Programming-File-Locking-System-Calls
#TUSHAR NAGPAL (MT2022125)

        #MINI PROJECT : ONLINE BANKING SYSTEM
        #(using Socket Programming, System Calls, File Locking)
        
PLEASE OPEN manual file in repository and follow the steps written to execute this project in your local machine.

STRUCTURES:


               CUSTOMER
                int cust_id;
                int acc_no;
          char user_name[1000];
           char password[1000];
            char gender[1000];
              char age[1000];



                ACCOUNT
              int accNumber;
            int jointOwners[2];
                int balance;
                bool active;



             TRANSACTION
                int acc_no;
                int cust_id;
                int transID;
               int cur_amount;
            int updated_amount;
           char dateNtime[1000];
               bool deposit;


CLIENT SIDE:
Client will just provide answers to the questions which the server will ask.
All the processing will be done by server.
For Example, 
To create an account server will ask Regular or Joint? Client will reply with his/her choice, say Regular, then the server will ask details like name, age, password, etc.
And then the server will provide account no, customer id, and all the required things to the client with a message that please remember these details for further services.

SERVER SIDE:
All the functionalities are implemented at server side.
Server will take actions on the basis of clients requirements.
It will provide options and perform that specified task.


