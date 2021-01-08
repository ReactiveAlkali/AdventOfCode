#include <stdio.h>
#include <stdlib.h>

void main() {
	int product;

	int input1;
	FILE *file1 = fopen("input", "r");

	while(fscanf(file1, "%d", &input1) != EOF) {
		int input2;
		FILE *file2 = fopen("input", "r");	//independent file descriptors

		while(fscanf(file2, "%d", &input2) != EOF) {
			int input3;
			FILE *file3 = fopen("input", "r");

			while(fscanf(file3, "%d", &input3) != EOF) {
				int sum = input1 + input2 + input3;
			
				if(sum == 2020) {
					product = input1 * input2 * input3;
					
					// close files and exit loop
					fclose(file2);
					fclose(file3);
					goto EXIT_LOOP;
				}
			}

			fclose(file3);
		}

		fclose(file2);
	}
	EXIT_LOOP:

	fclose(file1);

	printf("The product is %d\n", product);
}
