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

typedef struct server{
    char hostname[BUFFERSIZE];
    int port;
    int server_fd;
    struct sockaddr_in addr;
    char buf[BUFFERSIZE];
    int buflen;
}Server;

typedef struct client{
    char hostname[BUFFERSIZE];
    int client_fd;
    char buf[BUFFERSIZE];
    int buflen;
}Client;

typedef struct multiplexer{
    int maxfd;
    struct pollfd *fds;
    nfds_t nfds;
}Multiplexer;

void argCheck(int argc, const char *argv[]);
void initServer(Server *server, unsigned short port);
void initMultiplexer(Multiplexer *multiplexer, Server *server);
void initClients(Client **clients, int maxfd);
void newConnection(Server *server, Client *clients, Multiplexer *multiplexer);
void endConnection(Client *clients, Multiplexer *multiplexer, int idx);
int receiveRequest(Client *client);
int sendResponse(Server *server, Client *client);
void done(Server *server, Multiplexer *multiplexer, Client *clients);

int main(int argc, const char*argv[]){
    argCheck(argc, argv);

    Server server;
    initServer(&server, (unsigned short) atoi(argv[1]));

    Multiplexer multiplexer;
    initMultiplexer(&multiplexer, &server);
    
    Client *clients;
    initClients(&clients, multiplexer.maxfd);

    while(1){
        if(poll(multiplexer.fds, multiplexer.nfds, -1) < 0){
            fprintf(stderr, "poll failed\n");
            ERR_EXIT("");
        }
        
        for(int i = 1; i < multiplexer.nfds; i++){
            if(multiplexer.fds[i].revents == POLLIN){
                if(receiveRequest(&clients[multiplexer.fds[i].fd]) == 0)
                    endConnection(clients, &multiplexer, i);
                else if(sendResponse(&server, &clients[multiplexer.fds[i].fd]))
                    endConnection(clients, &multiplexer, i);
            }
        }

        if(multiplexer.fds[0].revents == POLLIN)
            newConnection(&server, clients, &multiplexer);
    }
    done(&server, &multiplexer, clients);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void argCheck(int argc, const char *argv[]){
    if(argc != 2){
        fprintf(stderr, "usage: %s [port]\n", argv[0]);
        ERR_EXIT("");
    }
    return ;
}

void initServer(Server *server, unsigned short port){
    gethostname(server->hostname, sizeof(server->hostname));
    server->port = port;

    server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server->server_fd == -1){
        fprintf(stderr, "socket failed\n");
        ERR_EXIT("");
    }
    
    bzero(&server->addr, sizeof(server->addr));
    server->addr.sin_family = AF_INET;
    server->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server->addr.sin_port = htons(port);
    if(bind(server->server_fd, (struct sockaddr *)&server->addr, sizeof(server->addr)) < 0){
        fprintf(stderr, "bind failed\n");
        ERR_EXIT("");
    }

    if(listen(server->server_fd, 1024) < 0){
        fprintf(stderr, "listen failed\n");
        ERR_EXIT("");
    }

    fprintf(stdout, "Server initialize successfully!\n");
    fprintf(stdout, "Starting listening on %s, port %d...\n", server->hostname, server->port);
    return ;
}

void initMultiplexer(Multiplexer *multiplexer, Server *server){
    multiplexer->maxfd = getdtablesize();
    multiplexer->fds = (struct pollfd *)malloc(sizeof(struct pollfd) * multiplexer->maxfd);
    multiplexer->fds[0].fd = server->server_fd;
    multiplexer->fds[0].events = POLLIN;
    multiplexer->nfds = 1;
    return ;
}

void initClients(Client **clients, int maxfd){
    *clients = (Client *)malloc(sizeof(Client) * maxfd);
    if(*clients == NULL){
        fprintf(stderr, "out of memory allocating all requests\n");
        ERR_EXIT("");
    }
    memset(*clients, 0, sizeof(Client) * maxfd);
    return ;
}

void newConnection(Server *server, Client *clients, Multiplexer *multiplexer){
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    int client_fd = accept(server->server_fd, (struct sockaddr *)&addr, (socklen_t *)&addrlen);
    if(client_fd < 0){
        if(errno == EINTR || errno == EAGAIN)
            return ;
        else if(errno == ENFILE){
            fprintf(stderr, "out of file descriptor table ... (maxconn %d), accept fail\n", multiplexer->maxfd);
            return ;
        }
        ERR_EXIT("");
    }
    else{
        clients[client_fd].client_fd = client_fd;
        strcpy(clients[client_fd].hostname, inet_ntoa(addr.sin_addr));
        multiplexer->fds[multiplexer->nfds].fd = client_fd;
        multiplexer->fds[multiplexer->nfds].events = POLLIN | POLLHUP | POLLERR;
        multiplexer->nfds ++;
        fprintf(stdout, "Getting a new request... fd %d from %s\n", client_fd, clients[client_fd].hostname);
    }
    return ;
}

void endConnection(Client *clients, Multiplexer *multiplexer, int idx){
    int fd = multiplexer->fds[idx].fd;
    multiplexer->fds[idx] = multiplexer->fds[multiplexer->nfds - 1];
    multiplexer->nfds --;
    close(fd);
    fprintf(stdout, "client %s, fd: %d, has been closed\n", clients[fd].hostname, fd);
    memset(clients[fd].hostname, 0, BUFFERSIZE);
    memset(clients[fd].buf, 0, BUFFERSIZE);
    return ;
}

int receiveRequest(Client *client){
    memset(client->buf, 0, BUFFERSIZE);
    int r = read(client->client_fd, client->buf, BUFFERSIZE);
    fprintf(stdout, "************** fd: %d, length: %d **************\n%s\n******************** end *********************\n", client->client_fd, r, client->buf);
    if(r < 0)
        return -1;
    else if(r == 0 || strncmp(client->buf, "\xff\xf4", 2) == 0) 
        return 0;
    else
        return 1;
}

int sendResponse(Server *server, Client *client){
    if(strncmp(client->buf, "GET", 3) == 0){
        char header[] = "HTTP/1.0 200 OK\r\n"
                        "Content-type: text/html\r\n\r\n"
                        "<html><body><h1>Cai-Yi Hu</h1><h2>CSIE B08208032</h2></body></html>\r\n";

        if(write(client->client_fd, header, sizeof(header)) < 0){
            fprintf(stderr, "Sending header failed\n");
            ERR_EXIT("");
        }
        return 1;
    }
    else{
        if(write(client->client_fd, client->buf, BUFFERSIZE) < 0){
            fprintf(stderr, "Echo failed\n");
            ERR_EXIT("");
        }
    }
    return 0;
}

void done(Server *server, Multiplexer *multiplexer, Client *clients){
    for(int i = 0; i < multiplexer->nfds; i++)
        close(multiplexer->fds[i].fd);
    close(server->server_fd);
    free(multiplexer->fds);
    free(clients);
    return ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////