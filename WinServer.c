#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <dirent.h>
#define MAX 8000
#define PORT 12345
#define SA struct sockaddr
#include "cJSON.h"
#include "cJSON.c"
#include <time.h>

int server_socket, client_socket;
struct sockaddr_in server, client;
char user[1000];
char authtoken[35];
char authtoken_vorodi[35];
char username[100];
char password[100];
char ch_name[100];
char messages [10000];
char payam[10000];
char channel_member[1000];
struct a{
    char user[100];
    char auth[36];
    char name_channel[100];
};
struct a karbar[1000];
struct b{
    char user[100];
    char name_ch[100];
};
struct b channel[1000];

int counter1 = 0, counter2 = 0, number1, number2;

int compare(char a[])
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("resources\\users"))!= NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if(strcmp(ent->d_name,a)==0)
            {
                closedir(dir);
                return 0;
            }
        }
        closedir (dir);
        return 1;
    }
}
int compare_channel(char a[])
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("resources\\channels"))!= NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            if(strcmp(ent->d_name,a)==0)
            {
                closedir(dir);
                return 0;
            }
        }
        closedir (dir);
        return 1;
    }
}


void fun_auth(){
    time_t t;
    srand((unsigned) time(&t));
    for(int i = 0; i < 35; i++){
        int x = (rand() % 26);
        authtoken[i] = (x + 97);
    }
}

int compare_online(char d[])
{
    for(int i = 0; i < 1000; i++){
        if(strcmp(karbar[i].user, d) == 0){
            return 0;
        }
    }
    return 1;
}

int compare_auth(char a[]){
    for(int i = 0; i < 1000; i++){
        if(strcmp(karbar[i].auth, a) == 0){
            return 1;
        }
    }
    return 0;
}


void chat()
{
    char buffer[MAX];
    int n;
    char type[1000];


    while (true)
    {

        memset(buffer, 0, sizeof(buffer));
        server_socket = socket(AF_INET, SOCK_STREAM, 6);
        if (server_socket == INVALID_SOCKET)
            wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
        else
            printf("Socket successfully created..\n");
        if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
            {
            printf("Socket binding failed...\n");
            exit(0);
            }
        else
            printf("Socket successfully bound..\n");
        if ((listen(server_socket, 5)) != 0)
            {
            printf("Listen failed...\n");
            exit(0);
            }
        else
            printf("Server listening..\n");
        int len = sizeof(client);
        client_socket = accept(server_socket, (SA *)&client, &len);
        if (client_socket < 0)
        {
            printf("Server accceptance failed...\n");
            exit(0);
        }
        else
            printf("Server acccepted the client..\n");

        recv(client_socket, buffer, sizeof(buffer), 0);
        sscanf(buffer, "%s", type);

        if(strcmp(type, "register") == 0)
        {
            for(int i = 0; i < 100; i++)
            {
                username[i] = 0;
                password[i] = 0;
            }
            int n = 0;
            for(n; buffer[n + 9] != 44 ; n++)
            {
                 username[n] = buffer[n + 9];
            }
            int i = n + 11;
            for(n = 0; buffer[n + i] != '\n'; n++)
            {
                password[n] = buffer[n + i];
            }
            char address[80];
            sprintf(address,"resources\\users\\%s",username);

            if(compare(username) == 0)
            {
                cJSON *response;
                response = cJSON_CreateObject();
                cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                cJSON_AddItemToObject(response, "content", cJSON_CreateString("The username is not valid"));
                memset(buffer, 0, sizeof(buffer));
                strcpy(buffer, cJSON_PrintUnformatted(response));
                printf("%s",buffer);
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
            }
            else
            {
                cJSON *response;
                response = cJSON_CreateObject();
                cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
                cJSON_AddItemToObject(response, "content", cJSON_CreateString(""));
                memset(buffer, 0, sizeof(buffer));
                strcpy(buffer, cJSON_PrintUnformatted(response));
                printf("%s",buffer);
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                FILE *username1 = fopen(address, "w");
                fprintf(username1, "%s", password);
                fclose(username1);
            }
        }
        else if(strcmp(type, "login") == 0)
        {
            for(int i=0; i<100; i++)
            {
                username[i]=0;
                password[i]=0;
            }
            int n = 0;
            for(n = 0; buffer[n + 6] != 44 ; n++)
            {
                username[n] = buffer[n + 6];
            }
            int i = n + 8;
            for(n = 0; buffer[n + i] != '\n'; n++)
            {
                password[n] = buffer[n + i];
            }
            printf("%s %s\n", username, password);
            if(compare(username) == 1)
            {
                cJSON *response;
                response = cJSON_CreateObject();
                cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                cJSON_AddItemToObject(response, "content", cJSON_CreateString("The username is not exsit"));
                memset(buffer, 0, sizeof(buffer));
                strcpy(buffer, cJSON_PrintUnformatted(response));
                printf("%s",buffer);
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
            }
            else
            {
                char address[80];
                sprintf(address,"resources\\users\\%s",username);
                FILE *use = fopen(address,"r");
                char password2[100];
                fscanf(use,"%s",password2);
                if(strcmp(password2,password)!=0)
                {
                    cJSON *response;
                    response = cJSON_CreateObject();
                    cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                    cJSON_AddItemToObject(response, "content", cJSON_CreateString("Password is incorrect"));
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, cJSON_PrintUnformatted(response));
                    printf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));
                }
                else
                {
                    if(compare_online(username) == 0){
                        cJSON *response;
                        response = cJSON_CreateObject();
                        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                        cJSON_AddItemToObject(response, "content", cJSON_CreateString("you are logged in"));
                        memset(buffer, 0, sizeof(buffer));
                        strcpy(buffer, cJSON_PrintUnformatted(response));
                        printf("%s",buffer);
                        send(client_socket, buffer, sizeof(buffer), 0);
                        memset(buffer, 0, sizeof(buffer));
                    }
                    else{
                    strcpy(user, username);
                    cJSON *response;
                    response = cJSON_CreateObject();
                    fun_auth();
                    cJSON_AddItemToObject(response, "type", cJSON_CreateString("AuthToken"));
                    cJSON_AddItemToObject(response, "content", cJSON_CreateString(authtoken));
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, cJSON_PrintUnformatted(response));
                    printf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(karbar[counter1].user, username);
                    strcat(authtoken,"\0");
                    strcpy(karbar[counter1].auth, authtoken);
                    counter1++;
                    memset(authtoken, 0, sizeof(authtoken));
                    }
                }
            }
        }
        else if(strcmp(type, "create") == 0){
            int n = 0;
            for(n = 0; buffer[n + 15] != 44 ; n++)
                {
                    ch_name[n] = buffer[n + 15];
                }
            int i = n + 17;
            for(n = 0; buffer[n + i] != '\n'; n++)
                {
                    authtoken_vorodi[n] = buffer[n + i];
                }
                if(compare_auth(authtoken_vorodi) == 0){
                    cJSON *response;
                    response = cJSON_CreateObject();
                    cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                    cJSON_AddItemToObject(response, "content", cJSON_CreateString("authtoken is not valid"));
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, cJSON_PrintUnformatted(response));
                    printf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));
                }
                else{
                    if(compare_channel(ch_name) == 0){
                        cJSON *response;
                        response = cJSON_CreateObject();
                        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                        cJSON_AddItemToObject(response, "content", cJSON_CreateString("channel name exist before"));
                        memset(buffer, 0, sizeof(buffer));
                        strcpy(buffer, cJSON_PrintUnformatted(response));
                        printf("%s",buffer);
                        send(client_socket, buffer, sizeof(buffer), 0);
                        memset(buffer, 0, sizeof(buffer));

                    }
                    else{
                        char address[80];
                        memset(address, 0, sizeof(address));
                        sprintf(address,"resources\\channels\\%s",ch_name);
                        cJSON *response;
                        response = cJSON_CreateObject();
                        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
                        cJSON_AddItemToObject(response, "content", cJSON_CreateString(""));
                        memset(buffer, 0, sizeof(buffer));
                        strcpy(buffer, cJSON_PrintUnformatted(response));
                        printf("%s",buffer);
                        send(client_socket, buffer, sizeof(buffer), 0);
                        memset(buffer, 0, sizeof(buffer));
                        char output[1000];
                        cJSON *chap, *content, *car;
                        chap = cJSON_CreateObject();
                        content = cJSON_CreateArray();
                        cJSON_AddItemToObject(chap, "type", cJSON_CreateString(""));
                        cJSON_AddItemToObject(chap, "content", content);

                        //adding a message to the cJSON

                        char message[100];
                        sprintf(message,"channel created by %s",karbar[number1].user);
                        cJSON_AddItemToArray(content, car = cJSON_CreateObject());
                        cJSON_AddItemToObject(car, "sender", cJSON_CreateString("server"));
                        cJSON_AddItemToObject(car, "content", cJSON_CreateString(message));

                        //printing it to the file

                        strcpy(output,cJSON_PrintUnformatted(chap));
                        FILE * fp;
                        fp=fopen(address,"w");
                        fprintf(fp,"%s",output);
                        fclose(fp);

                        FILE *channels = fopen(address, "a");
                        fclose(channels);
                        for(int i = 0; i < 1000; i++){
                            if(strcmp(karbar[i].auth, authtoken_vorodi) == 0){
                                number1 = i;
                                break;
                            }
                        }
                        strcpy(karbar[number1].name_channel, ch_name);
                        strcpy(channel[counter2].name_ch, ch_name);
                        strcpy(channel[counter2].user, karbar[number1].user);
                        counter2++;
                        }
                }
        }
        else if(strcmp(type, "join") == 0){
            memset(ch_name, 0, sizeof(ch_name));
            memset(authtoken_vorodi, 0, sizeof(authtoken_vorodi));
            int n = 0;
            for(n = 0; buffer[n + 13] != 44 ; n++)
                {
                    ch_name[n] = buffer[n + 13];
                }
            int i = n + 15;
            for(n = 0; buffer[n + i] != '\n'; n++)
                {
                    authtoken_vorodi[n] = buffer[n + i];
                }
                if(compare(authtoken_vorodi) == 0){
                    cJSON *response;
                    response = cJSON_CreateObject();
                    cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                    cJSON_AddItemToObject(response, "content", cJSON_CreateString("authtoken is not valid"));
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, cJSON_PrintUnformatted(response));
                    printf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));
                }
                else{
                    if(compare_channel(ch_name) == 1){
                        cJSON *response;
                        response = cJSON_CreateObject();
                        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                        cJSON_AddItemToObject(response, "content", cJSON_CreateString("this channel is not exist"));
                        memset(buffer, 0, sizeof(buffer));
                        strcpy(buffer, cJSON_PrintUnformatted(response));
                        printf("%s",buffer);
                        send(client_socket, buffer, sizeof(buffer), 0);
                        memset(buffer, 0, sizeof(buffer));
                    }
                    else{
                        cJSON *response;
                        response = cJSON_CreateObject();
                        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
                        cJSON_AddItemToObject(response, "content", cJSON_CreateString(""));
                        memset(buffer, 0, sizeof(buffer));
                        strcpy(buffer, cJSON_PrintUnformatted(response));
                        printf("%s",buffer);
                        send(client_socket, buffer, sizeof(buffer), 0);
                        memset(buffer, 0, sizeof(buffer));
                        for(int i = 0; i < 1000; i++){
                            if(strcmp(karbar[i].auth, authtoken_vorodi) == 0){
                                number1 = i;
                                break;
                            }
                        }
                        printf("%s\n",karbar[number1].auth);
                        for(int i = 0; i < 1000; i++){
                            if(strcmp(channel[i].name_ch, ch_name) == 0){
                                number2 = i;
                                break;
                            }
                        }
                        strcpy(channel[number2].user, karbar[number1].user);
                        strcpy(karbar[number1].name_channel, ch_name);
                    }
                }
            }
        else if(strcmp(type, "logout") == 0){
            memset(authtoken_vorodi, 0, sizeof(authtoken_vorodi));
            sscanf(buffer, "%*s %s", authtoken_vorodi);
            if(compare_auth(authtoken_vorodi) == 0){
                cJSON *response;
                response = cJSON_CreateObject();
                cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                cJSON_AddItemToObject(response, "content", cJSON_CreateString("authtoken is not valid"));
                memset(buffer, 0, sizeof(buffer));
                strcpy(buffer, cJSON_PrintUnformatted(response));
                printf("%s",buffer);
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
            }
            else{
                cJSON *response;
                response = cJSON_CreateObject();
                cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
                cJSON_AddItemToObject(response, "content", cJSON_CreateString(""));
                memset(buffer, 0, sizeof(buffer));
                strcpy(buffer, cJSON_PrintUnformatted(response));
                printf("%s",buffer);
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                for(int i = 0; i < 1000; i++){
                    if(strcmp(karbar[i].auth, authtoken_vorodi) == 0){
                        number1 = i;
                        break;
                    }
                }
                strcpy(karbar[number1].user,"\0");
                strcpy(karbar[number1].auth,"\0");
                for(int i = number1; i < 999; i++){
                    strcpy(karbar[i].user, karbar[i+1].user);
                    strcpy(karbar[i].auth, karbar[i+1].auth);
                }
            }
    }
        else if(strcmp(type, "send") == 0){
            memset(authtoken_vorodi, 0, sizeof(authtoken_vorodi));
            int n = 0;
            for(n = 0; buffer[n + 5] != 44 ; n++)
                {
                    messages[n] = buffer[n + 5];
                }
                int i=n+7;
            for(n=0; buffer[n+i] !='\n' ; n++)
                {
                    authtoken_vorodi[n] = buffer[n+i];
                }
            for(int i = 0; i < 1000; i++){
                if(strcmp(karbar[i].auth, authtoken_vorodi) == 0){
                    number1 = i;
                }
            }
            printf("%s\n", karbar[number1].auth);
            printf("%s\n", authtoken_vorodi);
            if(compare_auth(authtoken_vorodi) == 0){
                cJSON *response;
                response = cJSON_CreateObject();
                cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                cJSON_AddItemToObject(response, "content", cJSON_CreateString("authtoken is not valid"));
                memset(buffer, 0, sizeof(buffer));
                strcpy(buffer, cJSON_PrintUnformatted(response));
                printf("%s",buffer);
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
            }
            else{
                char address[80], cJ[1000];
                sprintf(address,"resources\\channels\\%s", karbar[number1].name_channel);
                cJSON *response;
                response = cJSON_CreateObject();
                cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
                cJSON_AddItemToObject(response, "content", cJSON_CreateString(""));
                memset(buffer, 0, sizeof(buffer));
                strcpy(buffer, cJSON_PrintUnformatted(response));
                printf("%s\n",buffer);
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                char out[1000];
                FILE * fp;
                cJSON * rec, * content, * car;
                fp=fopen(address,"r");
                fgets(cJ,1000,fp);
                rec=cJSON_Parse(cJ);
                fclose(fp);
                content=cJSON_GetObjectItem(rec,"content");
                cJSON_AddItemToArray(content, car = cJSON_CreateObject());
                cJSON_AddItemToObject(car, "sender", cJSON_CreateString(karbar[number1].user));
                cJSON_AddItemToObject(car, "content", cJSON_CreateString(messages));
                char output[1000];
                strcpy(output,cJSON_PrintUnformatted(rec));
                fp=fopen(address,"w");
                fprintf(fp,"%s",output);
                fclose(fp);

        }
        }
        else if(strcmp(type, "refresh") == 0){
            memset(authtoken_vorodi, 0, sizeof(authtoken_vorodi));
            int n = 0;
            sscanf(buffer,"%*s %s", authtoken_vorodi);
                if(compare_auth(authtoken_vorodi) == 0){
                    cJSON *response;
                    response = cJSON_CreateObject();
                    cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                    cJSON_AddItemToObject(response, "content", cJSON_CreateString("authtoken is not valid"));
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, cJSON_PrintUnformatted(response));
                    printf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));
                }
                else{
                    for(int i = 0; i < 1000; i++){
                        if(strcmp(karbar[i].auth, authtoken_vorodi) == 0){
                            number1 = i;
                        }
                    }
                    FILE * fp, *gp;
                    char address1[1000], address2[1000], payam[1000];
                    sprintf(address1,"resources\\channels\\%s",karbar[number1].name_channel);
                    sprintf(address2,"resources\\refresh\\%s-%s",karbar[number1].name_channel, karbar[number1].user);
                    fp=fopen(address2,"r");
                    gp=fopen(address1,"r");
                    if(fp == NULL){
                        fgets(buffer, 1000, gp);
                        send(client_socket, buffer, sizeof(buffer), 0);
                        cJSON * rec = cJSON_Parse(buffer);
                        cJSON * array = cJSON_GetObjectItem(rec,"content");
                        int length=cJSON_GetArraySize(array);
                        FILE * FF;
                        FF=fopen(address2,"w");
                        fprintf(FF,"%d",length);
                        printf("the length %d",length);
                        fclose(FF);
                        memset(buffer, 0, sizeof(buffer));
                    }
                    else{
                        int seen ;
                        char output[1000];
                        fgets(buffer, 1000, gp);
                        cJSON *chap, *content, *car;
                        chap = cJSON_CreateObject();
                        content = cJSON_CreateArray();
                        cJSON_AddItemToObject(chap, "type", cJSON_CreateString(""));
                        cJSON_AddItemToObject(chap, "content", content);
                        fscanf(fp,"%d",&seen);
                        printf("%d\n", seen);
                        cJSON * rec = cJSON_Parse(buffer);
                        cJSON * array = cJSON_GetObjectItem(rec,"content");
                        int length=cJSON_GetArraySize(array);
                        printf("the length is %d, however the seen is %d\n",length,seen);
                        for(int i = seen; i < length; i++){
                            cJSON * array1 = cJSON_GetArrayItem(array,i);
                            char sender[100],sms[1000];
                            strcpy(sender,cJSON_GetObjectItem(array1,"sender")->valuestring);
                            strcpy(sms,cJSON_GetObjectItem(array1,"content")->valuestring);
                            cJSON_AddItemToArray(content, car = cJSON_CreateObject());
                            cJSON_AddItemToObject(car, "sender", cJSON_CreateString(sender));
                            cJSON_AddItemToObject(car, "content", cJSON_CreateString(sms));
                        }
                        strcpy(buffer,cJSON_PrintUnformatted(chap));
                        send(client_socket, buffer, sizeof(buffer), 0);
                        memset(buffer, 0, sizeof(buffer));
                        fclose(fp);
                        FILE * FF;
                        FF=fopen(address2,"w");
                        fprintf(FF,"%d",length);
                        printf("the length %d",length);
                        fclose(FF);
                    }
                    }

        }
        else if(strcmp(type, "channel") == 0){
            memset(authtoken_vorodi, 0, sizeof(authtoken_vorodi));
            sscanf(buffer,"%*s %*s %s", authtoken_vorodi);
                if(compare_auth(authtoken_vorodi) == 0){
                    cJSON *response;
                    response = cJSON_CreateObject();
                    cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                    cJSON_AddItemToObject(response, "content", cJSON_CreateString("authtoken is not valid"));
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, cJSON_PrintUnformatted(response));
                    printf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));
                }
                else{
                    for(int i = 0; i < 1000; i++){
                        if(strcmp(karbar[i].auth, authtoken_vorodi) == 0){
                            number1 = i;
                        }
                    }
                    cJSON *response;
                    cJSON *ch_m;
                    response = cJSON_CreateObject();
                    ch_m = cJSON_CreateArray();
                    cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
                    cJSON_AddItemToObject(response, "content", ch_m);
                    for(int i = 0; i < 1000; i++){
                        if(strcmp(karbar[number1].name_channel, karbar[i].name_channel) == 0){
                            cJSON_AddItemToObject(ch_m, "content", cJSON_CreateString(karbar[i].user));
                        }
                    }
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, cJSON_PrintUnformatted(response));
                    printf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));

                }
        }
        else if(strcmp(type, "leave") == 0){
            memset(authtoken_vorodi, 0, sizeof(authtoken_vorodi));
            sscanf(buffer,"%*s %s", authtoken_vorodi);
                if(compare_auth(authtoken_vorodi) == 0){
                    cJSON *response;
                    response = cJSON_CreateObject();
                    cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
                    cJSON_AddItemToObject(response, "content", cJSON_CreateString("authtoken is not valid"));
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, cJSON_PrintUnformatted(response));
                    printf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));
                }
                else{
                    cJSON *response;
                    response = cJSON_CreateObject();
                    cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
                    cJSON_AddItemToObject(response, "content", cJSON_CreateString(""));
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, cJSON_PrintUnformatted(response));
                    printf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));
                    for(int i = 0; i < 1000; i++){
                        if(strcmp(karbar[i].auth, authtoken_vorodi) == 0){
                            number1 = i;
                            break;
                        }
                    }
                    strcpy(karbar[number1].name_channel, "\0");
                }
        }

        closesocket(server_socket);

        /*server_socket = socket(AF_INET, SOCK_STREAM, 6);
        if (server_socket == INVALID_SOCKET)
            wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
        else
            printf("Socket successfully created..\n");

        if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
        {
            printf("Socket binding failed...\n");
            exit(0);
        }
        else
            printf("Socket successfully bound..\n");

        // Now server is ready to listen and verify
        if ((listen(server_socket, 5)) != 0)
        {
            printf("Listen failed...\n");
            exit(0);
        }
        else
            printf("Server listening..\n");



        // If the message starts with "exit" then server exits and chat ends
        if (strncmp("exit", buffer, 4) == 0)
        {
            printf("Server stopping...\n");*/



    }
}

int main()
{
    for(int i = 0; i < 1000; i++){
        strcpy(karbar[i].user, "\0");
        strcpy(karbar[i].auth, "\0");
        strcpy(karbar[i].name_channel, "\0");
        strcpy(channel[i].name_ch, "\0");
        strcpy(channel[i].user, "\0");
    }

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    mkdir("resources");
    mkdir("resources\\users");
    mkdir("resources\\channels");
    mkdir("resources\\refresh");
    // Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Create and verify socket
    /*server_socket = socket(AF_INET, SOCK_STREAM, 6);
    if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");*/

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    /*if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully bound..\n");*/

    // Now server is ready to listen and verify
    /*if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");*/

    // Accept the data packet from client and verify
    //int len = sizeof(client);
    /*client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        exit(0);
    }
    else
        printf("Server acccepted the client..\n");
*/
    // Function for chatting between client and server
    chat();

    // Close the socket
    closesocket(server_socket);

    return 0;
}

