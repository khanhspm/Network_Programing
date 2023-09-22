#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

struct User{
    char username[255];
    int banned;
};
typedef struct User user;

struct Node{
	user data;
	struct Node *next;
};
typedef struct Node* node;

/**
 * @function Empty: checking list of nodes is NULL
 * 
 * @param list: list of nodes to check
 * 
 * @return: empty list
*/
int Empty(node list){
	return list == NULL;
}

void addnew(node* head, user x){
    struct Node* p = (struct Node*)malloc(sizeof(struct Node));
    p->data = x;
    p->next = NULL;
    if(Empty(*head)){
        *head = p;
    }
}

/**
 * @function Add: add a node to the list
 * 
 * @param head: head of list
 * @param x: node to add
*/
void Add(node* head, user x){
    if(Empty(*head)){
        addnew(head, x);
    }else{
        struct Node* a = *head;
        while(a->next != NULL){
            a = a->next;
        }
        struct Node* p = (struct Node*)malloc(sizeof(struct Node));
        p->data = x;
        p->next = NULL;
        a->next = p;
    }
}

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
	if(Empty(head)){
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

void log_acctions(int choose, char string_input[255], char status[5]){
    time_t rawtime;
   struct tm *info;
   char buffer[20];
   time( &rawtime );
   info = localtime( &rawtime );
   strftime(buffer,80,"%d/%m/%Y %X", info);

    FILE *fl;
    fl = fopen("log_20204990.txt", "a+");
    if(fl == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    fprintf(fl, "[%s] $ %d $ %s $ %s\n", buffer, choose, string_input, status);
    fclose(fl);
}

int main(){
    node l = NULL;
    user x;
    char str[255];
    FILE *fp;
    fp = fopen("account.txt", "r");

    if(fp == NULL){
        printf("Can not open file account.txt!!!");
        exit(1);
    }
    
    while(fgets(str, 255, fp) != NULL){
        if(sscanf(str, "%s %d", x.username, &x.banned) == 2){
        	Add(&l, x);
		}
        
    }
    fclose(fp);

    int choose;
    int login_status = 0;
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
                char status[] = "-ERR";
                printf("You have already logged in\n\n");
                log_acctions(choose, username, status);
                break;
            }
            if(login_status == 0){
                if(checkLogin(l, username) == 0){
                    char status[] = "-ERR";
                    printf("Account is not exist\n\n");
                    log_acctions(choose, username, status);
                    break;
                }
                if(checkLogin(l, username) == 1){
                    char status[] = "-ERR";
                    printf("Account is banned\n\n");
                    log_acctions(choose, username, status);
                    break;
                }
                if(checkLogin(l, username) == 2){
                    login_status=1;
                    char status[] = "+OK";
                    printf("Hello %s\n\n", username);
                    log_acctions(choose, username, status);
                    break;
                }
            }
        }

        case 2:{
            char message[255];
			printf("Message: ");
            fflush(stdin);
            fgets(message, 255, stdin);
            size_t len = strlen(message);
            if (len > 0 && message[len - 1] == '\n') {
                message[len - 1] = '\0';
            }
            printf("\n");
            if(login_status == 0){
                char status[] = "-ERR";
                printf("You have not logged in yet\n\n");
                log_acctions(choose, message, status);
                break;
            }
            if(login_status == 1){
                char status[] = "+OK";
                printf("Successful post\n\n");
                log_acctions(choose, message, status);
                break;
            }
        }
        case 3:{
            if(login_status == 1){
                login_status--;
                char status[] = "+OK";
                printf("Successful logged out\n\n");
                log_acctions(choose,"", status);
                break;
            }
            if(login_status == 0){
                char status[] = "-ERR";
                printf("You have not logged in yet\n\n");
                log_acctions(choose, "", status);
                break;
            }
        }

        case 4:
            break;

        default:{
            printf("You only can choose 1 or 2 or 3 or 4!!!!!\n\n");
            break;
        }
        }
    } while (choose != 4);
    
    return 0;
}