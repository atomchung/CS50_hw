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

//  Clears screen using ANSI escape sequences.

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
 
 // 因為原始是創造的 board[9][9] ，單因為d 可能會< 9 ，所以其他質沒有定義到，所以可以給成-1 來幫助 debug
void init(void)
{
    for (int i =0; i<d; i++)
    {
        for (int j=0; j <d; j++)
        {
            board[i][j] = d*d-i*(d)-j-1;
        }
    }
    // 創造一個空格來移動
    if ( d%2 == 0)
    {
        board[d-1][d-3] = 1;
        board[d-1][d-2] = 2;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int i=0; i<d; i++)
    {
        for(int j=0; j<d; j++)
        {
            if(board[i][j] == 0)
            {
                printf("  _");
            }
            else
            {
                printf("%3i" , board[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    // 透過 x y tilex tiley 記錄 0 與 tile 的座標
    int x = -1;
    int y = -1;
    int tilex = -1;
    int tiley = -1;
    for (int i=0; i<d; i++)
    {
        for (int j=0; j<d; j++)
        {
            // 找出中心點 0 的位置
            if(board[i][j]==0)
            {
                x = i;
                y = j;
            }
            // 透過給定的 tile 來判斷在矩陣中的位置
            else if(board[i][j]== tile)
            {
                tilex = i;
                tiley = j;
            }
        }
    }
    
    // 判斷輸入的 tile 不在九宮格數字中， 即 tile 的位置 board[z][a] 並沒有改變
    if  (tilex== -1 && tiley== -1 ) 
    {
        return false;
    }
    else
    {
        // 判斷是輸入的 tile 否是可以移動的值
        // 可移動的值為中心值上/ 下 / 左 / 右3
        if( ( tilex==x+1 && tiley==y) || (tilex==x-1 && tiley==y) || (tilex==x && tiley==y+1) || (tilex==x && tiley==y-1) )
        {
            // 創造新的board 交換 0 和 tile 的位置
            board[x][y]= tile;
            board[tilex][tiley]= 0;
            return true;
        }
        else
        {
            return false;
        }
    }
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // TODO
    // 如果從最左到最下每格都比較大的話 就贏惹
    for (int i =0; i<d-1; i++)
    {
        for (int j = 0; j <d-1; j++)
        {
            if ((board[i][j] < board[i][j+1]) && (board[i][j] < board[i+1][j]))
            {
                
            }
            else
            {
            return false;
            }
        }
    }
    return true;
}
