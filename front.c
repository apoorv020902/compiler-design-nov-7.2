/**
 * This the example lexical analyzer code in pages 173 - 177 of the
 * textbook,
 *
 * Sebesta, R. W. (2012). Concepts of Programming Languages. 
 * Pearson, 10th edition.
 *
 */

/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>

#include "front.h"

/* Global Variable */
int nextToken;

/* Local Variables */
static int charClass;
static char lexeme [100];
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();
static char* tokenCodeToName(int current_code);

/******************************************************/
/* main driver */
int main (int argumentCount, char **argumentValues) 
{   /*printing my R number for grading purposes*/
    printf("Cooke Analyzer :: R11723071\n\n");

    /*Error Handling; Making sure only one argument is passes from the command line */
        if (argumentCount != 2) {
        fprintf("Usage Error: Expected syntax - cooke_analyzer <path_to_source_file> \n");
        return 1;
    }

    /* Open the input data file and process its contents */
    if ((in_fp = fopen(argumentValues[1], "r")) == NULL) {
        printf("ERROR - cannot open / find %s \n", argumentValues[1]);
    } else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }
    fclose(in_fp);
    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the 
 * token */
static int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = OPEN_PAREN;
            break;
        case ')':
            addChar();
            nextToken = CLOSE_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        case '=':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                nextToken = EQUAL_OP;
            }
            else{
                ungetc(nextChar, in_fp);
                nextToken = ASSIGN_OP;
            }
            break;
        case '<':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                nextToken = LEQUAL_OP;
            }
            else{
                ungetc(nextChar, in_fp);
                nextToken = LESSER_OP;
            }
            break;
        case '>':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                nextToken = GEQUAL_OP;
            }
            else{
                ungetc(nextChar, in_fp);
                nextToken = GREATER_OP;
            }
            break;
        case '!':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                nextToken = NEQUAL_OP;
            }
            else{
                ungetc(nextChar, in_fp);
                nextToken = BOOL_NOT;
            }
            break;
        case '%':
            addChar();
            nextToken = MOD_OP;
            break;
        case '&':
            addChar();
            getChar();
            if (nextChar == '&'){
                addChar();
                nextToken = BOOL_AND;
            }
            else{
                ungetc(nextChar, in_fp);
                nextToken = UNKNOWN;
            }
            break;
        case '|':
            addChar();
            getChar();
            if (nextChar == '|'){
                addChar();
                nextToken = BOOL_OR;
            }
            else{
                ungetc(nextChar, in_fp);
                nextToken = UNKNOWN;
            }
            break;
        case ';':
            addChar();
            nextToken = SEMICOLON;
            break;
        case '{':
            addChar();
            nextToken = OPEN_CURL;
            break;
        case '}':
            addChar();
            nextToken = CLOSE_CURL;
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its 
 * character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace 
 * character */
static void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */

    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */

