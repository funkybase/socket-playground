/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "common.h"

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

static char input[2048];

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    puts("MyFTP Version 0.0.0.1");
    puts("Press Ctrl+c to Exit\n");

    if (argc != 2) {
        fprintf(stderr,"myftp >  [ERROR] usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "myftp >  [ERROR] getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("myftp > [ERROR] client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("myftp > [ERROR] client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "myftp > [ERROR] client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("myftp > client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("myftp > [ERROR] recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("myftp > client: received '%s'\n",buf);

    //add prompt here ...
    while(1) {
	    // Output our prompt
	    fputs("myftp > ", stdout);

	    //Read a line of user input of maximum size 2049
	    fgets(input, 2048, stdin);

	    //parse input remove the \n
	    strtok(input, "\n");
	    
	    if (strcmp(input, "exit") == 0) 
		    break;
	    
    	    if (send(sockfd, input, 13, 0) == -1) {
	    	    perror("myftp > [ERROR] send");
	    }

	    char *token = strtok(input, " ");
	    int i = 0;
	    char *args[5];

	    while (token != NULL) {
		    args[i++] = token;
		    token = strtok(NULL, " ");
	    }

	    if (strcmp(args[0], "lcd") == 0) {
		    if(args[1] == NULL) {
			    fprintf(stderr, "myftp > expected argument to \"lcd\"\n");
		    } else {
		    	if(chdir(args[1]) != 0) 
				perror("myftp > [ERROR] ");
		    }
		    continue;
	    } 

	    if (strcmp(args[0], "lpwd") == 0){
		    //print working directory
		    continue;
	    }
		
	    if (strcmp(args[0], "ldir") == 0) {
		    //print ls
		    continue;
	    }

	    if (strcmp(args[0], "get") == 0) {
		    //download/recv file
		    //recv_file(args[1], sockfd);
		    continue;
	    }
	    
	    if (strcmp (args[0], "put") == 0) {
		    //upload/send file
		    send_file(args[1], sockfd);
		    continue;
	    }
	   
    }

    close(sockfd);

    return 0;
}
