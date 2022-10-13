#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv){
	if (argc != 2){
		// argc is right
		fprintf(stderr, "%s: invalid number of argcs\n", argv[0]);
		return 1;
	}
    char *p = argv[1]; // p point to str that contains formula

	printf("	.global main\n");

	printf("main:\n");
    /* type of arg is string, so it is needed to convert *long* type
     * strtol is the func.
    * &p(char**), to change p's value
    *
    * num (op num )(op num)
    * the first num to pass to a0
    */
	printf("	li a0, %ld\n", strtol(p, &p, 10));

    // parse op num
    while(*p){
        if (*p == '+'){
            ++p; // next char for '+'
        // parse num
        // addi rd, rs1, imm ==> rd = rs1 + imm
        printf("    addi a0, a0, %ld\n", strtol(p, &p, 10));
        continue;
        }
        if (*p == '-'){
            ++p;

        printf("    addi a0, a0, -%ld\n", strtol(p, &p, 10));
        continue;

        }
        // if exist unable to parse char, then report error
        fprintf(stderr, "Unexpected character: '%c'\n", *p);
        return 1;
    }

    // ret value of a0

	printf("	ret\n");

}
