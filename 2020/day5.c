#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getRow(char *str);
int getRowOrCol(char *str, char type);

int main(void) {
	int yourID = 0;
	//int highestID = 0;
	char input[50];  
	int ids[128*8] = {0};	// existant ids

	FILE *inputFile = fopen("input", "r");

	// process input lines from text file
	while(fgets(input, 50, inputFile) != NULL) {
		int row = getRowOrCol(input, 'r');
		int col = getRowOrCol(input, 'c');
		
		int id = row * 8 + col;
		ids[id] = 1;
		//if(id > highestID)
			//highestID = id;
	}

	while(ids[yourID] == 0)
		yourID++;
	while(ids[yourID] == 1)
		yourID++;

	fclose(inputFile);

	printf("Your seat ID is %d\n", yourID);
	//printf("The highest seat ID is %d\n", highestID);

	return EXIT_SUCCESS;
}

/*
 * Gets the column number from the last 3 characters of the given string
 * or gets the row number from the first 7 characters of the given string
 */
int getRowOrCol(char *str, char type) {
	char upDelim;
	char lowDelim;
	int start;
	int end;
	int mid;
	int high;
	int low = 0;

	switch(type) {
		case 'r':
			high = 127;
			start = 0;
			end = 7;
			lowDelim = 'F';
			upDelim = 'B';
			break;
		case 'c':
			high = 7;
			start = 7;
			end = 10;
			lowDelim = 'L';
			upDelim = 'R'; 
			break;
	}

	for(int i = start; i < end; i++) {
		if(str[i] == lowDelim) {
			mid = (low + high)/2;
			high = mid;
		}
		else if(str[i] == upDelim) {
			mid = (low + high)/2 + 1;
			low = mid;
		}
	}

	return mid;
}

/*
 * Gets the row number from the first seven characters of the given string
 */
int getRow(char *str) {
	int mid;
	int low = 0;
	int high = 127;

	for(int i = 0; i < 7; i++) {
		if(str[i] == 'F') {
			mid = (low + high)/2;
			high = mid;
		}
		else if(str[i] == 'B') {
			mid = (low + high)/2 + 1;
			low = mid;
		}
	}

	return mid;
}
