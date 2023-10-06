#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"liblist.h"

/**
 * @function checkLogin: checking account exist and is bannedned.
 * 
 * @param username: A input string.
 * @param account: A struct contains all of information about account.
 *  
 * @return: 0 if account not exist.
 *          1 if account exist and is bannedned.
 *          2 if account exist and is not bannedned.
*/
int checkLogin(node head, char username[]){
    int check = 0;
	if(checkEmptyList(head)){
        return 0;
    }else{
        struct Node* p = head;
        while(p != NULL){
            if(strcmp(username, p->data.username) == 0){
                if(p->data.banned == 0){
                    check = 1;
                }
                if(p->data.banned == 1){
                    check = 2;
                }
            }
            p = p->next;
        }
    }

    return check;
}

/**
 * @function saveLog: Record activity log with format [dd/mm/yyyy hh:mm:ss] $ choose $ string_input $ status
 * 
 * @param choose: selection of activity
 * @param string_input: User-provided value
 * @param status: result of activity
*/
void saveLog(int choose, char string_input[255], char status[5]){
    time_t rawtime; //Change to Local Timezone
   struct tm *info;
   char buffer[20];
   time( &rawtime ); //Get current time and save to rawtime
   info = localtime( &rawtime ); //Convert rawtime to local time information
   strftime(buffer,80,"%d/%m/%Y %X", info); //Save to string

    FILE *fl;
    fl = fopen("log_20204990.txt", "a+");

    if(fl == NULL){
        printf("Error opening file log_20204990.txt\n");
        exit(1);
    }

    fprintf(fl, "[%s] $ %d $ %s $ %s\n", buffer, choose, string_input, status);

    fclose(fl);
}

/**
 * @function display: display the menu and notification to screen
 * 
 */
void display(){
    node l = NULL; // initialize the node
    user x; // initialize the user variable
    char str[255]; // initialize the string variable to read file
    FILE *fp;
    fp = fopen("account.txt", "r");

    if(fp == NULL){
        printf("Error opening file account.txt!!!\n");
        exit(1);
    }
    
    while(fgets(str, 255, fp) != NULL){
        if(sscanf(str, "%s %d", x.username, &x.banned) == 2){
        	addNode(&l, x);
		}
        
    }

    fclose(fp);

    int choose; // initialize variables to select the action
    int login_status = 0; //initialize variables for login status
    char err[] = "-ERR";
    char oke[] = "+OK";
    do
    {   
    	printf("1. Login\n");
    	printf("2. Post message\n");
    	printf("3. Logout\n");
    	printf("4. Exit\n\n");
        scanf("%d", &choose);

        switch (choose)
        {
        case 1:{
        	printf("Username: ");
            char username[255];
            scanf("%s", username);
            printf("\n");
            if(login_status == 1){
                printf("You have already logged in\n\n");
                saveLog(choose, username, err);
                break;
            }
            if(login_status == 0){
                if(checkLogin(l, username) == 0){
                    printf("Account is not exist\n\n");
                    saveLog(choose, username, err);
                    break;
                }
                if(checkLogin(l, username) == 1){
                    printf("Account is banned\n\n");
                    saveLog(choose, username, err);
                    break;
                }
                if(checkLogin(l, username) == 2){
                    login_status=1;
                    printf("Hello %s\n\n", username);
                    saveLog(choose, username, oke);
                    break;
                }
            }
        }

        case 2:{
            char message[255];
			printf("Message: ");
            fflush(stdin); // flush std out before reading string from keyboard
            fgets(message, 255, stdin); //get message
            size_t len = strlen(message);
            //delete character '\n' from message
            if (len > 0 && message[len - 1] == '\n') {
                message[len - 1] = '\0';
            }
            printf("\n");
            if(login_status == 0){
                printf("You have not logged.\n\n");
                saveLog(choose, message, err);
                break;
            }
            if(login_status == 1){
                printf("Successful post\n\n");
                saveLog(choose, message, oke);
                break;
            }
        }
        case 3:{
            if(login_status == 1){
                login_status--;
                printf("Successful logged out\n\n");
                saveLog(choose,"", oke);
                break;
            }
            if(login_status == 0){
                printf("You have not logged.\n\n");
                saveLog(choose, "", err);
                break;
            }
        }

        case 4:
            break;

        }

    } while (choose != 4);
}