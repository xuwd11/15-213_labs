/* Adapted from https://github.com/xiebei1108/csapp_code/blob/master/labs/proxylab-handout/proxy.c */

#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

#define MAX_HOSTNAME_LEN 64
#define MAX_PORT_LEN 20
#define MAX_HEADERNAME_LEN 32
#define MAX_HEADER_LEN 64

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

typedef struct {
    char hostname[MAX_HOSTNAME_LEN];
    char port[MAX_PORT_LEN];
    char path[MAXLINE];
} request_line;

typedef struct {
    char headername[MAX_HEADERNAME_LEN];
    char header[MAX_HEADER_LEN];
} header_line;

static void doit(int fd);
static void parse_request(int fd, request_line *linep, header_line *headers, int *num_headers);
static void parse_uri(char *uri, request_line *linep);
static header_line parse_header(char *buf);
static int send_to_server(request_line *linep, header_line *headers, int num_headers);

int main(int argc, char **argv)
{
    printf("%s", user_agent_hdr);
    int listenfd, *connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Malloc(sizeof(int));
        *connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        doit(*((int*)connfd));
    }

    return 0;
}


static void doit(int fd) {
    char buf[MAXLINE], uri[MAXLINE];
    int total_size, connfd;
    rio_t rio;
    request_line line;
    header_line headers[20];
    int num_headers, n;
    parse_request(fd, &line, headers, &num_headers);

    strcpy(uri, line.hostname);
    strcpy(uri + strlen(uri), line.path);

    total_size = 0;
    connfd = send_to_server(&line, headers, num_headers);
    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE))) {
        Rio_writen(fd, buf, n);
        total_size += n;
    }
    Close(connfd);
}

static void parse_request(int fd, request_line *linep, header_line *headers, int *num_headers) {
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))
        return;

    sscanf(buf, "%s %s %s", method, uri, version);
    parse_uri(uri, linep);

    *num_headers = 0;
    Rio_readlineb(&rio, buf, MAXLINE);
    while(strcmp(buf, "\r\n")) {
        headers[(*num_headers)++] = parse_header(buf);
        Rio_readlineb(&rio, buf, MAXLINE);
    }
}

static void parse_uri(char *uri, request_line *linep) {
    if (strstr(uri, "http://") != uri) {
        fprintf(stderr, "Error: invalid uri\n");
        exit(0);
    }
    uri += strlen("http://");
    char *c = strstr(uri, ":");
    *c = '\0';
    strcpy(linep->hostname, uri);
    uri = c + 1;
    c = strstr(uri, "/");
    *c = '\0';
    strcpy(linep->port, uri);
    *c = '/';
    strcpy(linep->path, c);
}

static header_line parse_header(char *buf) {
    header_line header;
    char *c = strstr(buf, ": ");
    if (c == NULL) {
        fprintf(stderr, "Error: invalid header\n");
        exit(0);
    }
    *c = '\0';
    strcpy(header.headername, buf);
    strcpy(header.header, c + 2);
    return header;
}

static int send_to_server(request_line *linep, header_line *headers, int num_headers) {
    int clientfd;
    char buf[MAXLINE], *buf_head = buf;
    rio_t rio;

    clientfd = Open_clientfd(linep->hostname, linep->port);
    Rio_readinitb(&rio, clientfd);
    sprintf(buf_head, "GET %s HTTP/1.0\r\n", linep->path);
    buf_head = buf + strlen(buf);
    for (int i = 0; i < num_headers; i++) {
        sprintf(buf_head, "%s: %s", headers[i].headername, headers[i].header);
        buf_head = buf + strlen(buf);
    }
    sprintf(buf_head, "\r\n");
    Rio_writen(clientfd, buf, MAXLINE);

    return clientfd;
}