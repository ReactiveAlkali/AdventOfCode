#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define PREAMBLE_SIZE 25

long findInvalid( FILE *fp );
int checkValidity( long num, long nums[] );
long findWeakness( FILE *fp, long inval );
void setVars( long *sum, long *min, long *max );

int main(void) {
	long invalNum;
	long encryptWeakness;
	FILE *fp;
	
	// opens the input file
	fp = fopen("input", "r");
	if(fp == NULL)
		exit(EXIT_FAILURE);

	invalNum = findInvalid(fp);
	encryptWeakness = findWeakness( fp, invalNum );

	printf( "The encryption weakness is %ld\n", encryptWeakness );
	//printf("The first invalid number is %ld\n", invalNum);

	fclose(fp);
	exit(EXIT_SUCCESS);
}

/*
 * Finds the encryption weakness of the input file
 */
long findWeakness( FILE *fp, long inval ) {
	long sum;
	long min;
	long max;
	long num;
	fpos_t lastStart;	// the last position we started from

	setVars( &sum, &min, &max );
	fgetpos( fp, &lastStart );

	while( fscanf(fp, "%ld\n", &num) != EOF && sum != inval ) {
		if( sum < inval ) {
			sum += num;
			if( num < min )
				min = num;
			if( num > max )
				max = num;
		}
		else if( sum > inval ) {
			// reset our variables
			setVars( &sum, &min, &max );
			
			// set position to where we started last and start on next line
			fsetpos( fp, &lastStart );
			fscanf( fp, "%ld\n", &num );
			fgetpos( fp, &lastStart );
		}
	}

	return min + max;
}

/*
 * Sets the variables to their starting values
 */
void setVars( long *sum, long *min, long *max ) {
	*sum = 0;
	*min = LONG_MAX;
	*max = LONG_MIN;
}

/*
 * Finds and returns the first invalid number from the input file
 */
long findInvalid( FILE *fp ) {
	long nums[PREAMBLE_SIZE];
	long num;
	int index = 0;
	int full = 0;	// whether the nums array is completely filled
	int valid = 1;

	while( valid && fscanf(fp, "%ld\n", &num) != EOF ) {
		if( full )
			valid = checkValidity(num, nums);

		nums[index++] = num;
		// wrap around to beginning of array
		if( index == PREAMBLE_SIZE ) {
			index = 0;
			full = 1;
		}
	}

	rewind(fp);
	return num;
}

/*
 * Checks whether the given number is valid
 * Returns 1 if valid and 0 otherwise
 */
int checkValidity( long num, long nums[] ) {
	int valid = 0;

	for( int i = 0; i < PREAMBLE_SIZE - 1 && !valid; i++ ) {
		for( int j = i + 1; j < PREAMBLE_SIZE && !valid; j++ ) {
			valid = num == nums[i] + nums[j];
		}
	}

	return valid;
}
