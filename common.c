#include "common.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024


char ** tokenize(char * input) {
	char * sample = "string";
	return malloc(sizeof(sample) * 3);
}

void recv_file(char * path, int fd) {
	FILE *f;
	char buffer[BUFSIZE];
	read(fd, buffer, BUFSIZE);
	f = fopen(path, "w");
	fprintf(f, "%s", buffer);
	printf("The file is received successfully");
}

void send_file(char * path, int fd) {
	FILE *f;
	char buf[BUFSIZE];
	f = fopen(path, "r");
	fscanf(f, "%s", buf);
	write(fd, buf, BUFSIZE);
	printf("The file is sent successfully");
}

char * curr_dir() {
	return "this directory";
}

char * list_dir() {
	return "this list";
}

	
