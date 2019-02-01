/*
 * production.c
 *
 *  Created on: Nov 3, 2018
 *      Author: student
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "production.h"
#include <math.h>
#include <string.h>

/**
 * Runs the program by scanning in the file, and sending the file data over to be decrypted
 * and used by PlayOne and generation
 * @param int argc the number of arguments
 * @param char* argv the array of arguments, with [0] being the name of the program
 * @return true if succeed, false otherwise
 */
bool production(int argc, char* argv[])
{
	bool results = false;
	bool done = false;
	int nRows=-1;
	int nCols = -1;
	int gens = 0; //number of generations to play
	int howManyLinesInFile = 0;
	int maximumWidth = 0;
	char filename[100];
	for(int i= 0; i<100; i++)
	{
		filename[i]='\0';
	}
	char print = 'n';
	char pause = 'n';
	//etc.
	//get the NR NC gens input [print] [pause], Usage as needed.
	if(argc<5)//not all mandatory args provided
	{
		usage();
		done=true;
	}
	else if (argc >= 7)
	{
		pause = argv[6][0];
	}
	if (!done && (argc >= 6))
	{
		print = argv[5][0];
	}

	if(!done)//must be greater than or equal to 5, so get the mandatory vals
	{

		char* ptr=0;
		long nr_l = strtol(argv[1],&ptr,10);//get the NR
		nRows = (int)nr_l;
		long nc_l = strtol(argv[2],&ptr,10);//get the NC
		nCols = (int)nc_l;
		long ng_l = strtol(argv[3],&ptr,10);//get the gens
		gens = (int)ng_l;
		if(nRows<1)
		{
			printf("Usage: Rows should be greater than 0, received %d.\n",nRows);
			done = true;
		}
		if(nCols<1)
		{
			printf("Usage: Columns should be greater than 0, received %d.\n",nCols);
			done = true;
		}
		if(gens<1)
		{
			printf("Usage: Generations should be greater than 0, received %d.\n",gens);
			done = true;
		}

		strcpy(filename, argv[4]);
		//now we have the command line
		//Let's read the input file
		FILE* fp = fopen(filename, "r"); //we try to read it
		if (fp != false)
		{//it opened, yay!
			printf("Opened %s.\n",filename);
			//can we read the data?
			char oRow[100];
			//Let's find out how many lines there are, and
			//find out the maximum width
			bool doneReadingFile = false;
			while(!doneReadingFile)
			{
				oRow[0]='\0';
				fscanf(fp, "%s", oRow);
				if(strlen(oRow)!=0)//there was something there
				{
					howManyLinesInFile++;
					if(maximumWidth < strlen(oRow)) {
						maximumWidth = strlen(oRow); //if we find a new biggest row, thats our max width
					}
				}
				else
				{
					doneReadingFile = true;
					fclose(fp);
				}
				//if our file lines are greater than given nRows, increase nRows
				if(howManyLinesInFile > nRows) nRows = howManyLinesInFile;
				//if our max width is greater than given nCols, increase nCols
				if(maximumWidth > nCols) nCols = maximumWidth;
			}
		}//can read filename
		else
		{
			puts("Cannot find that file,");
			done = true;
		}
		if(!done)
		{
			char A[nRows][nCols];
			char B[nRows][nCols];
			char C[nRows][nCols];
			char* old_p=&A[0][0];
			char* new_p=&B[0][0];
			char* other_p=&C[0][0];

			for(int row = 0; row< nRows; row++)
			{
				for(int col = 0; col<nCols; col++)
				{
					A[row][col] = 'o';
					B[row][col] = 'o';
					C[row][col] = 'o';
					//initialize arrays to initially have all 'o's in them
				}
			}
			FILE* fp = fopen(filename, "r");//we read it before, we expect we can read it again
			readFileIntoArray(nRows, nCols, howManyLinesInFile,  maximumWidth, old_p, fp);

			int howManyGens = generate(gens,  nRows,  nCols,  old_p, new_p, other_p, print, pause);
			printf("Ran %d generations\n", howManyGens);
		}

	}//not done
	results = !done;
	return results;

}
/**
 * PlayOne carries out one generation
 * @param unsigned int nr, the number of rows in the petri dish
 * @param unsigned int nc, the number of columns in the petri dish
 * @param char* Old, the location of the upper left of starting petri dish
 * @param char* New, the location of the upper left of the ending petri dish
 * @return There is none, because results are in New array
 */
void PlayOne (unsigned int nr, unsigned int nc, char* Old, char* New)
{
	int nRows = nr;
	int nCols = nc;
	//loop over all locations in the new dish
	//figure out whether occupied or not
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			//There are three rules,
			// use occupied in old
			//use neighbors in old
			bool occupied = true;
			occupied = 	(getLetter(row,col,nCols, Old)=='x');

			if(occupied
					&&((HowManyNeighbors(row, col, nRows, nCols, Old)<2)
							|| (HowManyNeighbors(row, col, nRows, nCols, Old)>3)))
			{//Death rule
				*(New + (row * nCols) + col)='o';
			}
			else if(occupied)
			{//Survival rule
				*(New + (row * nCols) + col)='x';
			}
			else if(HowManyNeighbors(row, col, nRows, nCols, Old)==3)
			{//Birth rule
				*(New + (row * nCols) + col)='x';

			}
			else
			{
				*(New + (row * nCols) + col)='o';
			}
		}
	}
}

/**
 * Function to print how to give program inputs to the program in case the user puts in the wrong args
 * @param none
 * @return none
 */
void usage(void)
{
	puts("Usage: HW2 NR NC gens input [print] [pause]");
}

/**
 * Gets a character from a 2D array
 * @param int row the row number we want to go to
 * @param int col the column number we want to go to
 * @param int nCols the number of columns in the array
 * @param char* Old a pointer to the 2D array that we want to go through
 * @return char the letter found at OLd[row][col]
 */
char getLetter(int row, int col, int nCols, char* Old)
{
	return *(Old+ (row*nCols)+col);
}

/**
 * Figures out how many neighbors (defined as 'x's) a character has next to it in Old
 * @param int row the row for the character we are looking around
 * @param int col the column for the character we are looking around
 * @param int nRows the number of rows in Old
 * @param int nCols the number of columns in OLd
 * @param char* Old a pointer to the 2D array we are looking through
 * @return int a number between 0 and 8 for how many 'x's the character at Old[row][col] has around it
 */
int HowManyNeighbors(int row, int col, int nRows, int nCols, char* Old)
{
	int howManyN = 0;
	//there could be as many as 8 neighbors
	//cells on an edge or corner have fewer neighbors
	//we will search for neighbors clockwise from NorthWest
	if (row>0)
	{
		if(col>0)
		{
			if(getLetter(row-1, col-1,nCols, Old)=='x')//NW
			{
				howManyN++;
			}
		}
		if(getLetter(row-1, col,nCols, Old)=='x')//N
		{
			howManyN++;
		}
		if(col<(nCols-1))
		{
			if(getLetter(row-1, col+1,nCols, Old)=='x')//NE
			{
				howManyN++;
			}
		}
	}//can look north
	if(col>0)
	{
		if(getLetter(row, col-1,nCols, Old)=='x')//W
		{
			howManyN++;
		}
	}
	if(col<(nCols-1))
	{
		if(getLetter(row, col+1,nCols, Old)=='x')//E
		{
			howManyN++;
		}
	}
	if(row<nRows-1)
	{
		if(col>0)
		{
			if(getLetter(row+1, col-1,nCols, Old)=='x')//SW
			{
				howManyN++;
			}
		}
		if(getLetter(row+1, col,nCols, Old)=='x')//S
		{
			howManyN++;
		}
		if(col<(nCols-1))
		{
			if(getLetter(row+1, col+1,nCols, Old)=='x')//SE
			{
				howManyN++;
			}
		}
	}//can look south

	return howManyN;
}

/**
 * Takes a file that contains 'o's and 'x's and puts it into a 2D array
 * @param int nRows the number of rows of the 2D array
 * @param int nCols the number of columns in the 2D array
 * @param int howManyLinesInFile the number of lines in the file
 * @param int maximumWidth the maximum row size found in the file
 * @param char* ar_p a pointer to the 2D array that is going to be filled
 * @param FILE* fp a pointer to the file that we are reading
 * @return none
 */
void readFileIntoArray(int nRows, int nCols, int howManyLinesInFile, int maximumWidth, char* ar_p, FILE* fp)
{
	for(int row = 0; row< nRows; row++)
	{
		for(int col = 0; col<nCols; col++)
		{
			*(ar_p+(row*nCols)+col) = 'o';

		}
	}

	for(int frow=0; frow< howManyLinesInFile; frow++)
	{
		char fromFile[maximumWidth];

		for(int i = 0; i<maximumWidth; i++)
		{
			fromFile[i]='o';
		}
		fscanf(fp, "%s", fromFile);
		for(int fcol=0; fcol<maximumWidth; fcol++)
		{
			if(fromFile[fcol]=='x')
			{
				int targetRow = frow+(nRows-howManyLinesInFile)/2;
				int targetCol = fcol+(nCols-maximumWidth)/2;
				*(ar_p+(targetRow*nCols)+targetCol) = 'x';
			}
		}
	}
}

/**
 * Generates all generations of the given (valid!) gameboard until the end conditions are met or
 * you reach the maximum number of generations. The end conditions are if a pattern repeats
 * over one or two generations, or if everyone dies off
 * @param int gens the number of generations to run the program for
 * @param int nRows the number of rows in the 2D arrays
 * @param int nCols the number of colums in the 2D arrays
 * @param char* old_p a pointer to the initial 2D array that is being changed to the next generation
 * @param char* new_p a pointer to the new generation created by calling PlayOne on old_p
 * @param char* other_p a pointer to a generation one before old_p.
 * @param char print either 'y' or 'n', if 'y' the function prints every generation but if 'n'
 * it prints only the first and last generation
 * @param char pause either 'y' or 'n', if 'y' the function waits for a keypress before continuing to
 * the next generation, if 'n' it proceeds from one generation to the other automatically.
 * @return int the number of generations passed.
 */
int generate(int gens,  int nRows,  int nCols,  char* old_p, char* new_p, char* other_p, char print, char pause)
{
	int g = 0;
	bool done = false;
	bool firstTime = true;

	for(int gensDone = 0; !done && (gensDone<gens); gensDone++)
	{
		if(firstTime) {
			//if this is the first run, print initial configuration
			puts("Initial configuration");
			printThis(nRows, nCols, old_p);
		}
		if(!anyX(old_p, nRows, nCols))
		{//all organisms are dead
			done = true;
			puts("All organisms dead.");
			printThis(nRows, nCols, old_p);
		}
		PlayOne(nRows, nCols, old_p, new_p); //generate new board and put it in new_p
		g++;
		if(sameContent(old_p, new_p, nRows, nCols))
		{
			//if old_p and new_p have the same stuff in it, the board is now static and we have
			//reached a termination condition.
			done = true;
			puts("Pattern repeated in one generation.");
			printThis(nRows, nCols, old_p);
		}
		if(firstTime)
		{
			firstTime = false;
		}
		else
		{
			if(sameContent(new_p, other_p, nRows, nCols))
			{
				//if new_p and other_p have the same things, the board is stuck in a loop and we
				//have reached a termination condition.
				puts("Pattern repeated after two generations.");
				printThis(nRows, nCols, other_p);
				done= true;

			}
		}
		if(!done)
		{
			if(print=='y')
			{
				puts("New generation");
				printThis(nRows, nCols, new_p);
			}
			if(pause=='y')
			{
				puts("Paused waiting for input.");
				getc(stdin);//just waits for user input
			}
			char* temp = other_p;
			other_p = old_p;
			old_p = new_p;
			new_p = temp;
		}
		if(gensDone == gens-1) {
			puts("Final Configuration");
			printThis(nRows,nCols, new_p);
		}
	}//end of generations

	return g;
}

/**
 * Checks to see if a 2D array has any 'x's in it
 * @param char* arr the array to look through
 * @param nRows the number of rows in arr
 * @param nCols the number of columns in nCols
 * @return true if contains an x, false otherwise.
 */
bool anyX(char* arr, int nRows, int nCols)
{
	bool any = false;
	for(int row=0; !any && (row<nRows); row++)
	{
		for(int col=0; !any && (col< nCols); col++)
		{
			if(*(arr+(row*nCols)+col) == 'x')
			{
				any=true;
			}
		}
	}
	return any;
}

/**
 * Checks to see if two same-size 2D arrays have the same content
 * @param char* one_p one of the 2D arrays
 * @param char* another_p the other 2D array
 * @param int nRows the number of rows for both of the arrays
 * @param int nCols the number of columns for both of the arrays
 * @return true if same content, false otherwise
 */
bool sameContent(char* one_p, char* another_p, int nRows, int nCols)
{
	bool same = true; //for all the comparisons that have been made so far

	for(int row=0; same && (row<nRows); row++)
	{
		for(int col=0; same && (col< nCols); col++)
		{
			if(getLetter(row, col, nCols, one_p) != getLetter(row, col, nCols, another_p))
			{
				same=false;
			}
		}
	}
	return same;

}

/**
 * Prints the entire 2D array passed into it, giving a blank space if 'o' found and an 'x' if 'x' found
 * @param int nRows the number of rows in the 2D array
 * @param int nCols the number of columns in the 2D array
 * @param char* old_p the pointer to the 2D array
 * @return none
 */
void printThis(int nRows, int nCols, char* old_p)
{
	for(int row=0;row<nRows;row++)
	{
		for(int col=0;col<nCols;col++)
		{
			char letter = getLetter(row, col, nCols, old_p);
			if(letter == 'x') printf("%c", letter); //if its an 'x', print it
			else printf(" "); //otherwise space
		}
		printf("\n");
	}
}
