#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv){
	if (argc != 2){
		// argc is right
		fprintf(stderr, "%s: invalid number of argcs\n", argv[0]);
		return 1;
	}

	printf("	.global main\n");

	printf("main:\n");
	printf("	li a0, %d\n", atoi(argv[1]));



	printf("	ret\n");

}
