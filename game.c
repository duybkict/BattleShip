#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "game.h"

void init_grid(char grid1[ROW][COLUMN], char grid2[ROW][COLUMN])
{
    int i = 0, j = 0;
    
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            grid1[i][j] = UNKNOWN;
            grid2[i][j] = UNKNOWN;
        }
    }
}

void display_grid(char grid1[ROW][COLUMN], char grid2[ROW][COLUMN])
{
    int i,j;
    
    printf("@@ Your map @@\t\t\t  @@ Enemy's map @@\n\n");
	printf("  0 1 2 3 4 5 6 7 8 9\t\t    0 1 2 3 4 5 6 7 8 9\n\n");      // printing algorithm
    
	for (i = 0; i < ROW; i++)
	{
        printf("%d", i);
        for (j = 0; j < COLUMN; j++)
            printf(" %c", grid1[i][j]);
        
        printf("\t\t  %d", i);
        for (j = 0; j < COLUMN; j++)
            printf(" %c", grid2[i][j]);
        printf("\n");
	}
}

void view_grid(char grid[ROW][COLUMN])
{
    int i = 0, j = 0;
	printf("   0 1 2 3 4 5 6 7 8 9\n");
    for (i = 0; i < ROW; i++)
    {
        printf("%d ", i);
        for (j = 0; j < COLUMN; j++)
            printf(" %c", grid[i][j]);
        printf("\n");
    }
}

int check_valid_ship(char grid[ROW][COLUMN], int dir, Coord coord, int i)
{
    int flag = 0, j = 0;
    
    if (!(coord.x <= 9 && coord.x >= 0 && dir > -3 && dir < 3 && dir != 0)) {
        printf("Invalid input\n");
        return 1;
    }
    
    switch (dir) {
        case 1: // up
            for (j = 0; j < i + 2; j++)
                if (grid[coord.x - j][coord.y] == ACTIVE) {
                    flag = 1;
                    break;
                }
            if ((coord.x - i - 3) < 0)
                flag = 1;
            break;
        case 2: // left
            for (j = 0; j < i + 2; j++)
                if (grid[coord.x][coord.y - j] == ACTIVE) {
                    flag = 1;
                    break;
                }
            if ((coord.y - i - 3) < 0)
                flag = 1;
            break;
        case -1: // down
            for (j = 0; j < i + 2; j++)
                if (grid[coord.x + j][coord.y] == ACTIVE) {
                    flag = 1;
                    break;
                }
            if ((coord.x + i + 2) > 9)
                flag = 1;
            break;
        case -2: // right
            for (j = 0; j < i + 2; j++)
                if (grid[coord.x][coord.y + j] == ACTIVE) {
                    flag = 1;
                    break;
                }
            if ((coord.y + i + 2) > 9)
                flag = 1;
            break;
        default:
            break;
    }
    
    if (flag)
        printf("* ERROR * Ship can not overlap others or be placed outside the map\n");
    
    return flag;
}

void place_ships(char grid[ROW][COLUMN])
{
    Coord coord;
    
    int i = 0, j = 0;
    int direction = 0;
    
    printf("Please place your ship:\n");
    
    for (i = 0; i < MAX_SHIP; i++)
    {
        do {
            printf("- [Ship %d (%d in length)]\n", i + 1, i + 2);
            printf("- Head of ship:\n\t+ row: "); scanf("%d", &coord.x);
            printf("\t+ column: "); scanf("%d", &coord.y);
            printf("\t+ direction: (up: 1; left: 2; down: -1; right: -2): "); scanf("%d", &direction);
        } while (check_valid_ship(grid, direction, coord, i));
        
        switch (direction) {
            case 1:
                for (j = 0; j < i + 2; j++) {
                    grid[coord.x - j][coord.y] = ACTIVE;
                }
                break;
            case 2:
                for (j = 0; j < i + 2; j++) {
                    grid[coord.x][coord.y - j] = ACTIVE;
                }
                break;
            case -1:
                for (j = 0; j < i + 2; j++) {
                    grid[coord.x + j][coord.y] = ACTIVE;
                }
                break;
            case -2:
                for (j = 0; j < i + 2; j++) {
                    grid[coord.x][coord.y + j] = ACTIVE;
                }
                break;
            default:
                break;
        }
        
        view_grid(grid);
        printf("Ship placed!!\n");
    }
    
    printf("* All ships placed!! *\n\n");
}

int update_player_grid(int shot_x, int shot_y,char grid[ROW][COLUMN])
{
    int flag = 0;
    
    if (grid[shot_x][shot_y] == ACTIVE) {
        grid[shot_x][shot_y] = DAMAGED;
        flag = 1;
    }
    else if (grid[shot_x][shot_y] == UNKNOWN) {
        grid[shot_x][shot_y] = VOID;
        flag = 0;
    }
    
    return flag;
}

void update_opponent_grid(int shot_x, int shot_y, char grid[ROW][COLUMN], int status)
{
    if (status == HIT)
        grid[shot_x][shot_y] = 'X';
    else if (status == MISS)
        grid[shot_x][shot_y] = 'O';
}