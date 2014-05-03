#include "unp.h"

void process_request(int sockfd, int type);
void process_mode3(int sockfd);
void process_mode1(int sockfd);
void process_mode2(int sockfd);

/**
 * make and then "./tcpserv01 [mode]" to run
 * mode = 1: reverse string
 * mode = 2: reverse string and save to file
 * mode = 3: take number input and return
 */
int main(int argc, char **argv)
{
        printf("TCP Server!\n");
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family	  = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port		= htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	while(1) {
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

		if ( (childpid = Fork()) == 0) {
			Close(listenfd);
			int mode = atoi(argv[1]);
			// process request from client
			process_request(connfd, mode);
			exit(0);
		}
		Close(connfd);
	}
}

void process_request(int sockfd, int mode)
{
	switch (mode) {
		case 1:
			process_mode1(sockfd);
			break;
		case 2:
			process_mode2(sockfd);
			break;
		case 3:
			process_mode3(sockfd);
			break;
		default:
			// default behavior
			str_echo(sockfd);
			break;
	}
}

void process_mode1(int sockfd)
{
	ssize_t		n;
	char		str[MAXLINE];
	
	while ( (n = read(sockfd, str, MAXLINE)) > 0) {
		str[n] = '\0';		
		char result[MAXLINE];
		
		if (strcmp(str, "\n") == 0) {
			exit(1);
		}
		
		int i = 0;
		for (i = 0; i < strlen(str) - 1; i++) {
			result[i] = str[strlen(str) - i - 2];
		}
		
		result[i++] = '\n';
		result[i] = '\0';
		
		Writen(sockfd, result, strlen(result));
	}
	
	if (n < 0 && errno == EINTR) {
		return process_mode1(sockfd);
	} else if (n < 0) {
		err_sys("str_echo: read error");
	}
}

void process_mode2(int sockfd)
{
	ssize_t		n;
	char		str[MAXLINE];
	
	while ( (n = read(sockfd, str, MAXLINE)) > 0) {
		str[n] = '\0';		
		char result[MAXLINE];
		
		int i = 0;
		for (i = 0; i < strlen(str) - 1; i++) {
			result[i] = str[strlen(str) - i - 2];
		}
		
		result[i++] = '\n';
		result[i] = '\0';
		
		FILE *file = fopen("reverse.txt", "w");
		if (file == NULL) {
			printf("Error opening file!\n");
			exit(1);
		}
		
		fprintf(file, "%s", result);
		fclose(file);
		
		strcpy(result, "Saved to file!\n");
		Writen(sockfd, result, strlen(result));
	}
	
	if (n < 0 && errno == EINTR) {
		return process_mode2(sockfd);
	} else if (n < 0) {
		err_sys("str_echo: read error");
	}
}

void process_mode3(int sockfd)
{
	ssize_t		n;
	char		str[MAXLINE];

	while ( (n = read(sockfd, str, MAXLINE)) > 0) {
		str[n] = '\0';
		int number = atoi(str);
		char result[MAXLINE];
		
		if (number < 0 || number > 10 || strlen(str) < 2 || strlen(str) > 3) {
			strcpy(result, "Out of range!\n");
		} else if (number == 0) {
			exit(0);
		} else {
			strcpy(result, "Received ");
			strcat(result, str);
		}
		
		Writen(sockfd, result, strlen(result));
	}
	
	if (n < 0 && errno == EINTR) {
		return process_mode3(sockfd);
	} else if (n < 0) {
		err_sys("str_echo: read error");
	}
}
