/* The Chase Project. Version 3.0 */
/******************************************************************************
 *  4. Level change sound
 *  7. General refactoring
 * 11. Stats are not properly cleared and reprinted. There are trailing zeros left behind.
 * 12. Visual - the "graphics" are not right.
 * 13. Randomisation broken (sometimes everything sits ontop of each other). By the 3rd level it stops working.
 * 14. Sounds and bombs visual.
 * 15. Reduce global variables.
 * 16. Remove cursor.
 * 17. Add README.
*****************************************************************************/

/******************************************************************************
 * Required libraries
 *****************************************************************************/
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>


/******************************************************************************
 * Required definitions
 *****************************************************************************/
#define ROWS 20
#define COLUMNS 60
#define OFFSETX 2
#define OFFSETY 4
#define MAXLEVELS 20
#define EMPTYCELL   100
#define ROBOTCELL   200
#define MANCELL     300
#define DEBRISCELL  900
#define ADVANCELEVEL 1000
#define VERSION 3.0


/******************************************************************************
 * Function Signatures
 *****************************************************************************/
void gotoxy(int x, int y);
void clear_screen( void );
void initialiseScreen(void);
void printStats(void);

void initializeMatrices(int offsetX, int offsetY, int matrix[COLUMNS][ROWS], int help[COLUMNS][ROWS]);
void randomiseCoordinates(int *x, int *y);

int calc(void);
int check(void);

void placeMan(int matrix[COLUMNS][ROWS], int help[COLUMNS][ROWS], int x, int y);
void placeRobot(int matrix[COLUMNS][ROWS], int help[COLUMNS][ROWS], int x, int y);
void placeEmpty(int matrix[COLUMNS][ROWS], int help[COLUMNS][ROWS], int x, int y);
void placeDebris(int matrix[COLUMNS][ROWS], int help[COLUMNS][ROWS], int x, int y);
void placeItem(int matrix[COLUMNS][ROWS], int help[COLUMNS][ROWS], int x, int y, int type, char* typestr);
void placeBomb(int *x, int *y);
void reduceRobots(void);

int keyboard(void);
void gameOver(void);


/******************************************************************************
 * Global Variables
 *****************************************************************************/
char keyb;
time_t t;
int c, k, i, j, level, robots, score, a, b, bombs, level2;
int matrix[COLUMNS][ROWS], help[COLUMNS][ROWS];


/******************************************************************************
 * Function Main
 *****************************************************************************/
int main(void)
{
    // screen initialisation
    initialiseScreen();
    
    // initialize matrices
    initializeMatrices(OFFSETX, OFFSETY, matrix, help);

    for (level=1; level<MAXLEVELS;)
    {
        // game control (keyboard driven)
        if (keyboard()==ADVANCELEVEL)
        {
            level++;
            matrix [a][b] = 0;
            gotoxy(a,b);
            printf(" ");
        }
        else
            break;
     }
     return;
}


/******************************************************************************
 * Game Control
 *****************************************************************************/
int keyboard()
{
    robots = level * 5;
    bombs += 5;

    // print the stats right before the user presses a button
    printStats();

    keyb = 'w';

    // place the robots
    for (i=0; (i < (level) * 5); i++)
    {
        randomiseCoordinates(&a, &b);
        placeRobot(matrix, help, a, b);
    }

    // place the man
    randomiseCoordinates(&a, &b);
    placeMan(matrix, help, a, b);

    // read user's input
    while ((keyb != 'q') || (keyb != 'Q'))
    {
        keyb = _getch();

        switch(keyb)
        {
            case '1': if ( b < 23 && a > 2 )
            {
                placeEmpty(matrix, help, a, b);
                b++; a--;
                placeMan(matrix, help, a, b);

                if (check()==ADVANCELEVEL) return ADVANCELEVEL;
                break;
            }
            else break;

            case '2': if (b < 23)
            {
                placeEmpty(matrix, help, a, b);
                b++;
                placeMan(matrix, help, a, b);

                if (check()==ADVANCELEVEL) return ADVANCELEVEL;
                break;
            }
            else break;

            case '3': if ( b < 23 && a < 61 )
            {
                placeEmpty(matrix, help, a, b);
                b++; a++;
                placeMan(matrix, help, a, b);

                if (check()==ADVANCELEVEL) return ADVANCELEVEL;
                break;
            }
            else break;

            case '4': if (a > 2)
            {
                placeEmpty(matrix, help, a, b);
                a--;
                placeMan(matrix, help, a, b);

                if (check()==ADVANCELEVEL) return ADVANCELEVEL;
                break;
            }
            else break;

            case '5':
            {
                placeMan(matrix, help, a, b);

                if (check()==ADVANCELEVEL) return ADVANCELEVEL;
                break;
            }

            case '6': if (a<61)
            {
                placeEmpty(matrix, help, a, b);
                a++;
                placeMan(matrix, help, a, b);

                if (check()==ADVANCELEVEL) return ADVANCELEVEL;
                break;
            }
            else break;

            case '7': if (b > 4 && a > 2)
            {
                placeEmpty(matrix, help, a, b);
                a--; b--;
                placeMan(matrix, help, a, b);

                if (check()==ADVANCELEVEL) return ADVANCELEVEL;
                break;
            }
            else break;

            case '8': if (b > 4)
            {
                placeEmpty(matrix, help, a, b);
                b--;
                placeMan(matrix, help, a, b);

                if (check()==ADVANCELEVEL) return ADVANCELEVEL;
                break;
            }
            else break;

            case '9': if (b > 4 && a < 61)
            {
                placeEmpty(matrix, help, a, b);
                a++; b--;
                placeMan(matrix, help, a, b);

                if (check()==ADVANCELEVEL) return ADVANCELEVEL;
                break;
            }
            else break;

            case '+':
            {
                while (check() != ADVANCELEVEL)
                {
                    placeMan(matrix, help, a, b);
                    // delay(100);
                }
                return ADVANCELEVEL;
            }

            case 'q': return 5000;
            case 'Q': return 5000;

            case 't':
            {
                placeEmpty(matrix, help, a, b);
                randomiseCoordinates(&a, &b);
                placeMan(matrix, help, a, b);

                if (check()==ADVANCELEVEL) return ADVANCELEVEL;
            }
            break;

            case 'T':
            {
                placeEmpty(matrix, help, a, b);
                randomiseCoordinates(&a, &b);
                placeMan(matrix, help, a, b);

                if (check()==ADVANCELEVEL) return ADVANCELEVEL;
            }
            break;

            case 'b':
            {
                bombs--;

                gotoxy(76,12);
                printf("%d",bombs);

                if (matrix[a-1][b-1] == ROBOTCELL)
                {
                    reduceRobots();
                    placeEmpty(matrix, help, a-1, b-1);
                    if (robots==0) return ADVANCELEVEL;
                }

                if (matrix[a-1][b] == ROBOTCELL)
                {
                    reduceRobots();
                    placeEmpty(matrix, help, a-1, b);
                    if (robots==0) return ADVANCELEVEL;
                }

                if (matrix[a-1][b+1] == ROBOTCELL)
                {
                    reduceRobots();
                    placeEmpty(matrix, help, a-1, b+1);
                    if (robots==0) return ADVANCELEVEL;
                }

                if (matrix[a][b-1] == ROBOTCELL)
                {
                    reduceRobots();
                    placeEmpty(matrix, help, a, b-1);
                    if (robots==0) return ADVANCELEVEL;
                }

                if (matrix[a][b+1] == ROBOTCELL)
                {
                    reduceRobots();
                    placeEmpty(matrix, help, a, b+1);
                    if (robots==0) return ADVANCELEVEL;
                }

                if (matrix[a+1][b-1] == ROBOTCELL)
                {
                    reduceRobots();
                    placeEmpty(matrix, help, a+1, b-1);
                    if (robots==0) return ADVANCELEVEL;
                }

                if (matrix[a+1][b] == ROBOTCELL)
                {
                    reduceRobots();
                    placeEmpty(matrix, help, a+1, b);
                    if (robots==0) return ADVANCELEVEL;
                }

                if (matrix[a+1][b+1] == ROBOTCELL)
                {
                    reduceRobots();
                    placeEmpty(matrix, help, a+1, b+1);
                    if (robots==0) return ADVANCELEVEL;
                }

                placeBomb(&a, &b);
                break;
            }

        } // end switch

        // print the stats right at the end of the loop to update with the latest
        printStats();
    } // end while

    return 0;

} // end function

int calc()
{
    // if a robot hits on a robot cell
    if (matrix[i][j] == ROBOTCELL)
    {
        // place debris
        placeDebris(matrix, help, i, j);

        // reduce robots by 2
        reduceRobots();
        reduceRobots();

        // if no robots left - return change level
        if (robots==0) return ADVANCELEVEL;
    }
    // if a robot hits you - you fucked
    else if (matrix[i][j] == MANCELL)
    {
        // place debris
        placeRobot(matrix, help, i, j);
        gameOver();
    }
    else if (matrix[i][j] == DEBRISCELL)
    {
        // place debris
        placeDebris(matrix, help, i, j);

        // reduce robots by 1
        reduceRobots();

        if (robots==0) return ADVANCELEVEL;

    }
    else
    {
        gotoxy(i,j);
        printf("R");
        matrix [i][j] = ROBOTCELL;
    }
    return 0;
}

int check()
{
        for (i=2; i<62; i++)
        {
            for (j=4; j<24; j++)
            {
                if (matrix[i][j] == ROBOTCELL && help[i][j] == ROBOTCELL)
                {
                    if (abs(i-a) > abs(j-b))
                    {
                        if (i-a > 0)
                        {
                            placeEmpty(matrix, matrix, i, j);
                            i--;
                            gotoxy(i,j);

                            if (calc()==ADVANCELEVEL) return ADVANCELEVEL;
                            i++;
                        }
                        else
                        {
                            placeEmpty(matrix, matrix, i, j);
                            i++;
                            gotoxy(i,j);

                            if (calc()==ADVANCELEVEL) return ADVANCELEVEL;
                            i--;
                        }
                    }
                    else if (abs(i-a) < abs(j-b))
                    {
                        if (j-b>0)
                        {
                            placeEmpty(matrix, matrix, i, j);
                            j--;
                            gotoxy(i,j);

                            if (calc()==ADVANCELEVEL) return ADVANCELEVEL;
                            j++;
                        }
                        else
                        {
                            placeEmpty(matrix, matrix, i, j);
                            j++;
                            gotoxy(i,j);

                            if (calc()==ADVANCELEVEL) return ADVANCELEVEL;
                            j--;
                        }
                    }
                else if (abs(i-a) == abs(j-b))
                {
                    if (j-b>0 && i-a>0)
                    {
                        placeEmpty(matrix, matrix, i, j);
                        i--; j--;
                        gotoxy(i,j);

                        if (calc()==ADVANCELEVEL) return ADVANCELEVEL;
                        i++; j++;
                    }
                    else if (j-b<0 && i-a>0)
                    {
                        placeEmpty(matrix, matrix, i, j);
                        i--; j++;
                        gotoxy(i,j);

                        if (calc()==ADVANCELEVEL) return ADVANCELEVEL;
                        i++; j--;
                    }
                    else if (j-b<0 && i-a<0)
                    {
                        placeEmpty(matrix, matrix, i, j);
                        i++; j++;
                        gotoxy(i,j);

                        if (calc()==ADVANCELEVEL) return ADVANCELEVEL;
                        i--; j--;
                    }
                    else if (j-b>0 && i-a<0)
                    {
                            placeEmpty(matrix, matrix, i, j);
                            i++; j--;
                            gotoxy(i,j);

                            if (calc()==ADVANCELEVEL) return ADVANCELEVEL;
                            i--; j++;
                    }
                } // main if
            } // end third for
        } // end second for
    } // end first for

    level2 = level;

    // copy matrices?
    for (i=2; i<62; i++)
        for (j=4; j<24; j++)
            help [i] [j] = matrix [i] [j];

    return 0;
} // end function


/******************************************************************************
 * Helper Functions
 *****************************************************************************/

void gotoxy(int x, int y)
{
    COORD c = { x, y };  
    SetConsoleCursorPosition(  GetStdHandle(STD_OUTPUT_HANDLE) , c);
}

void clear_screen( void )
{
  DWORD n;                         /* Number of characters written */
  DWORD size;                      /* number of visible characters */
  COORD coord = {0};               /* Top left screen position */
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  /* Get a handle to the console */
  HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );

  GetConsoleScreenBufferInfo ( h, &csbi );

  /* Find the number of characters to overwrite */
  size = csbi.dwSize.X * csbi.dwSize.Y;

  /* Overwrite the screen buffer with whitespace */
  FillConsoleOutputCharacter ( h, TEXT ( ' ' ), size, coord, &n );
  GetConsoleScreenBufferInfo ( h, &csbi );
  FillConsoleOutputAttribute ( h, csbi.wAttributes, size, coord, &n );

  /* Reset the cursor to the top left position */
  SetConsoleCursorPosition ( h, coord );
}

/* 
 * Supply 2 references to 2 integers and populate them using a random number
 * seed from the current time. The random coordinates are within the game
 * limits (ROWS, COLUMNS).
 */
void randomiseCoordinates(int *x, int *y)
{
    int maxx, maxy;
    
    // the seed for our randomisation
    srand((unsigned) time(&t));

    // set the max x and y
    maxx = COLUMNS + OFFSETX - 1;
    maxy = ROWS + OFFSETY - 1;

    // get the initial x,y coordinates
    *x = (rand() % (maxx));
    *y = (rand() % (maxy));

    // check if they are EMPTY and recalculate if not
    while (*x < OFFSETX || *x > COLUMNS + 1 || matrix [*x][*y] != EMPTYCELL)
        *x = (rand() % maxx);

    while (*y < OFFSETY || *y > ROWS + 1 || matrix [*x][*y] != EMPTYCELL)
        *y = (rand() % maxy);
}

void initialiseScreen()
{
    // FIXME: remove the cursor and clear the screen
    //_setcursortype(_NOCURSOR);
    clear_screen();

    // main frame
    for (i=2; i<80; i++)
    {
        gotoxy(i,1);
        printf("-");
        gotoxy(i,3);
        printf("-");
        gotoxy(i,24);
        printf("-");
        gotoxy(i,25);
        printf("-");
    }

    for (i=2; i<25; i++)
    {
        gotoxy(1,i);
        printf("|");
        gotoxy(62,i);
        printf("|");
        gotoxy(80,i);
        printf("|");
    }

    gotoxy(1,1);
    printf("�");
    gotoxy(80,1);
    printf("�");
    gotoxy(1,24);
    printf("�");
    gotoxy(1,25);
    printf("�");
    gotoxy(78,25);
    printf("�");
    gotoxy(78,24);
    printf("�");

    gotoxy(62,1);
    printf("�");
    gotoxy(62,3);
    printf("�");
    gotoxy(62,24);
    printf("�");

    gotoxy(1,3);
    printf("�");
    gotoxy(1,24);
    printf("�");

    gotoxy(80,3);
    printf("�");
    gotoxy(80,24);
    printf("�");

    gotoxy(62,25);
    printf("�");

    gotoxy(64,16);
    printf(" ��    ���   �");
    gotoxy(64,17);
    printf("����� ����� �");
    gotoxy(64,18);
    printf("��� ��� ������");
    gotoxy(64,19);
    printf("���     ���  ��");
    gotoxy(64,20);
    printf("���     ���  ���");
    gotoxy(64,22);
    printf("   creations (R)");
    gotoxy(79,25);
    printf(" ");
}

void printStats()
{
    gotoxy(3,2);
    printf("The Chase v%2.1f                           Manos Korakakis", VERSION);
    gotoxy(64,2);
    printf(" Information :");
    gotoxy(65,5);
    printf("Score  :   %d", score);
    gotoxy(65,8);
    printf("Level  :   %d", level);
    gotoxy(65,10);
    printf("Robots :   %d", robots);
    gotoxy(65,12);
    printf("Bombs  :   %d", bombs);
}

void gameOver(void)
{
    gotoxy(8,10);
    printf("����   ��    �   � ���       ��  �   � ���  ���");
    gotoxy(8,11);
    printf("��     � �   �� �� �        �  �  �  � �    �  �");
    gotoxy(8,12);
    printf("��   � ����  � � � ��       �  �  � �  ��   ���");
    gotoxy(8,13);
    printf("������ �  �  �   � ����      ��    �   ���� �  �");

    for (i=0; i<1; i++)
    {
        //sound(100);
        //delay(100);
        //sound(200);
        //delay(100);

        //sound(500);
        //sound(40);
        //delay(100);
        //nosound();
    }

    getch();
    exit(1 - '0');
}

void initializeMatrices(
    int offsetX,
    int offsetY,
    int matrix[COLUMNS][ROWS],
    int help[COLUMNS][ROWS])
{
    for (a=offsetX; a<COLUMNS+offsetX; a++)
        for (b=offsetY; b<ROWS+offsetY; b++)
        {
            matrix[a][b] = EMPTYCELL;
            help[a][b] = EMPTYCELL;
        }
}

void placeMan(
    int matrix[COLUMNS][ROWS],
    int help[COLUMNS][ROWS],
    int x,
    int y)
{
    placeItem(matrix, help, x, y, MANCELL, "M");
}

void placeRobot(
    int matrix[COLUMNS][ROWS],
    int help[COLUMNS][ROWS],
    int x,
    int y)
{
    placeItem(matrix, help, x, y, ROBOTCELL, "R");
}

void placeEmpty(
    int matrix[COLUMNS][ROWS],
    int help[COLUMNS][ROWS],
    int x,
    int y)
{
    placeItem(matrix, help, x, y, EMPTYCELL, " ");
}

void placeDebris(
    int matrix[COLUMNS][ROWS],
    int help[COLUMNS][ROWS],
    int x,
    int y)
{
    placeItem(matrix, help, x, y, DEBRISCELL, "#");
}

void placeItem(
    int matrix[COLUMNS][ROWS],
    int help[COLUMNS][ROWS],
    int x,
    int y,
    int type,
    char* typestr)
{
        matrix[x][y] = type;
        help[x][y] = type;
        gotoxy(x, y);
        printf(typestr);
}

void reduceRobots()
{
    // reduce robots by one
    robots--;
    
    // increase score by one
    score++;
}

void placeBomb(int *x, int *y)
{
    int k, c, d;

    if (bombs > -1)
    {
        for (k=1; k<10; k++)
        {
            for (c=1; c<30; c++)
            {
                if (*x>2)
                {
                    if (*y>4)
                    {
                        gotoxy(*x-1, *y-1);
                        printf("*");
                    }

                    gotoxy(*x-1, *y); printf("*");

                    if (*y<23)
                    {
                        gotoxy(*x-1, *y+1);
                        printf("*");
                    }
                }

                if (*y>4)
                {
                    gotoxy(*x, *y-1);
                    printf("*");
                }

                if (*y<23)
                {
                    gotoxy(*x, *y+1);
                    printf("*");
                }

                if (*x<61)
                {
                    if (*y>4)
                    {
                        gotoxy(*x+1, *y-1);
                        printf("*");
                    }

                    gotoxy(*x+1, *y); printf("*");

                    if (*y<23)
                    {
                        gotoxy(*x+1, *y+1);
                        printf("*");
                    }
                }
            }

            for (d=1; d<30; d++)
            {
                if (*x>2)
                {
                    if (*y>4)
                    {
                        gotoxy(*x-1, *y-1);
                        printf(" ");
                    }

                    gotoxy(*x-1, *y); printf(" ");

                    if (*y<23)
                    {
                        gotoxy(*x-1, *y+1);
                        printf(" ");
                    }
                }

                if (*y>4)
                {
                    gotoxy(*x, *y-1);
                    printf(" ");
                }

                if (*y<23)
                {
                    gotoxy(*x, *y+1);
                    printf(" ");
                }

                if (*x<61)
                {
                    if (*y>4)
                    {
                        gotoxy(*x+1, *y-1);
                        printf(" ");
                    }

                    gotoxy(*x+1, *y); printf(" ");

                    if (*y<23)
                    {
                        gotoxy(*x+1, *y+1);
                        printf(" ");
                    }
                }
            }
        }

        //sound(100);
        //delay(100);
        //sound(200);
        //delay(100);
        //sound(500);
        //sound(40);
        //delay(100);
        //nosound();
    }
    else
    {
        //sound(200);
        //delay(300);
        //nosound();
    }
}
