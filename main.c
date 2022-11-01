#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// kind of terminator
typedef enum {
    TK_PUNCT, // operator like '+' '-'
    TK_NUM, // number
    TK_EOF, // EOF of file
} TokenKind;

// struct of terminator
typedef struct Token Token;

struct Token {
    TokenKind kind; // kind
    Token *next;
    int val; // value
    char *loc; //
    int len; //length
};

// error info
static void error(char *fmt, ...) {
    va_list va;
    // va get all args from fmt
    va_start(va, fmt);
    // vfprintf put va_list type's args
    vfprintf(stderr, fmt, va);
    // add '\n' at end of newline
    fprintf(stderr, "\n");
    va_end(va);
    exit(1);
}

// judge Tok is special char
static bool equal(Token *tok, char *str){
    /*比较字符串LHS（左部），RHS（右部）的前N位，S2的长度应大于等于N.
      比较按照字典序，LHS<RHS回负值，LHS=RHS返回0，LHS>RHS返回正值
      同时确保，此处的Op位数=N
    */
    return memcmp(tok->loc, str, tok->len) == 0 && str[tok->len] == '\0';
}

// skip special str
static Token *skip(Token *tok, char *str){
    if (!equal(tok, str))
        error("expect '%s'", str);
    return tok->next;
}

// get number ==> return TK_NUM's val
static int get_number(Token *tok){
    if (tok->kind != TK_NUM)
        error("expect a num");
    return tok->val;
}

// return new token
static Token *new_token(TokenKind kind, char *start, char *end){
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->loc = start;
    tok->len = end - start;
    return tok;
}


// parse for terminator
static Token *tokenize(char *p){
    Token head = {};
    Token *cur = &head;

    while(*p){
        // skip all space
        if (isspace(*p)){
            ++p;
            continue;
        }
        // parse number
        // constructor
        if (isdigit(*p)){
            cur->next = new_token(TK_NUM, p, p);
            // pointer forward
            cur = cur->next;
            const char *old_ptr = p;
            cur->val = strtoul(p, &p, 10);
            cur->len = p - old_ptr;
            continue;
        }

        // parse operator
        if (*p == '+' || *p == '-'){
            cur->next = new_token(TK_PUNCT, p, p+1);
            cur = cur->next;
            ++p;
            continue;
        }

        error("invalid token: %c", *p);
    }
    // parse come to end, add EOF,
    cur->next = new_token(TK_EOF, p, p);
    return head.next;
}

int main (int argc, char ** argv){
	if (argc != 2){
		// argc is right
        error("%s, invalid number of arguments", argv[0]);
	}

    Token *tok = argv[1]; //

	printf("	.global main\n");

	printf("main:\n");
    /* type of arg is string, so it is needed to convert *long* type
     * strtol is the func.
    * &p(char**), to change p's value
    *
    * num (op num )(op num)
    * the first num to pass to a0
    */
	printf("	li a0, %d\n", get_number(tok));
    tok = tok->next;

    // parse op num
    while(tok->kind != TK_EOF){
        if (equal(tok, "+")){
            tok = tok->next;
            printf("    addi a0, a0, %d\n", get_number(tok));
            tok = tok->next;
            continue;
        }
        // not "+", to judge "-"
        // no subi, so use add "-%d"
        tok = skip(tok, "-");
        printf("        addi, a0, a0, -%d\n", get_number(tok));
        tok = tok->next;

    }

    // ret value of a0

	printf("	ret\n");

}
