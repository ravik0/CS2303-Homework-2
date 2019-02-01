/*
 * tests.c
 *
 *  Created on: Nov 3, 2018
 *      Author: student
 */
#include <stdbool.h>
#include "tests.h"
#include "production.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for strncmp

bool tests(void)
{
	bool results=false;
	puts("During tests");
	bool ok1 = testReadInput();
	if(ok1)puts("Found and read the test file.");
	bool ok2 = testMakeArrays();
	if(ok2)puts("Was able to allocate the arrays ok.");
	bool ok3 = testPlayOne();
	if(ok3)puts("playOne is ok.");
	bool ok4 = testNeighbors();
	if(ok4)puts("howManyNeighbors is ok.");
	bool ok5 = testAnyX();
	if(ok5)puts("anyX is ok.");
	bool ok6 = testSameContent();
	if(ok6)puts("sameContent is ok.");
	puts("end of tests");
	results = ok1 && ok2 && ok3 && ok4 && ok5 && ok6;
	printf("tests returning %d.\n",results);
	return results;
}
bool testReadInput(void)
{
	//the plan is:
	//create a file
	//make sure it can be opened.
	//try a file that should not exist
	//check that we notice it does not exist

	bool results = false;
	bool ok1 = false;
	bool ok2 = false;
	bool ok3 = false;
	//
	//Here goes: create a file
	FILE* fp;
	fp = fopen("/tmp/LifeInput.txt", "w"); //we write it first for later reading
	if (fp != false)
	{//it opened, yay!
		//file containing a sequence of lines,
		//each consisting of a sequence of 'x' and 'o' characters,
		//indicating the occupied and unoccupied cells
		//of the initial configuration, e.g.
		//oxo
		//xox
		//xox
		//oxo
		int w1 = fprintf(fp, "oxo\n");//returns the number of chars excluding null
		int w2 = fprintf(fp, "xox\n");
		int w3 = fprintf(fp, "xox\n");
		int w4 = fprintf(fp, "oxo\n");
		if ((w1 == w2) && (w2 == w3) && (w3 == w4) && (w4 == 4))
		{
			ok1 = true;
		}
		fclose(fp);
	}
	fp = fopen("/tmp/LifeInput.txt", "r"); //we try to read it
	if (fp != false)
	{//it opened, yay!
		ok2 = true;
		//can we read the correct data?
		char oRow[4]; //test case, we know a string of 3 chars terminated by null
		int count =  fscanf(fp, "%s", oRow);
		//printf("First row is %s\n", oRow);
		if(0==strncmp(oRow, "oxo", 4))//0 means match
		{//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);
		//printf("Second row is %s\n", oRow);
		if(0==strncmp(oRow, "xox", 4))
		{//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);
		//printf("Third row is %s\n", oRow);
		if(0==strncmp(oRow, "xox", 4))
		{//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);
		//printf("Fourth row is %s\n", oRow);
		if(0==strncmp(oRow, "oxo", 4))
		{//got the right data
			ok3 = true;
		}
	}
	results = ok1 && ok2 && ok3;
	return results;
}
bool testMakeArrays(void)
{
	bool results = false;
	bool ok1 = false;
	//we need to allocate and populate arrays
	//In this test case we set our arrays to 4 rows x3 columns
	int nRows = 4;
	int nCols = 4;
	char boardBefore[4][4]={
			{'o','x','o','\0'},
			{'x','o','x','\0'},
			{'x','o','x','\0'},
			{'o','x','o','\0'}
	};

	char boardAfter[nRows][nCols];
	//here's the test
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			boardAfter[row][col]=boardBefore[row][col];
		}
	}
	ok1 = true;
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			if (boardAfter[row][col]!=boardBefore[row][col])
			{
				ok1=false;
			}
		}
	}

	results = ok1;
	return results;
}
bool testPlayOne(void)
{
	bool results = false;
	bool ok1 = false;
	int nRows = 4;
	int nCols = 3;
	char boardBefore[4][3]={
			{'o','x','o'},
			{'x','o','x'},
			{'x','o','x'},
			{'o','x','o'}
	};
	char correctBoardAfter[4][3]={
			{'o','x','o'},
			{'x','o','x'},
			{'x','o','x'},
			{'o','x','o'}
	};

	char boardAfter[nRows][nCols];
	//here's the invocation
	PlayOne(nRows, nCols, (char*)boardBefore, (char*)boardAfter);
	//here's the check
	ok1 = true; //no errors found yet
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			if(boardAfter[row][col]!=correctBoardAfter[row][col])
			{//error found
				ok1 = false;
			}
		}
	}
	results = ok1;
	return results;
}

bool testNeighbors(void) {
	char board[4][3]={
		{'o','x','o'},
		{'x','o','x'},
		{'x','x','x'},
		{'o','x','o'}
	};
	int neighborCount = HowManyNeighbors(0, 1, 4, 3, (char*)board);
	bool ok1 = neighborCount == 2;
	int count2 = HowManyNeighbors(2, 1, 4, 3, (char*)board);
	bool ok2 = count2 == 5;
	return ok1 && ok2;
}

bool testAnyX(void) {
	char board[3][3]={
		{'o','o','o'},
		{'o','o','o'},
		{'o','o','o'},
	};
	bool ok1 = anyX((char*)board, 3, 3);
	char board2[3][3]={
		{'o','o','o'},
		{'o','x','o'},
		{'o','o','o'},
	};
	bool ok2 = anyX((char*)board2, 3, 3);
	return !ok1 && ok2;
}

bool testSameContent(void) {
	char board1[3][3]={
		{'o','x','o'},
		{'o','x','o'},
		{'o','o','o'},
	};
	char board2[3][3]={
		{'o','x','o'},
		{'o','x','o'},
		{'o','o','o'},
	};
	char board3[3][3]={
		{'o','o','o'},
		{'o','x','o'},
		{'o','o','o'},
	};
	bool ok1 = sameContent((char*)board1, (char*)board2, 3, 3);
	bool ok2 = !sameContent((char*)board1, (char*)board3, 3, 3);
	bool ok3 = !sameContent((char*)board3, (char*)board2, 3, 3);
	return ok1 && ok2 && ok3;
}
