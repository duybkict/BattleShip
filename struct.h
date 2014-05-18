#ifndef STRUCT_H
#define	STRUCT_H

#define ROW 10
#define COLUMN 10

typedef struct Player_ {
    char name[20];
    int win;
    int lose;
    int status;
    int fd;
    char ip[100];
} Player;

typedef struct Match_ {
    char log[100];
    Player playerOne;
    Player playerTwo;
    char mapOne[ROW][COLUMN];
    char mapTwo[ROW][COLUMN];
} Match;

typedef struct Turn_ {
    int row;
    int column;
} Turn;


Player new_player(int fd, int win, int lose, int status, char * name, char * ip);
void add_player(Player * list, Player player);
void remove_player(Player * list, Player player);


int find_player_socket(Match * list,int socket);
int find_sock_by_player_name(Player * list,char * name);


Match new_match(Player p1,Player p2);
void add_match(Match * list,Match match);
void remove_match(Match * list, Match match);


Player find_player_by_socket(Player * list,int socket);
Match find_match_by_socket(Match * list,int socket);


#endif	/* STRUCT_H */
