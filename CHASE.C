/* The Chase Project. Version 2.0 */

/*********************************************
 * Known Bugs
 *********************************************
 1. After changing bounds and using offsets there exists an area in the bottom of
		the screen that the robots cannot be in.
 2. Level change. (Probably due to 1).
 3. M cannot lose (again because of 1).
 4. Level change sound.
 5. Quit (case insensitivity).
 6. Bomb graphic effects.
 7. General refactoring
 8. Free move for robots (with +).
 9. Max number of robots
10. Teleport is not working always right (randomization)
*/

/*********************************************
 * Required Libraries
 *********************************************/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>

/*********************************************
 * Game Definitions
 *********************************************/

/* game size */
#define ROWS 20
#define COLUMNS 60
#define OFFSETX 2
#define OFFSETY 4

#define MAXLEVELS 20

/* cell definitions */
#define EMPTYCELL 	100
#define ROBOTCELL 	200
#define MANCELL  	300
#define DEBRISCELL 	900

#define ADVANCELEVEL 1000

#define VERSION 2.0

/*********************************************
 * Function Signatures
 *********************************************/

void initializeEnvironment(void);
void initializeMatrices(int offsetX, int offsetY, int matrix[COLUMNS][ROWS], int help[COLUMNS][ROWS]);
void randomizeCoordinates(int *x, int *y);

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

/*********************************************
 * Global Variables
 *********************************************/

// int level, robots, score, bombs
char keyb;
time_t t;

int c, k, i, j, level, robots, score, a, b, bombs, level2;
int matrix[COLUMNS][ROWS], help[COLUMNS][ROWS];

/*********************************************
 * Function Main
 *********************************************/

int main(void)
{
	// environment initialization and welcome screen
	initializeEnvironment();

	// initialize matrices
	initializeMatrices(OFFSETX, OFFSETY, matrix, help);

	for (level=1; level<MAXLEVELS;)
	{
		gotoxy(76,8);
		printf("%d", level, " ");

		if (keyboard()==1000)
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


/*********************************************
 * Game Control
 *********************************************/

int keyboard()
{
	robots = level * 5;
	bombs += 5;
	gotoxy(76,10);
	printf("%d", robots, " ");
	gotoxy(76,12);
	printf("%d", bombs, " ");

	keyb = 'w';

	// the seed for our randomization
	srand((unsigned) time(&t));

	// place the robots
	for (i=0; (i < (level) * 5); i++)
	{
		randomizeCoordinates(&a, &b);
		placeRobot(matrix, help, a, b);
	}

	// place the man
	randomizeCoordinates(&a, &b);
	placeMan(matrix, help, a, b);

	// read user's input
	while ((keyb != 'q') || (keyb != 'Q'))
	{
		keyb = getch();

		switch(keyb)
		{
			case '1': if ( b < 23 && a > 2 )
			{
				placeEmpty(matrix, help, a, b);
				b++; a--;
				placeMan(matrix, help, a, b);

				if (check()==1000) return 1000;
				break;
			}
			else break;

			case '2': if (b < 23)
			{
				placeEmpty(matrix, help, a, b);
				b++;
				placeMan(matrix, help, a, b);

				if (check()==1000) return 1000;
				break;
			}
			else break;

			case '3': if ( b < 23 && a < 61 )
			{
				placeEmpty(matrix, help, a, b);
				b++; a++;
				placeMan(matrix, help, a, b);

				if (check()==1000) return 1000;
				break;
			}
			else break;

			case '4': if (a > 2)
			{
				placeEmpty(matrix, help, a, b);
				a--;
				placeMan(matrix, help, a, b);

				if (check()==1000) return 1000;
				break;
			}
			else break;

			case '5':
			{
				placeMan(matrix, help, a, b);

				if (check()==1000) return 1000;
				break;
			}

			case '6': if (a<61)
			{
				placeEmpty(matrix, help, a, b);
				a++;
				placeMan(matrix, help, a, b);

				if (check()==1000) return 1000;
				break;
			}
			else break;

			case '7': if (b > 4 && a > 2)
			{
				placeEmpty(matrix, help, a, b);
				a--; b--;
				placeMan(matrix, help, a, b);

				if (check()==1000) return 1000;
				break;
			}
			else break;

			case '8': if (b > 4)
			{
				placeEmpty(matrix, help, a, b);
				b--;
				placeMan(matrix, help, a, b);

				if (check()==1000) return 1000;
				break;
			}
			else break;

			case '9': if (b > 4 && a < 61)
			{
				placeEmpty(matrix, help, a, b);
				a++; b--;
				placeMan(matrix, help, a, b);

				if (check()==1000) return 1000;
				break;
			}
			else break;

			case '+':
			{
				while (check() != 1000)
				{
					placeMan(matrix, help, a, b);
					delay(100);
				}
				return 1000;
			}

			case 'q': return 5000;
			case 'Q': return 5000;

			case 't':
			{
				placeEmpty(matrix, help, a, b);
				randomizeCoordinates(&a, &b);
				placeMan(matrix, help, a, b);

				if (check()==1000) return 1000;
			}
			break;

			case 'T':
			{
				placeEmpty(matrix, help, a, b);
				randomizeCoordinates(&a, &b);
				placeMan(matrix, help, a, b);

				if (check()==1000) return 1000;
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
					if (robots==0) return 1000;
				}

				if (matrix[a-1][b] == ROBOTCELL)
				{
					reduceRobots();
					placeEmpty(matrix, help, a-1, b);
					if (robots==0) return 1000;
				}

				if (matrix[a-1][b+1] == ROBOTCELL)
				{
					reduceRobots();
					placeEmpty(matrix, help, a-1, b+1);
					if (robots==0) return 1000;
				}

				if (matrix[a][b-1] == ROBOTCELL)
				{
					reduceRobots();
					placeEmpty(matrix, help, a, b-1);
					if (robots==0) return 1000;
				}

				if (matrix[a][b+1] == ROBOTCELL)
				{
					reduceRobots();
					placeEmpty(matrix, help, a, b+1);
					if (robots==0) return 1000;
				}

				if (matrix[a+1][b-1] == ROBOTCELL)
				{
					reduceRobots();
					placeEmpty(matrix, help, a+1, b-1);
					if (robots==0) return 1000;
				}

				if (matrix[a+1][b] == ROBOTCELL)
				{
					reduceRobots();
					placeEmpty(matrix, help, a+1, b);
					if (robots==0) return 1000;
				}

				if (matrix[a+1][b+1] == ROBOTCELL)
				{
					reduceRobots();
					placeEmpty(matrix, help, a+1, b+1);
					if (robots==0) return 1000;
				}

				placeBomb(&a, &b);
				break;
			}

		} // end switch
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

		if (robots==0) return 1000;

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
	//for (i=50; i<64; i++)
	//	matrix[i][20] = EMPTYCELL;

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

							if (calc()==1000) return 1000;
							i++;
						}
						else
						{
							placeEmpty(matrix, matrix, i, j);
							i++;
							gotoxy(i,j);

							if (calc()==1000) return 1000;
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

							if (calc()==1000) return 1000;
							j++;
						}
						else
						{
							placeEmpty(matrix, matrix, i, j);
							j++;
							gotoxy(i,j);

							if (calc()==1000) return 1000;
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

						if (calc()==1000) return 1000;
						i++; j++;
					}
					else if (j-b<0 && i-a>0)
					{
						placeEmpty(matrix, matrix, i, j);
						i--; j++;
						gotoxy(i,j);

						if (calc()==1000) return 1000;
						i++; j--;
					}
					else if (j-b<0 && i-a<0)
					{
						placeEmpty(matrix, matrix, i, j);
						i++; j++;
						gotoxy(i,j);

						if (calc()==1000) return 1000;
						i--; j--;
					}
					else if (j-b>0 && i-a<0)
					{
							placeEmpty(matrix, matrix, i, j);
							i++; j--;
							gotoxy(i,j);

							if (calc()==1000) return 1000;
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


/*********************************************
 * Helper Functions
 *********************************************/

void randomizeCoordinates(int *x, int *y)
{
	*x = (rand() % 62);
	*y = (rand() % 23);

	while (*x < 2 || *x > 61 || matrix [*x][*y] != EMPTYCELL)
		*x = (rand() % 62);

	while (*y < 4 || *y > 23 || matrix [*x][*y] != EMPTYCELL)
		*y = (rand() % 23);
}

void initializeEnvironment()
{
	_setcursortype(_NOCURSOR);
	clrscr();

	for (i=2; i<80; i++)
	{
		gotoxy(i,1);
		printf("Э");
		gotoxy(i,3);
		printf("Э");
		gotoxy(i,24);
		printf("Э");
		gotoxy(i,25);
		printf("Э");
	}

	for (i=2; i<25; i++)
	{
		gotoxy(1,i);
		printf("К");
		gotoxy(62,i);
		printf("К");
		gotoxy(80,i);
		printf("К");
	}

	gotoxy(1,1);
	printf("Щ");
	gotoxy(80,1);
	printf("Л");
	gotoxy(1,24);
	printf("Ш");
	gotoxy(1,25);
	printf("Ш");
	gotoxy(78,25);
	printf("М");
	gotoxy(78,24);
	printf("Ы");

        gotoxy(62,1);
	printf("Ы");
	gotoxy(62,3);
	printf("Ю");
	gotoxy(62,24);
	printf("Ю");

        gotoxy(1,3);
	printf("Ь");
	gotoxy(1,24);
	printf("Ь");

	gotoxy(80,3);
	printf("Й");
	gotoxy(80,24);
	printf("М");

	gotoxy(62,25);
	printf("Ъ");
	gotoxy(3,2);
	printf("The Chase v%2.1f                           Manolis Korakakis", VERSION);
	gotoxy(64,2);
	printf(" Information :");
	gotoxy(65,5);
	printf("Score  :   0");
	gotoxy(65,8);
	printf("Level  :    ");
	gotoxy(65,10);
	printf("Robots :   5");
	gotoxy(65,12);
	printf("Bombs  :   5");
	gotoxy(64,16);
	printf(" лл    ллл   л");
	gotoxy(64,17);
	printf("ллллл ллллл л");
	gotoxy(64,18);
	printf("ллл ллл лллллл");
	gotoxy(64,19);
	printf("ллл     ллл  лл");
	gotoxy(64,20);
	printf("ллл     ллл  ллл");
	gotoxy(64,22);
	printf("   creations (R)");
	gotoxy(79,25);
	printf(" ");
}

void gameOver(void)
{
	gotoxy(8,10);
	printf("лллл   лл    л   л ллл       лл  л   л ллл  ллл");
	gotoxy(8,11);
	printf("лл     л л   лл лл л        л  л  л  л л    л  л");
	gotoxy(8,12);
	printf("лл   л лллл  л л л лл       л  л  л л  лл   ллл");
	gotoxy(8,13);
	printf("лллллл л  л  л   л лллл      лл    л   лллл л  л");

	for (i=0; i<1; i++)
	{
		sound(100);
		delay(100);
		sound(200);
		delay(100);

		sound(500);
		sound(40);
		delay(100);
		nosound();
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
	gotoxy(76,10);
	printf("%d", robots);

	// increase score by one
	score++;
	gotoxy(76,5);
	printf("%d",score);
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

		sound(100);
		delay(100);
		sound(200);
		delay(100);
		sound(500);
		sound(40);
		delay(100);
		nosound();
	}
	else
	{
		sound(200);
		delay(300);
		nosound();
	}
}
