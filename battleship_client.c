#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "struct.c"
#include "constant.h"

typedef char Board;

void play_game(Board board[ROW][COLUMN],int sock,int turn)
{
    int i = 0;
    printf("Just for wait..."); scanf("%d", &i);
}

int login_menu() {
	char choice[10];
	
	printf("\n********** BATTLE SHIP GAME **********\n");
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
    
    printf("\n ----------------------------------------------------- \n\n");
    printf("       *****    Invite or wait for invitation    ********      \n\n");
    printf(" ----------------------------------------------------- \n\n\n");
    
    printf("1. Send invitation\n");
    printf("2. Wait for invitation\n");
    printf("3. Exit\n");
    
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

int main(int args, char * argv[])
{
    int i=0;
    Board board[ROW][COLUMN];
    Player * plist = (Player *)malloc(sizeof(Player)*100);

    char user[50];
    char passwd[50];
    char invite[10];

    int sock;
    long bytes_recieved;
    char send_data[1024],recv_data[1024];
    struct hostent *host;
    struct sockaddr_in server_addr;
    int mn,mn2;

    host = gethostbyname("127.0.0.1");
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero),8);
    
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Connect");
        exit(1);
    }
    
    while(1)
    {
        mn = login_menu();
        switch(mn)
	    {
            case 1:
                send(sock,"login",5,0);
                printf("\tUsername: ");
                //gets(user);
                scanf("%s", user);
                printf("\tPassword: ");
                //gets(passwd);
                scanf("%s", passwd);
                strcat(user,":");
                strcat(user,passwd);
                
                send(sock,user,strlen(user),0);
                bytes_recieved = recv(sock,recv_data,1024,0);
                
                recv_data[bytes_recieved] = '\0';
                if (strcmp(recv_data,"ok")==0)
                {
                    printf("* LOGIN SUCCESS *\n");
                    
                    send(sock,"loginok",7,0);
                    bytes_recieved=recv(sock,plist,sizeof(Player)*100,0);
                    
                    printf ("\n\t-- Online list:\n");
                    i=0;
                    while(1)
                    {
                        if (strcmp(plist[i].name,"")==0) break;
                        else
                            printf("\t- %s -\n",plist[i].name);
                        i++;
                    }
                    if (i == 0)
                        printf("No one online except you!!\n");
                    
                    mn2 = main_play_menu();
                    switch(mn2)
                    {
                        case 1://mn2
                            send(sock,"invite",6,0);
                            
                            printf("Invite player: ");
                            fflush(stdin);
                            scanf("%s", invite);
                            
                            send(sock,invite,sizeof(invite),0);
                            printf ("waiting for accept......\n");
                            bytes_recieved=recv(sock,recv_data,1024,0);
                            recv_data[bytes_recieved] = '\0';
                            printf("\t#### recv from server: %s\n",recv_data);
                            
                            if (strcmp(recv_data,"accept")==0)
                            {
                                play_game(board,sock,1);
                                //printf("Play GAME!!");
                            }
                            break;
                        case 2://mn2
                            bytes_recieved=recv(sock,recv_data,1024,0);
                            recv_data[bytes_recieved] = '\0';
                            printf("\t#### recv from server: %s\n",recv_data);
                            printf("YES or NO (y/n): ");
                            fflush(stdin);
                            scanf("%s",send_data);
                            send(sock,send_data,sizeof(send_data),0);
                            if (strcmp(send_data,"y")==0)
                                play_game(board,sock,0);
                                //printf("Play GAME!!");
                            break;//mn2
                        case 3:
                            exit(1);
                            break;
                    }
                }
                else
                    printf("\tLogin failed!! :(\n");
                break;
                
            /*case 3://mn1
                send(sock,"register",8,0);
                bytes_recieved=recv(sock,recv_data,1024,0);
                recv_data[bytes_recieved] = '\0';
                if(strcmp(recv_data,"continue")==0)
                {
                    printf("user :");
                    gets(user);
                    printf("pass :");
                    gets(passwd);
                    strcat(user,":");
                    strcat(user,passwd);
                    send(sock,user,strlen(user),0);
                    
                }
                send(sock,"register",8,0);
                bytes_recieved=recv(sock,recv_data,1024,0);
                if(strcmp(recv_data,"ok"))
                    printf("dang ky thanh cong\n");
                else
                    printf("ten dang nhap nay da co nguoi dung\n");
                break;*/
            case 2://mn1
                close(sock);
                exit(1);
                break;
	    }
    }
    
	close(sock);
	return 0;
}
