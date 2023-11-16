#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/**
 * @function createFolerStorge: check folder which is used to storge received data from client to server exist?
 *                              if not exist yet -> create new folder
 * @param folder: name of folder need to create to storge
*/
void createFolerStorage(char *folder){
    if(access(folder, F_OK) != -1){
        printf("Server started\n");
    }else{
        if(mkdir(folder, 0777) == 0){
            printf("Server started\n");
        }else{
            printf("Make folder error!!\n");
            exit(1);
        }
    }
}

/**
 * @function breakString : break the string with spaces
 * 
 * @param string : string to break
 * @param str : storaged string
 */
void breakString(char *string, char *str[]){
    int i = 0;
    char *token = strtok(string," ");
    while(token != NULL){
        str[i] = malloc(strlen(token) + 1);
        strcpy(str[i], token);
        i++;
        token = strtok(NULL, " ");
    }
}

/**
 * @function saveLog: Record activity log with format [dd/mm/yyyy hh:mm:ss] $ ip:port $ string_input $ status
 * 
 * @param choose: selection of activity
 * @param string_input: User-provided value
 * @param status: result of activity
*/
void saveLog(char *ip, char *port, char *string_input, char *status){
   time_t rawtime; //Change to Local Timezone
   struct tm *info;
   char log[1024];
   time( &rawtime ); //Get current time and save to rawtime
   info = localtime( &rawtime ); //Convert rawtime to local time information
   strftime(log,80,"%d/%m/%Y %X", info); //Save to string

    FILE *fl;
    fl = fopen("log_20204990.txt", "a+");

    if(fl == NULL){
        printf("Error opening file log_20204990.txt\n");
        exit(1);
    }

    fprintf(fl, "[%s] $ %s : %s $ %s $ %s\n", log, ip, port, string_input, status);

    fclose(fl);
}