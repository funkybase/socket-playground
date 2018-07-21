/* 
 * common.h
 */

/* common utilities folder for both server and client */

#ifndef COMMON_H_
#define COMMON_H_

char ** tokenize(char * input);

void recv_file(char * path, int fd);

void send_file(char * path, int fd);

char * curr_dir();

char * list_dir();

#endif
