#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define ERR_EXIT(a) do { perror(a); exit(1); } while(0)

#define OBJ_NUM 3

#define FOOD_INDEX 0
#define CONCERT_INDEX 1
#define ELECS_INDEX 2
#define RECORD_PATH "./bookingRecord"

static char* obj_names[OBJ_NUM] = {"Food", "Concert", "Electronics"};

typedef struct {
    char hostname[512];  // server's hostname
    unsigned short port;  // port to listen
    int listen_fd;  // fd to wait for a new connection
} server;

typedef struct {
    char host[512];  // client's host
    int conn_fd;  // fd to talk with client
    char buf[512];  // data sent by/to client
    size_t buf_len;  // bytes used by buf
    int id;
    int wait_for_write;  // used by handle_read to know if the header is read or not.
} request;

server svr;  // server
request* requestP = NULL;  // point to a list of requests
int maxfd;  // size of open file descriptor table, size of request list

const char* accept_read_header = "ACCEPT_FROM_READ";
const char* accept_write_header = "ACCEPT_FROM_WRITE";
const unsigned char IAC_IP[3] = "\xff\xf4";

static void init_server(unsigned short port);
// initailize a server, exit for error

static void init_request(request* reqP);
// initailize a request instance

static void free_request(request* reqP);
// free resources used by a request instance

typedef struct {
    int id;          // 902001-902020
    int bookingState[OBJ_NUM]; // 1 means booked, 0 means not.
}record;

int handle_read(request* reqP) {
    /*  Return value:
     *      1: read successfully
     *      0: read EOF (client down)
     *     -1: read failed
     */
    int r;
    char buf[512] = {0};

    // Read in request from client
    r = read(reqP->conn_fd, buf, sizeof(buf));
    if (r < 0) return -1;
    if (r == 0) return 0;
    char* p1 = strstr(buf, "\015\012");
    int newline_len = 2;
    if (p1 == NULL) {
        p1 = strstr(buf, "\012");
        if (p1 == NULL) {
            if (!strncmp(buf, IAC_IP, 2)) {
                // Client presses ctrl+C, regard as disconnection
                
                return 0;
            }
            ERR_EXIT("this really should not happen...");
        }
    }
    size_t len = p1 - buf + 1;
    memmove(reqP->buf, buf, len);
    reqP->buf[len - 1] = '\0';
    reqP->buf_len = len-1;
    return 1;
}

void setLock(struct flock *lock, short l_type, short l_whence, off_t l_start, off_t l_len){
    lock->l_type = l_type;
    lock->l_whence = l_whence;
    lock->l_start = l_start;
    lock->l_len = l_len;
    return ;
}

int parse(char s[]){
    int len = strlen(s);
    if((s[0] != '-' && !isdigit(s[0])) || len > 6)
        return -99999999;
    for(int i = 1; i < len; i++)
        if(!isdigit(s[i]))
            return -99999999;
    return atoi(s);
}

int main(int argc, char** argv) {

    // Parse args.
    if (argc != 2) {
        fprintf(stderr, "usage: %s [port]\n", argv[0]);
        exit(0);
    }

    struct sockaddr_in cliaddr;  // used by accept()
    int clilen;

    int conn_fd;  // fd for a new connection with client
    int file_fd = open(RECORD_PATH, O_RDWR);  // fd for file that we open for reading, file_fd = 3
    char buf[512];
    int buf_len;

    // Initialize server
    init_server((unsigned short) atoi(argv[1]));

    // Loop for handling connections
    fprintf(stderr, "\nstarting on %.80s, port %d, fd %d, maxconn %d...\n", svr.hostname, svr.port, svr.listen_fd, maxfd);

    // CaiCai 
    struct pollfd fds[maxfd];
    nfds_t nfds = 1;
    fds[0].fd = svr.listen_fd;
    fds[0].events = POLLIN;
    int isLocked[20] = {0};
    struct flock lock;
   
    while (1) {
        // TODO: Add IO multiplexing
        fprintf(stderr, "available poll: %d\n", poll(fds, nfds, -1));
        
        for(int i = 1; i < nfds; i++){
            if(fds[i].revents & POLLIN){
                int ret = handle_read(&requestP[fds[i].fd]);
                if (ret < 0) {
                    fprintf(stderr, "bad request from %s\n", requestP[fds[i].fd].host);
                    continue;
                }
                else if(ret == 0){
                    fprintf(stderr, "Client %s is down\n", requestP[fds[i].fd].host);
                    if(requestP[fds[i].fd].id != 0){
                        if(isLocked[requestP[fds[i].fd].id - 902001] == 1){
                            setLock(&lock, F_UNLCK, SEEK_SET, sizeof(record) * (requestP[fds[i].fd].id - 902001), sizeof(record));
                            fcntl(file_fd, F_SETLK, &lock);
                        }
                        isLocked[requestP[fds[i].fd].id - 902001] --;
                    }
                    close(requestP[fds[i].fd].conn_fd);
                    free_request(&requestP[fds[i].fd]);
                    fds[i--] = fds[--nfds];
                    continue;
                }
                // TODO: handle requests from clients
#ifdef READ_SERVER  
                if(requestP[fds[i].fd].id == 0){
                    int id = parse(requestP[fds[i].fd].buf);
                    if( (id < 902001 || 902020 < id)){
                        write(requestP[fds[i].fd].conn_fd, "[Error] Operation failed. Please try again.\n", 44);
                        close(requestP[fds[i].fd].conn_fd);
                        free_request(&requestP[fds[i].fd]);
                        fds[i--] = fds[--nfds];
                    }
                    else{
                        requestP[fds[i].fd].id = id;
                        setLock(&lock, F_RDLCK, SEEK_SET, sizeof(record) * (requestP[fds[i].fd].id - 902001), sizeof(record));
                        if(fcntl(file_fd, F_SETLK, &lock) == -1){
                            write(requestP[fds[i].fd].conn_fd, "Locked.\n", 8);
                            close(requestP[fds[i].fd].conn_fd);
                            free_request(&requestP[fds[i].fd]);
                            fds[i--] = fds[--nfds];
                        }
                        else{
                            record data;
                            lseek(file_fd, (requestP[fds[i].fd].id - 902001) * sizeof(record), SEEK_SET);
                            read(file_fd, &data, sizeof(record));
                            sprintf(requestP[fds[i].fd].buf, "Food: %d booked\nConcert: %d booked\nElectronics: %d booked\n\n(Type Exit to leave...)\n", data.bookingState[FOOD_INDEX], data.bookingState[CONCERT_INDEX], data.bookingState[ELECS_INDEX]);
                            requestP[fds[i].fd].buf_len = strlen(requestP[fds[i].fd].buf);
                            write(requestP[fds[i].fd].conn_fd, requestP[fds[i].fd].buf, requestP[fds[i].fd].buf_len);
                            isLocked[requestP[fds[i].fd].id - 902001] ++;
                        }
                    }
                }
                else if(strcmp(requestP[fds[i].fd].buf, "Exit") == 0){
                    if(isLocked[requestP[fds[i].fd].id - 902001] == 1){
                        setLock(&lock, F_UNLCK, SEEK_SET, sizeof(record) * (requestP[fds[i].fd].id - 902001), sizeof(record));
                        fcntl(file_fd, F_SETLK, &lock);
                    }
                    isLocked[requestP[fds[i].fd].id - 902001] --;
                    close(requestP[fds[i].fd].conn_fd);
                    free_request(&requestP[fds[i].fd]);
                    fds[i--] = fds[--nfds];
                }
                
#elif defined WRITE_SERVER
                if(requestP[fds[i].fd].id == 0){
                    int id = parse(requestP[fds[i].fd].buf);
                    if( (id < 902001 || 902020 < id)){
                        write(requestP[fds[i].fd].conn_fd, "[Error] Operation failed. Please try again.\n", 44);
                        close(requestP[fds[i].fd].conn_fd);
                        free_request(&requestP[fds[i].fd]);
                        fds[i--] = fds[--nfds];
                    }
                    else{
                        requestP[fds[i].fd].id = id;
                        setLock(&lock, F_WRLCK, SEEK_SET, sizeof(record) * (requestP[fds[i].fd].id - 902001), sizeof(record));
                        if(fcntl(file_fd, F_SETLK, &lock) == -1 || isLocked[requestP[fds[i].fd].id - 902001] == 1){
                            write(requestP[fds[i].fd].conn_fd, "Locked.\n", 8);
                            close(requestP[fds[i].fd].conn_fd);
                            free_request(&requestP[fds[i].fd]);
                            fds[i--] = fds[--nfds];
                        }
                        else{
                            record data;
                            lseek(file_fd, (requestP[fds[i].fd].id - 902001) * sizeof(record), SEEK_SET);
                            read(file_fd, &data, sizeof(record));
                            sprintf(requestP[fds[i].fd].buf, "Food: %d booked\nConcert: %d booked\nElectronics: %d booked\n\nPlease input your booking command. (Food, Concert, Electronics. Positive/negative value increases/decreases the booking amount.):\n", data.bookingState[FOOD_INDEX], data.bookingState[CONCERT_INDEX], data.bookingState[ELECS_INDEX]);
                            requestP[fds[i].fd].buf_len = strlen(requestP[fds[i].fd].buf);
                            write(requestP[fds[i].fd].conn_fd, requestP[fds[i].fd].buf, requestP[fds[i].fd].buf_len);
                            isLocked[requestP[fds[i].fd].id - 902001] = 1;
                        }
                    }
                }
                else{
                    record data, input;
                    char tmp[3][33];
                    lseek(file_fd, (requestP[fds[i].fd].id - 902001) * sizeof(record), SEEK_SET);
                    read(file_fd, &data, sizeof(record));
                    sscanf(requestP[fds[i].fd].buf, "%s %s %s", tmp[0], tmp[1], tmp[2]);
                    input.bookingState[FOOD_INDEX] = parse(tmp[0]);
                    input.bookingState[CONCERT_INDEX] = parse(tmp[1]);
                    input.bookingState[ELECS_INDEX] = parse(tmp[2]);
                    if(input.bookingState[FOOD_INDEX] == -99999999 || input.bookingState[CONCERT_INDEX] == -99999999 || input.bookingState[ELECS_INDEX] == -99999999){
                        write(requestP[fds[i].fd].conn_fd, "[Error] Operation failed. Please try again.\n", 44);
                    }
                    else if(data.bookingState[FOOD_INDEX] + data.bookingState[CONCERT_INDEX] + data.bookingState[ELECS_INDEX] + input.bookingState[FOOD_INDEX] + input.bookingState[CONCERT_INDEX] + input.bookingState[ELECS_INDEX] > 15){
                        write(requestP[fds[i].fd].conn_fd, "[Error] Sorry, but you cannot book more than 15 items in total.\n", 64);
                    }
                    else if(data.bookingState[FOOD_INDEX] + input.bookingState[FOOD_INDEX] < 0 || data.bookingState[CONCERT_INDEX] + input.bookingState[CONCERT_INDEX] < 0 || data.bookingState[ELECS_INDEX] + input.bookingState[ELECS_INDEX] < 0){
                        write(requestP[fds[i].fd].conn_fd, "[Error] Sorry, but you cannot book less than 0 items.\n", 54);
                    }
                    else{
                        data.bookingState[FOOD_INDEX] += input.bookingState[FOOD_INDEX];
                        data.bookingState[CONCERT_INDEX] += input.bookingState[CONCERT_INDEX];
                        data.bookingState[ELECS_INDEX] += input.bookingState[ELECS_INDEX];
                        lseek(file_fd, (requestP[fds[i].fd].id - 902001) * sizeof(record), SEEK_SET);
                        write(file_fd, &data, sizeof(record));
                        sprintf(requestP[fds[i].fd].buf, "Bookings for user %d are updated, the new booking state is:\nFood: %d booked\nConcert: %d booked\nElectronics: %d booked\n", requestP[fds[i].fd].id, data.bookingState[FOOD_INDEX], data.bookingState[CONCERT_INDEX], data.bookingState[ELECS_INDEX]);
                        requestP[fds[i].fd].buf_len = strlen(requestP[fds[i].fd].buf);
                        write(requestP[fds[i].fd].conn_fd, requestP[fds[i].fd].buf, requestP[fds[i].fd].buf_len);
                        
                    }
                    setLock(&lock, F_UNLCK, SEEK_SET, sizeof(record) * (requestP[fds[i].fd].id - 902001), sizeof(record));
                    fcntl(file_fd, F_SETLK, &lock);
                    isLocked[requestP[fds[i].fd].id - 902001] = 0;
                    close(requestP[fds[i].fd].conn_fd);
                    free_request(&requestP[fds[i].fd]);
                    fds[i--] = fds[--nfds];
                }
#endif
            }
        }
        
        // Check new connection
        if(fds[0].revents == POLLIN){
            clilen = sizeof(cliaddr);
            conn_fd = accept(svr.listen_fd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen);
            if (conn_fd < 0) {
                if (errno == EINTR || errno == EAGAIN) continue;  // try again
                if (errno == ENFILE) {
                    (void) fprintf(stderr, "out of file descriptor table ... (maxconn %d)\n", maxfd);
                    continue;
                }
                ERR_EXIT("accept");
            }
            strcpy(requestP[conn_fd].host, inet_ntoa(cliaddr.sin_addr));
            fprintf(stderr, "getting a new request... fd %d from %s\n", conn_fd, requestP[conn_fd].host);
            write(conn_fd, "Please enter your id (to check your booking state):\n", 52);
            requestP[conn_fd].id = 0;
            requestP[conn_fd].conn_fd = conn_fd;
            fds[nfds].fd = conn_fd;
            fds[nfds].events = POLLIN;
            nfds ++;
        }
    }
    close(file_fd);
    free(requestP);
    return 0;
}

// ======================================================================================================
// You don't need to know how the following codes are working
#include <fcntl.h>

static void init_request(request* reqP) {
    reqP->conn_fd = -1;
    reqP->buf_len = 0;
    reqP->id = 0;
}

static void free_request(request* reqP) {
    /*if (reqP->filename != NULL) {
        free(reqP->filename);
        reqP->filename = NULL;
    }*/
    init_request(reqP);
}

static void init_server(unsigned short port) {
    struct sockaddr_in servaddr;
    int tmp;

    gethostname(svr.hostname, sizeof(svr.hostname));
    svr.port = port;

    svr.listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (svr.listen_fd < 0) ERR_EXIT("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    tmp = 1;
    if (setsockopt(svr.listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) {
        ERR_EXIT("setsockopt");
    }
    if (bind(svr.listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        ERR_EXIT("bind");
    }
    if (listen(svr.listen_fd, 1024) < 0) {
        ERR_EXIT("listen");
    }

    // Get file descripter table size and initialize request table
    maxfd = getdtablesize();
    requestP = (request*) malloc(sizeof(request) * maxfd);
    if (requestP == NULL) {
        ERR_EXIT("out of memory allocating all requests");
    }
    for (int i = 0; i < maxfd; i++) {
        init_request(&requestP[i]);
    }
    requestP[svr.listen_fd].conn_fd = svr.listen_fd;
    strcpy(requestP[svr.listen_fd].host, svr.hostname);

    return;
}
