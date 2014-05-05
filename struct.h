#ifndef STRUCT_H
#define	STRUCT_H

#define ROW 10
#define COLUMN 10

#include <stdlib.h>
#include <string.h>

typedef struct Player_ {
    char * name;
    int win;
    int lose;
    int status;
    int fd;
    char * ip;
} Player;

typedef struct Match_ {
    int ID;
    char log[255];
    Player * playerOne;
    Player * playerTwo;
    char playerOneMap[ROW][COLUMN];
    char playerTwoMap[ROW][COLUMN];
} Match;

typedef struct Turn_ {
    int row;
    int column;
} Turn;

Player new_player(int fd, int win, int lose, int status, char * name, char * ip)
{
    Player player;
    player.fd = fd;
    player.win = win;
    player.lose = lose;
    player.status = status;
    strcpi(player.name, name);
    strcpi(player.ip, ip);
    
    return player;
}

void add_player(Player * list, Player player)
{
    int i = 0;
    while (1) {
        if (strcmp(list[i].name, "") == 0)
            break;
        i++;
    }
    
    list[i] = player;
}

void remove_player(Player * list, Player player)
{
    int i = 0;
    while (1) {
        if (strcmp(list[i].name, player.name) == 0)
            while (1) {
                list[i] = list[i+1];
                i++;
                if (strcmp(list[i].name, "") == 0)
                    return;
            }
        i++;
    }
}

Player find_player_by_socket(Player * list,int socket)
{
    int i = 0;
    Player player;
    while(1)
    {
        if (list[i].fd == socket)
            return list[i];
        i++;
        if (strcmp(list[i].name,"") == 0)
        {
            strcpy(player.name,"");
            return player;
        }
    }
}

int find_sock_by_player_name(Player * list,char * name)
{
    
    int i = 0;
    Player player;
    while(1)
    {
        if (strcmp(plist[i].name,"") == 0)
            return -1;
        
        if (strcmp(plist[i].name,name) == 0)
            return list[i].fd;
        i++;
    }
}

Match new_match(Player p1,Player p2)
{
    Match match;
    int i = 0, j = 0;
    
    match.playerOne = p1;
    match.playerTwo = p2;
    strcpy(match.log, "");
    
    for (i = 0; i < ROW; i++)
        for (j = 0; j < COLUMN; i++) {
            match.playerOneMap[i][j] = '~';
            match.playerTwoMap[i][j] = '~';
        }

    return match;
}
void add_match(Match * list,Match match)
{
    int i=0;
    while(1)
    {
        if (strcmp(list[i].log,"")==0)
            break;
        i++;
    }
    
    list[i] = match;
}
void remove_match(Match * list, Match match)
{
    int i=0;
    
    while(1)
    {
        if ((strcmp(list[i].playerOne.name, match.playerOne.name) == 0) || (strcmp(list[i].playerTwo.name, match.playerTwo.name) == 0))
            while (1)
            {
                list[i] = list[i+1];
                i++;
                if (strcmp(list[i].log, "") == 0)
                    return;
            }
        i++;
    }
    
}

Match find_match_by_socket(Match * list,int socket)
{
    int i = 0;
    Match match;
    
    while(1)
    {
        if (strcmp(list[i].log, "")==0)
        {
            printf ("Match list = 0\n");
            break;
        }
        if ((list[i].playerOne.fd == socket) || (list[i].playerTwo.fd == socket))
            return list[i];
        i++;
    }
    return match;
    
}
int find_player_socket(Match * list,int socket)
{
    int i=0;
    
    while(1)
    {
        if (strcmp(list[i].log,"") == 0)
            break;
        if (list[i].playerOne.fd == socket)
            return list[i].playerTwo.fd;
        if (list[i].playerTwo.fd==fd)
            return list[i].playerOne.fd;
        i++;
    }
    
    return -1;
}


#endif	/* STRUCT_H */

