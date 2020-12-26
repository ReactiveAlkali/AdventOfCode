#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int strCount(char *str, char c);
int checkPos(char *str, char c, int pos);

int main(void) {
	int valid = 0;	// the number of valid passwords
	
	int num1;
	int num2;
	char letter;
	char password[50];

	FILE *inputFile = fopen("input", "r");

	while(fscanf(inputFile, "%d-%d %c: %s", &num1, &num2, &letter, password) != EOF) {
		//int count = strCount(password, letter);

		//if(count >= num1 && count <= num2)
			//valid++;

		// whether the letter occurs at a given position
		int pos1 = checkPos(password, letter, num1);
		int pos2 = checkPos(password, letter, num2);

		if(pos1 != pos2) 
			valid++;

	}

	fclose(inputFile);

	printf("The number of valid passwords is %d\n", valid);

	return EXIT_SUCCESS;
}

/*
 * Checks whether a given character occurs in the given position of the string
 * Note that the position of the first letter is 1
 *
 * Returns 1 if the character is located at the given position 0 if its not
 */
int checkPos(char *str, char c, int pos) {
	int there = 0;
	
	if(str[pos-1] == c)
		there = 1;
	
	return there;
}

/*
 * Counts the number of times a given character occurs whithin a a given string
 */
int strCount(char *str, char c) {
	int count = 0;
	int length = strlen(str);

	for(int i = 0; i < length; i++) {
		if(str[i] == c) {
			count++;
		}
	}
	
	return count;
}
