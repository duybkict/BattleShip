#include "unp.h"
#include "constant.h"

void process_message(int sockfd, char *buf, int length);
void process_login(int sockfd, char *username);
void process_logout(int sockfd);

typedef struct {
	int sockfd;
	char* username;
} Client;

Client logged_in_users[MAX_PLAYER_POOL];

int main(int argc, char **argv) {
	int i, maxi, maxfd, listenfd, connfd, sockfd;
	int nready, client[FD_SETSIZE];
	ssize_t n;
	fd_set rset, allset;
	char buf[MAXLINE];
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof (servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(listenfd, (SA *) & servaddr, sizeof (servaddr));

	Listen(listenfd, LISTENQ);

	maxfd = listenfd; /* initialize */
	maxi = -1; /* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1; /* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
	/* end fig01 */

	/* include fig02 */
	for (;;) {
		rset = allset; /* structure assignment */
		nready = Select(maxfd + 1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)) { /* new client connection */
			clilen = sizeof (cliaddr);
			connfd = Accept(listenfd, (SA *) & cliaddr, &clilen);
#ifdef	NOTDEF
			printf("new client: %s, port %d\n",
				Inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
				ntohs(cliaddr.sin_port));
#endif

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					printf("Accept client %d\n", connfd);
					client[i] = connfd; /* save descriptor */
					break;
				}
			if (i == FD_SETSIZE)
				err_quit("too many clients");

			FD_SET(connfd, &allset); /* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd; /* for select */
			if (i > maxi)
				maxi = i; /* max index in client[] array */

			if (--nready <= 0)
				continue; /* no more readable descriptors */
		}

		for (i = 0; i <= maxi; i++) { /* check all clients for data */
			if ((sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
					/*4connection closed by client */
					process_logout(sockfd);
				} else {
					process_message(sockfd, buf, n);
				}

				if (--nready <= 0)
					break; /* no more readable descriptors */
			}
		}
	}
}

void process_message(int sockfd, char *buf, int length) {
	if (DEBUG) printf("Start processing message...\n");
	
	char tokens[MAX_TOKEN_NUM][MAXLINE];
	char *str1, *token, *saveptr1;
	int i;

	for (i = 0, str1 = buf; ; i++, str1 = NULL) {
		token = strtok_r(str1, "|", &saveptr1);
		if (token == NULL)
			break;
		strcpy(tokens[i], token);
	}
	
	switch (atoi(tokens[0])) {
		case MSG_LOGIN:
			process_login(sockfd, tokens[1]);
			break;
	}

	if (DEBUG) printf("End processing message...\n");
}

void process_login(int sockfd, char* username) {
	if (DEBUG) printf("Start login...\n");

	char response[MAXLINE];
	int i;

	for (i = 0; i < MAX_PLAYER_POOL; i++) {
//		if (logged_in_users[i] == NULL)
//			break;
		
		if (strcmp(logged_in_users[i].username, username) == 0) {
			
			// Another player logged in with the same name
			sprintf(response, "%d%c%d%c%s", MSG_LOGIN, DELIMITER, ERROR, DELIMITER, "Another player logged in with the same name");
			Writen(sockfd, response, strlen(response));
			return;
		}
	}

	if (sockfd >= MAX_PLAYER_POOL) {
		
		// Reach maximum players pool
		sprintf(response, "%d%c%d%c%s", MSG_LOGIN, DELIMITER, ERROR, DELIMITER, "Reach maximum players pool");
		Writen(sockfd, response, strlen(response));
		return;
	}

	// Login successfully
	logged_in_users[sockfd].sockfd = sockfd;
	logged_in_users[sockfd].username = strdup(username);

	sprintf(response, "%d%c%d%c%s", MSG_LOGIN, DELIMITER, SUCCESS, DELIMITER, "Login successfully");
	Writen(sockfd, response, strlen(response));
	
	if (DEBUG) printf("End login...\n");
}

void process_logout(int sockfd) {
	
}