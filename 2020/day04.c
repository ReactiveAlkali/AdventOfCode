#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isValid(char *str, const char **fields);
void replaceTrailing(char *str, char replacement);
int getIndex(char *str, const char **strings);
int valHgt(char *hgt);
int valHcl(char *hcl);

int main(void) {
	const char *FIELDS[8] = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid"};

	int valid = 0;
	char input[100];
	char passport[100];

	FILE *inputFile = fopen("input", "r");

	while(fgets(input, 100, inputFile) != NULL) {
		strcpy(passport, input);
		replaceTrailing(passport, ' ');

		// get rest of passport info
		while(fgets(input, 100, inputFile) != NULL && strcmp(input, "\n") != 0) { 
			strcat(passport, input);
			replaceTrailing(passport, ' ');
		}
		replaceTrailing(passport, '\0');
	
		valid = valid + isValid(passport, FIELDS);
	}

	fclose(inputFile);

	printf("The number of valid passports is %d\n", valid);

	return EXIT_SUCCESS;
}

/*
 * Replaces the trailing character of a given string
 */
void replaceTrailing(char *str, char replacement) {
	int len = strlen(str);

	str[len-1] = replacement;
}

/*
 * Checks whether a given string contains a valid passport
 * Returns 0 if invalid and 1 if valid
 */
int isValid(char *str, const char **FIELDS) {
	int isValid;
	char *token;
	char field[6];
	char data[20];

	// check whether every field is present
	for(int i = 0; i < 7 && isValid; i++)
		isValid = strstr(str, FIELDS[i]) != NULL;

	// check whether each field has valid data
	token = strtok(str, " ");

	while(token != NULL && isValid) {
		int year;
		char valClrs[] = "amb blu brn gry grn hzl oth";

		token[3] = ' ';
		sscanf(token, "%s %s", field, data);

		switch(getIndex(field, FIELDS)) {
			case 0:
				year = atoi(data);
				isValid = year >= 1920 && year <= 2002;
				break;
			case 1:
				year = atoi(data);
				isValid = year >= 2010 && year <= 2020;
				break;
			case 2:
				year = atoi(data);
				isValid = year >= 2020 && year <= 2030;
				break;
			case 3:
				isValid = valHgt(data);
				break;
			case 4:
				isValid = valHcl(data);
				break;
			case 5:
				isValid = strstr(valClrs, data) != NULL;
				break;
			case 6:
				isValid = strlen(data) == 9 && atoi(data) != 0;
				break;
			case 7:
				break;
		}
		
		token = strtok(NULL, " ");
	}

	return isValid;
}

/*
 * Checks whether a given hair colour is valid
 */
int valHcl(char *hcl) {
	int valid = hcl[0] == '#' && strlen(hcl) == 7;

	int numChar = 0;
	const char VAL_CHARS[] = "0123456789abcdef";

	for(int i = 1; i < (strlen(hcl) - 1) && valid; i++) 
		valid = strchr(VAL_CHARS, hcl[i]) != NULL;

	return valid;
}

/*
 * Checks whether a given height value is valid
 */
int valHgt(char *hgt) {
	int valid = 0;
	int num = atoi(hgt);

	if(strstr(hgt, "cm") != NULL)
		valid = num >= 150 && num <= 193;
	else if(strstr(hgt, "in") != NULL)
		valid = num >= 59 && num <= 76;

	return valid;
}
/*
 * Gets the index of the given field from an array of fields
 * Returns -1 if the given string is not found
 */
int getIndex(char *field, const char **FIELDS) {
	int index = -1;

	for(int i = 0; i < 8 && index == -1; i++) {
		if(strcmp(FIELDS[i], field) == 0)
			index = i;
	}

	return index;
}
