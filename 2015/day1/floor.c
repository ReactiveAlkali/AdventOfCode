#include <stdio.h>
#include <string.h>

#define MAX_CHARS 10000

void main() {
	FILE *inputFile;

	int currFloor = 0;	// current floor
	int basementPos = 0;	// position of char that takes us to the basement	
	char input[MAX_CHARS];	// large string to store input in

	// get input
	inputFile = fopen("input.txt", "r");
	fgets(input, MAX_CHARS, inputFile);

	for(int i = 0; i < strlen(input); i++) {
		if(input[i] == '(') {
			currFloor++;
		}
		else if(input[i] == ')') {
			currFloor--;
		}

		// Finds position of first character that takes santa to the basement
		if(currFloor == -1 && basementPos == 0) { 
			basementPos = i + 1;
		}
	}

	fclose(inputFile);

	printf("The instructions take santa to floor %d\n", currFloor);
	printf("The position of the character that first takes us to the basement is %d\n", basementPos);
}
