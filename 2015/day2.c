/*
 * Given an file containing a list of present box dimensions, the required amount of wrapping paper to
 * wrap these gifts is calculated
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

const char DELIM[2] = "x";

int wrappingPaper(int dimensions[]);
int calcRibbon(int dimen[]);

void main() {
	char input[16];		// a string to store input lines in
	int paper = 0;		// how much wrapping paper you need
	int ribbon = 0;

	FILE *inputFile = fopen("input.txt", "r");

	assert(inputFile != NULL);

	while(fgets(input, 16, inputFile) != NULL) {
		int dimensions[3];
		char *token;
		
		// extract dimensions from current input line
		token = strtok(input, DELIM);
		
		for(int i = 0; i < 3 && token != NULL; i++) {
			dimensions[i] = atoi(token);
			token = strtok(NULL, DELIM);
		}
		
		paper = paper + wrappingPaper(dimensions);

		ribbon = ribbon + calcRibbon(dimensions);
	}

	fclose(inputFile);

	printf("The amount of wrapping paper needed is %d sqft\n", paper);
	printf("The length of ribbon required is %d ft\n", ribbon);
}

// finds the length of ribbon required for a present given its dimensions
int calcRibbon(int dimen[]) {
	int vol = 1;
	int maxIndex = 0;
	int perim = 0;

	// find volume of present
	for(int i = 0; i < 3; i++) {
		vol = vol * dimen[i];
	}

	// find index of max dimension
	for(int i = 1; i < 3; i++) {
		if(dimen[i] > dimen[maxIndex])
			maxIndex = i;
	}

	// find the smallest perimeter
	for( int i = 0; i < 3; i++ ) {
		if( i != maxIndex )
			perim = perim + 2 * dimen[i];
	}

	return vol + perim;
}

// finds the wrapping paper required for a present given its dimensions
int wrappingPaper(int dimensions[]) {
	int sideAreas[3];
	int surfArea = 0;
	int minArea;

	// calculate side areas
	sideAreas[0] = dimensions[0] * dimensions[1];
	sideAreas[1] = dimensions[1] * dimensions[2];
	sideAreas[2] = dimensions[2] * dimensions[0];

	// find wrapping paper required by current present
	for(int i = 0; i < 3; i++) {
		surfArea = surfArea + 2 * sideAreas[i];
	}

	minArea = sideAreas[0];
	for(int i = 1; i < 3; i++) {
		if(sideAreas[i] < minArea)
			minArea = sideAreas[i];
	}

	return surfArea + minArea;
}
