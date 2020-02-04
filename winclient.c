#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define MAX 800
#define PORT 12345
#define SA struct sockaddr
#include "cJSON.h"
#include "cJSON.c"
struct sockaddr_in servaddr;
// Function designed for chat between client and server.
void chat(int client_socket)
{
    char buffer[MAX];
    int number, command;
    char username[40];
    char password[40];
    char authtoken[150];
    char channel_name[40];
    char join_channel[40];
    char messages[1000];
    /*client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        printf("Socket creation failed...\n");
        exit(0);
    }*/
    int state = 0;
    while(1)
    {

        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1)
        {
            printf("Socket creation failed...\n");
            exit(0);
        }
        if (state == 0)   // Account menu
        {

            printf("Account Menu:\n1: Register\n2: Login\n");
            scanf("%d", &command);
            if (command == 1)
                state = 1;
            if (command == 2)
                state = 2;
        }
        else if (state == 1)      // Register
        {

            printf("Enter Username: ");
            scanf("%s", &username);
            printf("Enter Password: ");
            scanf("%s", &password);
            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer, "register %s, %s\n", username, password);
            if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
            {
                printf("Connection to the server failed...\n");
                exit(0);
            }
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            closesocket(client_socket);
            cJSON * recieved=cJSON_Parse(buffer);
            printf("%s\n",cJSON_GetObjectItemCaseSensitive(recieved,"type")->valuestring);
            if((strcmp(cJSON_GetObjectItemCaseSensitive(recieved,"type")->valuestring, "Successful")) == 0)
            {
                printf("Account Menu:\n1: Register\n2: Login\n");
                scanf("%d", &command);
                if (command == 1)
                    state = 1;
                if (command == 2)
                    state = 2;
            }
            else
                printf("%s\n", cJSON_GetObjectItemCaseSensitive(recieved,"content")->valuestring);
            // TODO: register
        }
        else if (state == 2)     //login
        {
            printf("Enter Username: ");
            scanf("%s", &username);
            printf("Enter Password: ");
            scanf("%s", &password);
            sprintf(buffer, "login %s, %s\n", username, password);
            /*client_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (client_socket == -1)
            {
                printf("Socket creation failed...\n");
                exit(0);
            }*/
            if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
            {
                printf("Connection to the server failed...\n");
                exit(0);
            }
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            closesocket(client_socket);
            cJSON * recieved=cJSON_Parse(buffer);
            if((strcmp(cJSON_GetObjectItemCaseSensitive(recieved,"type")->valuestring, "AuthToken")) == 0)
            {
                strcpy(authtoken, cJSON_GetObjectItemCaseSensitive(recieved,"content")->valuestring);
                state = 3;
                continue;
            }
            else
                printf("%s\n", cJSON_GetObjectItemCaseSensitive(recieved,"content")->valuestring);
            // TODO: login
        }
        else if (state == 3)      // Main menu
        {
            int a;
            printf("1: Create Channel\n2: Join Channel\n3: Logout\n");
            scanf("%d", &a);
            if(a == 1)
            {
                printf("Enter Name Of Channel:");
                scanf("%s", &channel_name);
                sprintf(buffer, "create channel %s, %s\n", channel_name, authtoken);
                if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
                {
                    printf("Connection to the server failed...\n");
                    exit(0);
                }
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * recieved=cJSON_Parse(buffer);
                if((strcmp(cJSON_GetObjectItemCaseSensitive(recieved,"type")->valuestring, "Successful")) == 0)
                {
                    printf("%s\n",cJSON_GetObjectItemCaseSensitive(recieved,"type")->valuestring);
                    state = 4;
                    continue;
                }
                else
                    printf("%s\n", cJSON_GetObjectItemCaseSensitive(recieved,"content")->valuestring);
                    state = 3;
                    continue;
            }
            else if(a == 2)
            {
                printf("Enter Name Of Channel To Join It:");
                scanf("%s", &join_channel);
                sprintf(buffer, "join channel %s, %s\n", join_channel, authtoken);
                if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
                {
                    printf("Connection to the server failed...\n");
                    exit(0);
                }
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * recieved=cJSON_Parse(buffer);
                if((strcmp(cJSON_GetObjectItemCaseSensitive(recieved,"type")->valuestring, "Successful")) == 0){
                    state = 4;
                    continue;
                }
                else{
                    printf("%s", cJSON_GetObjectItemCaseSensitive(recieved,"content\n")->valuestring);
                    state = 3;
                    continue;
                }
            }
            else if(a == 3)
            {
                sprintf(buffer, "logout %s\n", authtoken);
                if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
                {
                    printf("Connection to the server failed...\n");
                    exit(0);
                }
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * recieved=cJSON_Parse(buffer);
                if((strcmp(cJSON_GetObjectItemCaseSensitive(recieved,"type")->valuestring, "Successful")) == 0)
                {
                    state = 0;
                    continue;
                }
                else
                    printf("%s",cJSON_GetObjectItemCaseSensitive(recieved,"type")->valuestring);
            }
        }
        else if(state == 4)  //Chat Menu
        {
            int b;
            printf(" 1: Send Message\n 2: Refresh\n 3: Channel Members\n 4: Leave Channel\n");
            scanf("%d", &b);
            if(b == 1)
            {
                printf("Enter Your Message:");
                getchar();
                gets(messages);
                sprintf(buffer, "send %s, %s\n", messages, authtoken);
                if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
                {
                    printf("Connection to the server failed...\n");
                    exit(0);
                }
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * recieved=cJSON_Parse(buffer);
                printf("%s\n",cJSON_GetObjectItemCaseSensitive(recieved,"type")->valuestring);
                state = 4;
                continue;
            }
            if(b == 2){
                sprintf(buffer, "refresh %s\n", authtoken);
                if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
                {
                    printf("Connection to the server failed...\n");
                    exit(0);
                }
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * recieved=cJSON_Parse(buffer);
                printf("%s\n",cJSON_Print(cJSON_GetObjectItemCaseSensitive(recieved, "content")));
                continue;
            }
            if(b == 3){
                sprintf(buffer, "channel members %s\n", authtoken);
                if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
                {
                    printf("Connection to the server failed...\n");
                    exit(0);
                }
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * recieved=cJSON_Parse(buffer);
                printf("%s\n",cJSON_Print(cJSON_GetObjectItemCaseSensitive(recieved,"content")));
            }
            if(b == 4){
                 sprintf(buffer, "leave %s\n", authtoken);
                if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
                {
                    printf("Connection to the server failed...\n");
                    exit(0);
                }
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * recieved=cJSON_Parse(buffer);
                if((strcmp(cJSON_GetObjectItemCaseSensitive(recieved,"type")->valuestring, "Successful")) == 0){
                    state = 3;
                    continue;
                }
                else
                    continue;
            }
        }
    }
}

int main()
{
    char sys[]="color BF";
    sys[6]='6';
    sys[7]='d';
    system(sys);

    int client_socket, server_socket;
    struct sockaddr_in cli;

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

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
    /*client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
    {
        printf("Socket successfully created..\n");
    }*/

    // Assign IP and port
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to server socket
    /*if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Connection to the server failed...\n");
        exit(0);
    }
    else
    {
        printf("Successfully connected to the server..\n");
    }*/

    // Function for chat
    chat(client_socket);

    // Close the socket
    closesocket(client_socket);
}
