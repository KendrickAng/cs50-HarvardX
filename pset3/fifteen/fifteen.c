/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

//blank tile coordinate holder
int x;
int y;
int blankTile;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    } 
    
    //save initial coordinates of the blank tile
    else {
        x = d - 1;
        y = d - 1;
    } 

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int buffer = -1;
    int counter = -1;
    
    //fill the board with numbers in descending order
    for(int i = 0; i < d; i++) {
        
        for(int j = 0; j < d; j++) {
            
            counter++;
            
            board[i][j] = d*d - 1 - counter;
             
        }
    }
    
    //swap 1 and 2 when d is even
    if(d % 2 == 0) {
                
                buffer = board[d-1][d-3];
                board[d-1][d-3] = board[d-1][d-2];
                board[d-1][d-2] = buffer;
                
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int i = 0; i < d; i++) {
        
        for(int j = 0; j < d; j++) {
            
            if(board[i][j] == 0) {
                
                printf("  _");
                
            } else {
                
                printf(" %2i", board[i][j]);
                
            }
        } printf("\n");
    } 
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    for(int i=0; i < d; i++) {
        
        for(int j = 0; j < d; j++) {
            
            if(board[i][j] == tile) {
                
                int blankTile = 0;
                
                //Check if tile is ajacent to blank tile
                if(((x == (i - 1)) && (j == y)) ||  ((x == (i + 1)) && (j == y)) ||
                ((i == x) && (y == (j - 1))) || ((i == x) && (y == (j + 1)))) {
                    
                    board[x][y] = tile;
                    board[i][j] = blankTile;
                    
                    //update coordinates of blank tile
                    x = i;
                    y = j;
                    
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int counter = 0;
    
    //Use counter to check that array is in ascending order
    for(int i = 0; i < d; i++) {
        
        for(int j = 0; j < d; j++) {
            
            if (!(i == d - 1 && j == d - 1)) {
            
                counter++;
                
                if (board[d-1][d-1] != 0) { 
                    
                    return false;
                }
            
                if(board[i][j] != counter) {
                
                    return false;
                
                }
            }
        }
    }
    return true;
}
