#ifndef GAME_H
#define	GAME_H

#define ROW 10
#define COLUMN 10

#define HIT 1
#define MISS 0

#define MAX_SHIP 2
#define MAX_HIT 5

/* grid symbols */
#define UNKNOWN		'.'
#define VOID		'O'
#define ACTIVE		'*'
#define DAMAGED		'X'
#define DESTROYED   'X'

typedef struct Coord_ {
    int x; // row
    int y; // column
} Coord;

typedef struct Ship_ {
    Coord head;
    int direction; // 1: up; -1: down; 2: left; -2: right
    int length;
} Ship;

void init_grid(char grid1[ROW][COLUMN], char grid2[ROW][COLUMN]);
void display_grid(char grid1[ROW][COLUMN], char grid2[ROW][COLUMN]);
int update_player_grid(int shot_x, int shot_y,char grid[ROW][COLUMN]);
void update_opponent_grid(int shot_x, int shot_y, char grid[ROW][COLUMN], int status);
void place_ships(char grid[ROW][COLUMN]);

#endif	/* STRUCT_H */
