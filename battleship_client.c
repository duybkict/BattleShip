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
#include "game.c"
#include "constant.h"

typedef char Board;

Board myMap[ROW][COLUMN];
Board enemyMap[ROW][COLUMN];

void play_game(int sock,int turn)
{
    int flag = 0;
    Coord target;
    char message[20];
    long bytes_recieved;
    int remaining_point = MAX_HIT;
    
    int hit_status = 0;
    
    strcpy(message, "0-00-0");
    
    printf ("\nXXXXX   XXXX  XXXXXX XXXXXX XX     XXXXXX  XXXXX XX  XX XX XXXX\n");
	printf   ("XX  XX XX  XX   XX     XX   XX     XX     XX     XX  XX XX XX  XX\n");
	printf   ("XXXXX  XX  XX   XX     XX   XX     XXXX    XXXX  XXXXXX XX XXXX\n");
	printf   ("XX  XX XXXXXX   XX     XX   XX     XX         XX XX  XX XX XX\n");
	printf   ("XXXXX  XX  XX   XX     XX   XXXXXX XXXXXX XXXXX  XX  XX XX XX\n");
	printf ("\n\n");
    
    init_grid(myMap, enemyMap);
    
    place_ships(myMap);
    
    while (1) {
        if (turn == 0)
        {
            if (message[1] != '-')
                update_opponent_grid(message[0] - 48, message[2] - 48, enemyMap, message[1] - 48);
            
            display_grid(myMap, enemyMap);

            do {
                flag = 0;
                
                fflush(stdin);
                printf("\n*** YOUR TURN TO FIRE!!!\n");
                printf("\t+ Enter row: "); scanf("%d", &target.x);
                printf("\t+ Enter column: "); scanf("%d", &target.y);
                if (!(0 <= target.x && 0 <= target.y && target.x <= 9 && target.y <= 9))
                    printf("Invalid coordinate!!\n");
                else
                    flag = 1;
            } while (!flag);
            
            message[0] = target.x + 48;
            message[2] = target.y + 48;
            
            printf("\n* You fired at (%d,%d) *\n", target.x, target.y);
            
            send(sock, message, strlen(message), 0);
            
            printf("\nwaiting for opponent to make move....\n");
            
            bytes_recieved = recv(sock, message, 1024, 0);
            message[bytes_recieved] = '\0';
            
            if (strcmp(message, "disconnect") == 0) {
                printf("\n$$$ Opponent disconnected from server!! :v $$$\n");
                break;
            }
            else if (strcmp(message, "win") == 0) {
                printf("\n\n\t\t\t===xxxx YOU WIN!! :) xxxx===\n\n");
                break;
            }
            
            printf("\n* Received shot at (%d,%d) *\n", message[3] - 48, message[5] - 48);
            // update player's map
            hit_status = update_player_grid(message[3] - 48, message[5] - 48, myMap);
            message[4] = hit_status + 48;
            
            remaining_point -= hit_status;
            
            if (remaining_point == 0) {
                printf("\n\n\t\t\t===xxxx YOU LOSE!! :v xxxx===\n\n");
                send(sock, "lose", 4, 0);
                break;
            }
        }
        else
        {
            printf("\nwaiting for opponent to make move....\n");
            
            bytes_recieved = recv(sock, message, 1024, 0);
            message[bytes_recieved] = '\0';
            
            if (strcmp(message, "disconnect") == 0) {
                printf("\n$$$ Opponent disconnected from server!! :v $$$\n");
                break;
            }
            else if (strcmp(message, "win") == 0) {
                printf("\n\n\t\t\t===xxxx YOU WIN!! :) xxxx===\n\n");
                break;
            }
            
            printf("\n* Receive shot at (%d,%d) *\n", message[0] - 48, message[2] - 48);
            // update player's map
            hit_status = update_player_grid(message[0] - 48, message[2] - 48, myMap);
            message[1] = hit_status + 48;
            
            remaining_point -= hit_status;
            
            if (remaining_point == 0) {
                printf("\n\n\t\t\t===xxxx YOU LOSE!! :v xxxx===\n\n");
                send(sock, "lose", 4, 0);
                break;
            }
            
            if (message[4] != '-')
                update_opponent_grid(message[3] - 48, message[5] - 48, enemyMap, message[4] - 48);
                
            display_grid(myMap, enemyMap);
            
            do {
                flag = 0;
                printf("*** YOUR TURN TO FIRE!!!\n");
                printf("\t+ Enter row: "); scanf("%d", &target.x);
                printf("\t+ Enter column: "); scanf("%d", &target.y);
                if (!(0 <= target.x && 0 <= target.y && target.x <= 9 && target.y <= 9)) {
                    printf("Invalid coordinate!!\n");
                }
                else
                    flag = 1;
            } while (!flag);
            
            message[3] = target.x + 48;
            message[5] = target.y + 48;
            
            printf("\n* You fire at (%d,%d) *\n", target.x, target.y);
            
            send(sock, message, strlen(message), 0);
        }
    }
}

int login_menu() {
	char choice[10];
	
	printf("\n********** BATTLE SHIP GAME **********\n");
	printf("\n");
	printf(" 1. Login \n");
  	printf(" 1. Register \n");
	printf(" 3. Exit \n");
	printf("\n");
	printf("*********************************\n");
	printf("Choice: ");
	
	while (1) {
		gets(choice);
		
		if (strlen(choice) == 1) {
			int c = choice[0] - '0';
			
			if (c >= 1 && c <= 3) {
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
    
    do {
        fflush(stdin);
        printf("\tYour choice: "); scanf("%d", &choice);
        
        if (!(choice < 4 && choice > 0)) {
            printf("Invalid choice!!\n");
        }
        else
            return choice;
    } while (!(choice < 4 && choice > 0));
    
    return 0;
}

int main_play_menu() {
    int choice = 0;
    
    printf("\n ----------------------------------------------------- \n\n");
    printf("       *****    Invite or wait for invitation    ********      \n\n");
    printf(" ----------------------------------------------------- \n\n\n");
    
    printf("1. Send invitation\n");
    printf("2. Wait for invitation\n");
    printf("3. View list of online players\n");
    printf("4. View ranking board\n");
    printf("5. Exit\n");
    
    do {
        fflush(stdin);
        printf("\tYour choice: "); scanf("%d", &choice);
        
        if (!(choice < 6 && choice > 0)) {
            printf("Invalid choice!!\n");
        }
        else
            return choice;
    } while (!(choice < 6 && choice > 0));
    
    return 0;
}

int main(int args, char * argv[])
{
    int i=0;
    
    Player * plist = (Player *)malloc(sizeof(Player) * 100);

    char user[20];
    char passwd[20];
    char invite[20];

    int sock;
    long bytes_recieved;
    char send_data[1024], recv_data[1024];
    struct hostent * host;
    struct sockaddr_in server_addr;
    int mn, mn2;

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
                scanf("%s", user);
                printf("\tPassword: ");
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
                    
                    printf ("\n\t-- Online list: (LEGEND: 0-IDLE; 1-ON WAIT; 2-IN GAME)\n");
                    i=0;
                    while(1)
                    {
                        if (strcmp(plist[i].name,"")==0) break;
                        else
                            printf("\t- %s : %d -\n",plist[i].name, plist[i].status);
                        i++;
                    }
                    if (i == 0)
                        printf("No one online except you!!\n");
                MENU2:
                    mn2 = main_play_menu();
                    switch(mn2)
                    {
                        case 1://mn2
                            send(sock,"invite",6,0);
                            
                            printf("Invite player: ");
                            do {
                                fflush(stdin);
                                scanf("%s", invite);
                                if (!strcmp(invite, user))
                                    printf("You can not play with yourself!!\n");
                            } while (!strcmp(invite, user));
                            send(sock,invite,sizeof(invite),0);
                            printf ("waiting for accept......\n");
                            bytes_recieved = recv(sock,recv_data,1024,0);
                            recv_data[bytes_recieved] = '\0';
                            printf("\t#### recv from server: %s\n",recv_data);
                            
                            if (strcmp(recv_data,"accept") == 0)
                            {
                                play_game(sock, 0);
                            }
                            else if (strcmp(recv_data, "notfound") == 0)
                            {
                                printf("No user found!!\n");
                            }
                            break;
                        case 2://mn2
                            printf("waiting for invitation....\n");
                            send(sock, "onwait", 6, 0);

                            bytes_recieved=recv(sock,recv_data,1024,0);
                            recv_data[bytes_recieved] = '\0';
                            printf("\t#### recv from server: %s\n",recv_data);
                            printf("Take the challenge? (y/n): ");
                            do {
                                fflush(stdin);
                                scanf("%s",send_data);
                                if (!(strcmp(send_data, "y") == 0 || strcmp(send_data, "n") == 0))
                                    printf("You can only reply with either 'y' or 'n'!!\nAgain: ");
                            } while (!(strcmp(send_data, "y") == 0 || strcmp(send_data, "n") == 0));
                            send(sock,send_data,sizeof(send_data),0);
                            if (strcmp(send_data,"y")==0) {
                                play_game(sock, 1);
                            }
                            break;
                        case 3://mn2
                            send(sock, "viewlist", 8, 0);
                            bytes_recieved = recv(sock,plist,sizeof(Player)*100,0);
                            
                            printf ("\n\t-- Online list: (LEGEND: 0-IDLE; 1-ON WAIT; 2-IN GAME)\n");
                            i=0;
                            while(1)
                            {
                                if (strcmp(plist[i].name,"")==0)
                                    break;
                                else
                                    printf("\t- %s : %d -\n",plist[i].name, plist[i].status);
                                i++;
                            }
                            if (i == 0)
                                printf("No one online except you!!\n");
                            break;
                        case 4://mn2
                            printf("Not yet implemented :(\n");
                            break;
                        case 5://mn2
                            exit(1);
                            break;
                    }
                    goto MENU2;
                }
                else
                    printf("\t* Invalid username or password!! *\n");
                break;
                
            case 2://mn1
                /*send(sock,"register",8,0);
                
                printf("\tNew Username: ");
                scanf("%s", user);
                printf("\tPassword: ");
                scanf("%s", passwd);
                strcat(user,":");
                strcat(user,passwd);
                send(sock, user, strlen(user), 0);

                bytes_recieved = recv(sock,recv_data,1024,0);
                recv_data[bytes_recieved] = '\0';
                
                if(strcmp(recv_data,"success")) {
                    printf("* Registered *\n");
                    printf("It may sound stupid but please log in again :/\n");
                    goto MENU2;
                }
                else
                    printf("* Username already in use *\n");*/
                printf("Not yet completed!! :(\n");
                break;
            case 3://mn1
                close(sock);
                exit(1);
                break;
	    }
    }
    
	close(sock);
	return 0;
}
