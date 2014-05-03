#include <stdio.h>
#include <stdlib.h>
#include "unp.h"
#include "requester.h"

int send_request_login(int sockfd, char *username) {
	char buf[MAXLINE];
	
	Writen(sockfd, username, strlen(username));	
	Read(sockfd, buf, MAXLINE);
	printf("Logged in as %s\n", buf);
	
	return 1;
}
