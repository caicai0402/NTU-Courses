#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ERR_EXIT(m) {perror(m); exit(1);}
#define BUFFERSIZE 1024

typedef struct client{
    char hostname[BUFFERSIZE];
    int client_fd;
    int port;
    struct sockaddr_in addr;
    char buf[BUFFERSIZE];
    int buflen;
}Client;

void argCheck(int argc, const char *argv[]);
void initClient(Client *client, const char *addr, unsigned short port);

int main(int argc , const char *argv[]){
    argCheck(argc, argv);

    Client client;
    initClient(&client, argv[1], (unsigned short) atoi(argv[2]));

    do{
        fprintf(stdout, "Client: ");
        memset(client.buf, 0, BUFFERSIZE);
        fscanf(stdin, "%s", client.buf);
        send(client.client_fd, client.buf, strlen(client.buf), 0);

        memset(client.buf, 0, BUFFERSIZE);
        recv(client.client_fd, client.buf, BUFFERSIZE, 0);
        fprintf(stdout, "Server: %s\n", client.buf);
        
    }while(strncmp(client.buf, "\xff\xf4", 2) != 0);

    return 0;
}

void argCheck(int argc, const char *argv[]){
    if(argc != 3){
        fprintf(stderr, "usage: %s [addr] [port]\n", argv[0]);
        ERR_EXIT("");
    }
    return ;
}

void initClient(Client *client, const char *addr, unsigned short port){
    gethostname(client->hostname, sizeof(client->hostname));
    
    client->client_fd = socket(AF_INET , SOCK_STREAM , 0);
    if(client->client_fd == -1){
        fprintf(stderr, "socket failed\n");
        ERR_EXIT("");
    }

    bzero(&client->addr, sizeof(client->addr));
    client->addr.sin_family = PF_INET;
    client->addr.sin_addr.s_addr = inet_addr(addr);
    client->port = client->addr.sin_port = htons(port);

    if(connect(client->client_fd, (struct sockaddr *)&client->addr, sizeof(client->addr)) < 0){
        fprintf(stderr, "connect failed\n");
        ERR_EXIT("");
    }

    fprintf(stdout, "Client has connected to server successfully!\n");
    return ;
}