#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OFFSET 97

int main(void) {
	char input[50];
	char initStr[50];
	int sum = 0;
	int yes[26] = {0};
	int first = 1;

	FILE *inputFile = fopen("input", "r");
	
	while(fgets(input, 50, inputFile)) {
		if(strcmp(input, "\n") == 0) {
			memset(yes, 0, sizeof(int) * 26);
			first = 1;
		}
		else {
			strtok(input, "\n");	// remove trailing newline

			if(first) {
				strcpy(initStr, input);
				
				for(int i = 0; i < strlen(initStr); i++) {
					int index = initStr[i] - OFFSET;

					sum++;
					yes[index] = 1;
				}
			}
			else {
				for(int i = 0; i < strlen(initStr); i++) {
					int index = initStr[i] - OFFSET;

					if(!strchr(input, initStr[i]) && yes[index]) {
						sum--;
						yes[index] = 0;
					}
				}
			}

			first = 0;
		}
	}

	fclose(inputFile);

	printf("The sum of those counts is %d\n", sum);

	return EXIT_SUCCESS;
}
