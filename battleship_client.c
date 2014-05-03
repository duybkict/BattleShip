#include "unp.h"

void onload();
int login_menu();
int send_request_login(int , char *);

int sockfd;

int main(int argc, char **argv) {
	printf("TCP Client!\n");
	struct sockaddr_in servaddr;	
	char* server_ipaddress = "127.0.0.1";

	// Check for valid arguments
	if (argc >= 2) {
		strcpy(server_ipaddress, argv[1]);
	}

	// Connect sockets between client and server
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof (servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, server_ipaddress, &servaddr.sin_addr);

	Connect(sockfd, (SA *) & servaddr, sizeof (servaddr));
	
	onload();
	
	exit(0);
}

/**
 * Client main logic
 */
void onload() {
	
	int choice = login_menu();
	char username[256];	
	switch (choice) {
		case 1:
			printf("Enter your name: ");
			gets(username);
			send_request_login(sockfd, username);
			
			break;
		case 2:
			exit(0);
			break;
	}
	
}

int login_menu() {
	char choice[10];
	
	printf("********** BATTLE SHIP **********\n");
	printf("\n");
	printf(" 1. Login \n");
	printf(" 2. Exit \n");
	printf("\n");
	printf("*********************************\n");
	printf("Choice: ");
	
	while (1) {
		gets(choice);
		
		if (strlen(choice) == 1) {
			int c = choice[0] - '0';
			
			if (c >= 1 && c <= 2) {
				return c;
			}
		}
		
		printf("Invalid choice!\n");
		printf("Please enter again: ");
	}
	
	return 0;
}

