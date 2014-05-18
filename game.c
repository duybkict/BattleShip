/* Battleship: Developed by Robert Herrera, Danny Rodriguez , Fernando Torres. */
/*    ---  Developed and  Run  on  Dev C++     ---      */
/* Game resets if overlap is present */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define iWIN 9 // winnning value when you have hit all ships
#define nrows 12 // matrix must be higher than ten to instill boundary test
#define ncols 12 // matrix must be higher than ten to instill boundary test

typedef char Board;

/* function prototypes */
void welcome();// welcomes players and provides instructions if necessary

void initial_grid(char board[nrows][ncols]); // prints actual board of the game
void Computer_placement(char board[nrows][ncols], char ship[50], int size);// pseduo random algorithm for ship placement
void DisplayGrid(char board[nrows][ncols]); // displays current board positioning
void fire(char board[nrows][ncols],int x, int y); // fire function allow users to input coordinate of attack
void check_hit(char board[nrows][ncols],int x, int y); // checks status of coordinate and determines either hit or miss.
int check_2(char board[nrows][ncols],int x, int y); // checks grid that player sees to change
void beta_hit(char board[nrows][ncols], int x ,int y, int z); // prints respective x or o on screen

/* Battleship */
/*int main(int argc, char ** argv)
 {
 srand(time(NULL)); // randomizing function
 int game; // game loop
 int row,col; // coordinate opertors
 int counter; // keeps track of how many moves you make
 int trans;// transition value to print correct letter on X/O on screen
 int win; // when  player equals amount of ships on battlefield that have been hit
 char grid[12][12]; // grid that carries initial value and coordinates of ships from computer
 char grid_2[12][12];// "empty" grid that player sees
 int r,c,error;
 int begin;
 
 
 do{
 counter = 0;// initial counter value
 grid[nrows][ncols] = '~';//sets grids to tildes
 grid_2[nrows][ncols] = '~';//sets grids to tildes(ocean)
 r = 0;
 c = 0;
 error = 0;
 welcome();//instructions provided if necessary
 win = 0;// initial win value
 //system("PAUSE");
 game = 1; // initializes game and loop
 initial_grid(grid);// places grid in memory
 initial_grid(grid_2);// places grid in memory
 //system("CLS");// clears screen
 Computer_placement( grid, "destroyer" , 4); // char in the middle provide
 Computer_placement( grid, "cruiser" , 3);
 Computer_placement( grid, "vessel" , 2);
 Computer_placement( grid_2, "~XO" , 4); // initial character is tilde to
 Computer_placement( grid_2, "~XO" , 3);  // different values do matter since it is the overlay and only sets
 Computer_placement( grid_2, "~XO" , 2);
 
 for(r=0;r<10;r++)
 {
 for(c=0;c<10;c++)
 {
 if( grid[r][c] != '~'){       // if the amount of ships is less than 9 ships the game resets itself
 error++;
 }
 }
 }
 
 if( error == 8 || error != 9){
 printf(" ERROR: Only 8 ships were generated... Game reset\n\n");  // if the amount of ships is less than 9 ships the game resets itself
 begin = 1;
 }
 else{
 begin = 0;
 }
 }while(begin == 1);
 
 
 //DisplayGrid(grid);
 
 // }  cheat code**************************   ships presented if following codes are uncommented
 //system("pause");
 
 
 
 do{       // ships are placed
 
 DisplayGrid(grid_2);// "empty grid" is shown
 printf(" BEEP!  Enemies detected.... Fire at will!\n\n");
 printf(" Please enter the coordinates of the strike sir! \n\n"); // coordinates inputed by user
 printf(" Take Control of the artillery...\n\n\n");
 printf(" - enter a number then 'enter' key...\n\n\n");
 printf("(row): ");
 scanf("%d", &row);
 printf("(column): ");
 scanf("%d", &col);
 if( row > 9 || row<0 ||  col > 9 ||  col <0){
 printf("invalid entry... re-enter coordinates\n");
 //system("PAUSE");
 continue;
 }
 if(grid[row][col] == 'X' || grid[row][col] == 'O')
 {
 continue;
 }
 
 fire(grid,row,col);// re-enters coordinate as either hit or miss
 
 //check_2(grid,row, col);// returns integer value for hit or miss calculations and printing 'x' or 'o'
 trans = check_2(grid,row, col); // trans value is input to next function to tell an 'x' or 'o'
 
 if(trans == 1){ // TRANSintion code to transer coordinate information to player graph.
 win++;   // if X is printed (hit), then win increments until nine coodinates are hit
 }
 beta_hit(grid_2,row,col,trans);// prints x or o to grid 2 ( user)
 DisplayGrid(grid_2);// displays "current" grid postioning
 check_hit(grid,row,col); // prints hit or miss
 //system("pause");
 counter++;// counts moves made
 
 if(win == iWIN){
 game = 0;  // end_game
 }
 
 }while(game == 1 && win <= iWIN);
 if(game == 0){
 //system("CLS");
 printf("You win!\n");
 }
 if( game == 2){
 //system("PAUSE");
 return 0;
 }
 
 //system("PAUSE");
 return 0;
 
 }*//*  ------ End of Battleship --------- */

// Function definitions

void welcome() // welcome function
{
    printf(" ----------------------------------------------------- \n\n");
    printf("       *****    Welcome to Battleship    ********       \n\n");
    printf(" ----------------------------------------------------- \n\n\n\n");
    
    
    printf("Briefing:\n\nYour mission, should you choose to accept it, is to destroy all opposing enemy\nships.\n\n");
    printf("You will be shooting missiles by entering a coordinate (x,y),based on your\nreasoning you will be entering coordinates ");
    printf("until all enemy ships are destroyed  \n\n X = Hit!, O = Miss!,  ~ = Blank Space (sea)\n\n ");
    
} // end function

/**
 * Init board grid
 */

void initial_grid(char board[nrows][ncols]) //Sets the board initially to all open spaces before ships inhabit water.
{
    int i,j;
	for(i=0;i<10;i++)
	{                                            // printing algorithm
		for( j=0;j<10;j++)
		{
			board[i][j]='~';
		}
	}
} // end function

/**
 * Display board grid
 */

void DisplayGrid(char board[nrows][ncols]) //Displays the entire 10x10 board.
{
    int i,j;
	//system("CLS");
	printf("  0 1 2 3 4 5 6 7 8 9\n\n");      // printing algorithm
	for(i=0;i<10;i++)
	{
        printf("%d",i);
        for(j=0;j<10;j++)
		{
            printf(" %c", board[i][j]);
		}
        printf("\n");
	}
} // end function

void Computer_placement(char board[nrows][ncols], char ship[50], int size) // logic that places ships
{                                             // ^^ ship character ( ship[50] to give enough space for any name and avoid error)
	int row=0;
	int col=0;
	int go; // tests integer value if coordinate is true
	go=1;
	char dir;
	int test;// tests direction based on boundaries of matrix
	int ai;// random direction... may overlap
    
    
    do
	{
	    row = rand()%10;    /* } pseudo random coordinate system */
        col = rand()%10;
        
		if(row>9 || row<0 || col<0 || col>9)
		{
			go=0;
		}
		else if(board[row][col]!='~')
		{
			go=0;
		}
        else
            go = 1;
        
		if(go==0){
            row = rand()%10;
            col = rand()%10;
            
            if(row>9 || row<0 || col<0 || col>9) // just in case there is an error in modulus operator
            {
                go=0;
                continue;
            }
            else if(board[row][col]!='~') // if tilde is not present the coordinate is not available. Msy not be working
            {
                go=0;
                continue;
            }
            else
                go = 1;
        }
        
        
        
	}while(go==0);
    
    
    
	do
	{
        
        if(test==0){
            test=1;
        }
        ai = rand()%4+1;
        
        switch(ai){ // psedudo random direction
            case 1:
                dir = 'l'; // left
                break;
            case 2:
                dir = 'u'; // up
                break;
            case 3:
                dir = 'd'; // down
                break;
            case 4:
                dir = 'r'; // right
                break;
            default:
                test = 0; // re-loops if no direction is randomized
                continue;
                break;
        }
        
	    switch(dir)
		{
                int i,x; // i representing row, x representing column (j)
                
            case 'l':
                if(col-size>=1) // makes sure it is not past the boundary of the grid
                {
                    for(i=0;i<=size-1;i++)// increments row
                    {
                        if(board[row][col-i]!='~') // tests if present character is there.   ****************May not be working as overlap is possible.
                        {
                            test=0;
                            continue; // reloop
                        }
                    }
                }
                else
                {
                    test=0;
                    continue; // reloop
                }
                test = 1;
                if(test==1)
                {
                    for(x=0;x<=size-1;x++)  //increments column to print based on direction and value of x coordinate
                    {
                        board[row][col-x]=ship[0]; // the ships first letter
                    }
                }
                break;
				
                /* all cases below follow the same algorithm as above */
            case 'r':
                if(col+size<=10)
                {
                    for(i=0;i<=size-1;i++)
                    {
                        if(board[row][col+i]!= '~') // increments one column if anything not a tilde is present in array memory
                        {
                            test=0;
                            continue;
                        }
                    }
                }
                else
                {
                    test=0;
                    continue;
                }
                test = 1;
                if(test==1)
                {
                    for(x=0;x<=size-1;x++)
                    {
                        board[row][col+x]=ship[0]; // the ships first letter
                    }
                }
                break;
                
            case 'u':
                if(row-size>=1)
                {
                    for(i=0;i<=size-1;i++)
                    {
                        if(board[row-i][col]!= '~')  // increments one column if anything not a tilde is present in array memory
                        {
                            test=0;
                            continue;
                        }
                    }
                }
                else
                {
                    test=0;
                    continue;
                }
                test = 1;
                if(test==1)
                {
                    for(x=0;x<=size-1;x++)
                    {
                        board[row-x][col]=ship[0]; // the ships first letter
                    }
                }
                break;
                
            case 'd':
                if(row+size<=10)
                {
                    for(i=0;i<=size-1;i++)
                    {
                        if(board[row+i][col]!='~')
                        {
                            test=0;
                            continue;
                        }
                    }
                }
                else
                {
                    test=0;
                    continue;
                }
                test = 1;
                if(test==1)
                {
                    for(x=0;x<=size-1;x++) // incrementation by row
                    {
                        board[row+x][col]=ship[0]; // the ships first letter
                    }
                }
		}
		
	}while(test==0);
} // end function


void fire(char board[nrows][ncols], int x,int y){ // fire function allow users to input coordinate of attack
    
    char hit_miss[10];
    int status;
    status = 1;
    
    do{
        
        while( x > 9 || x<0 ||  y > 9 || y<0){
            printf("invalid entry... re-enter coordinates\n");   // avoids invalid entry after same coordinates are entered
            printf("(row)y: ");
            scanf("%d", &x);
            printf("(column)x: ");
            scanf("%d", &y);
        }
        
        if(board[x][y]  == 'O' || board[x][y]  == 'X'){
            status = 1;
        }
        if(board[x][y] == 'c' || board[x][y]== 'v' || board[x][y]== 'd'){
            hit_miss[0] = 'X';
            status = 0;
        }
        else {                    // prints respective x or o on background grid (grid 1)
            hit_miss[0] = 'O';
            status = 0;
        }
        
    } while(status==1);
    
    board[x][y]= hit_miss[0];
}  

int check_2(char board[nrows][ncols],int x, int y){ // fire function allow users to input coordinate of attack for second grid
    if(board[x][y]=='X'){
        return 1;
    }
    else
        return 0;
}

void beta_hit(char board[nrows][ncols], int x,int y, int z){ // prints respective x or o on screen
    
    if( z == 1 ){
        board[x][y] = 'X';
    } else
        board[x][y] = 'O';
}


void check_hit(char board[nrows][ncols],int x, int y){ // fire function allow users to input coordinate of attack
    if(board[x][y]=='X'){
        printf("HIT!\n\n");
    }
    else
        printf("MISS!\n\n");
}




