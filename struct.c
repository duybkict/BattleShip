#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "struct.h"

Player new_player(int fd, int win, int lose, int status, char * name, char * ip)
{
    printf("+++ create new player\n");
    Player player;
    player.fd = fd;
    player.win = win;
    player.lose = lose;
    player.status = status;
    strcpy(player.name, name);
    strcpy(player.ip, ip);
    
    return player;
}

void add_player(Player * list, Player player)
{
    printf("+++ add player\n");
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
    printf("+++ remove player\n");
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
    printf("+++ find player by socket\n");
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
    printf("+++ find socket by player name\n");
    int i = 0;

    while(1)
    {
        if (strcmp(list[i].name,"") == 0)
            return -1;
        
        if (strcmp(list[i].name,name) == 0)
            return list[i].fd;
        i++;
    }
}

void update_player_status(Player * list, int status, int socket)
{
    printf("+++ update player status\n");
    int i = 0;
    
    while(1)
    {
        if (list[i].fd == socket) {
            list[i].status = status;
            break;
        }
        i++;
        if (strcmp(list[i].name,"") == 0)
            return;
    }
}

void update_player_win(Player * list, int socket)
{
    printf("+++ update player status\n");
    int i = 0;
    
    while(1)
    {
        if (list[i].fd == socket) {
            list[i].win++;
            break;
        }
        i++;
        if (strcmp(list[i].name,"") == 0)
            return;
    }
}

void update_player_lose(Player * list, int socket)
{
    printf("+++ update player status\n");
    int i = 0;
    
    while(1)
    {
        if (list[i].fd == socket) {
            list[i].lose++;
            break;
        }
        i++;
        if (strcmp(list[i].name,"") == 0)
            return;
    }
}

Match new_match(Player p1,Player p2)
{
    printf("+++ new match\n");
    char log[100];
    Match match;
    int i = 0, j = 0;
    
    match.playerOne = p1;
    match.playerTwo = p2;

    strcat(log,p1.name);
    strcat(log,p2.name);
    strcat(log,".txt");
    strcpy(match.log,log);

    for (i = 0; i < ROW; i++)
        for (j = 0; j < COLUMN; j++) {
            match.mapOne[i][j] = '-';
            match.mapTwo[i][j] = '-';
        }

    return match;
}

void add_match(Match * list,Match match)
{
    printf("+++ add match\n");
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
    printf("+++ remove match\n");
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
    printf("+++ find match by socket: %d\n", socket);
    int i = 0;
    Match match;
    strcpy(match.log, "NA");

    while(i < 10)
    {
        if ((list[i].playerOne.fd == socket) || (list[i].playerTwo.fd == socket))
            return list[i];
        i++;
    }
    
    return match;
}

int find_other_player_socket(Match * list,int socket)
{
    printf("+++ find player socket: %d\n", socket);
    int i=0;
    
    while(1)
    {
        if (strcmp(list[i].log,"") == 0)
            break;
        if (list[i].playerOne.fd == socket)
            return list[i].playerTwo.fd;
        if (list[i].playerTwo.fd == socket)
            return list[i].playerOne.fd;
        i++;
    }
    
    return -1;
}
