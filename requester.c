#include "requester.h"

void process_response(char* response, char** tokens);

int request_login(int sockfd, char *username) {
	if (DEBUG) printf("Start login...\n");

	char buf[MAXLINE];
	char tokens[MAX_TOKEN_NUM][MAXLINE];

	sprintf(buf, "%d%c%s", MSG_LOGIN, DELIMITER, username);
	if (DEBUG) printf("Sent\t %s\n", buf);
	Writen(sockfd, buf, MAXLINE);

	Read(sockfd, buf, MAXLINE);
	if (DEBUG) printf("Received\t %s\n", buf);	
	process_response(buf, tokens);

	return 1;
}

void process_response(char* response, char** tokens) {
	char *str1, *token, *saveptr1;
	int i;

	for (i = 0, str1 = response;; i++, str1 = NULL) {
		token = strtok_r(str1, "|", &saveptr1);
		if (token == NULL)
			break;
		tokens[i] = strdup(token);
	}
}
