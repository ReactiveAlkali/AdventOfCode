#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
	char input[50];
	int len;
	long product = 1;
	
	int check4 = 0;	// boolean of whether to check tree for slope 4
	long trees[] = {0, 0, 0, 0, 0};
	int indexes[] = {1, 3, 5, 7, 1};
	int increments[] = {1, 3, 5, 7, 1};
	
	FILE *inputFile = fopen("input", "r");

	fgets(input, 50, inputFile);	// skip first line
	len = strlen(input) - 1;	// not sure why strlen gives a value one too many

	while(fgets(input, 50, inputFile) != NULL) {
		
		for(int i = 0; i < 5; i++) {
			int index = indexes[i];
				
			if(input[index] == '#' && (i != 4 || check4)) 
				trees[i]++;

			if(i != 4 || check4) 
				indexes[i] = indexes[i] + increments[i];

			if(indexes[i] >= len)
				indexes[i] = indexes[i] - len;
		}

		check4 = !check4;
	}

	for(int i = 0; i < 5; i++) 
		product = product * trees[i];

	printf("The product of trees encountered is %ld\n", product);

	return EXIT_SUCCESS;
}
