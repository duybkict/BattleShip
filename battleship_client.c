#include <stdio.h>
#include <stdlib.h>

#include "unp.h"
#include "game.c"

void onload();
int login_menu();
int request_login(int , char *);

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
			request_login(sockfd, username);
			
			break;
		case 2:
			exit(0);
			break;
	}
	
	while(1);
	
}

int login_menu() {
	char choice[10];
	
	printf("********** BATTLE SHIP GAME **********\n");
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

int main_menu() {
    int choice = 0;
    
    printf(" ----------------------------------------------------- \n\n");
    printf("       *****    Welcome to Battleship    ********       \n\n");
    printf(" ----------------------------------------------------- \n\n\n\n");
    
    printf("1. Choose player to play\n");
    printf("2. View ranking board\n");
    printf("3. Log out and exit\n");
    
    while (!(choice < 4 && choice > 0)) {
        printf("\tYour choice: "); scanf("%d", &choice);
        
        if (!(choice < 4 && choice > 0)) {
            printf("Invalid choice!!\n");
        }
        else
            return choice;
    }
    
    return 0;
}

int main_play_menu() {
    int choice = 0;
    
    printf(" ----------------------------------------------------- \n\n");
    printf("       *****    Invite or wait for invitation    ********      \n\n");
    printf(" ----------------------------------------------------- \n\n\n\n");
    
    printf("1. Send invitation\n");
    printf("2. Wait for invitation\n");
    printf("3. Exit to main menu\n");
    
    while (!(choice < 4 && choice > 0)) {
        printf("\tYour choice: "); scanf("%d", &choice);
        
        if (!(choice < 4 && choice > 0)) {
            printf("Invalid choice!!\n");
        }
        else
            return choice;
    }
    
    return 0;
}


