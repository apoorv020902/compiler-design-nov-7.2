/**
 * This is a lexical analyzer code in
 *
 * Sebesta, R. W. (2012). Concepts of Programming Languages. 
 * Pearson, 10th edition.
 *
/* front.c - a lexical analyzer system for simple arithmetic expressions */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
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
    printf("Cooke Analyzer :: R11723071\n");

    /*Error Handling; Making sure only one argument is passes from the command line */
        if (argumentCount != 2) {
        printf("Usage Error: Expected syntax - cooke_analyzer <path_to_source_file> \n");
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
            nextToken = UNKNOWN;
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

/*implemented switch case to return token name linked with the passed integer value*/
static char* tokenCodeToName(int current_code) {
    switch (current_code) {
        case 0: return "LETTER";
        case 1: return "DIGIT";
        case 99: return "UNKNOWN";
        case 10: return "INT_LIT";
        case 11: return "IDENT";
        case 20: return "ASSIGN_OP";
        case 21: return "ADD_OP";
        case 22: return "SUB_OP";
        case 23: return "MULT_OP";
        case 24: return "DIV_OP";
        case 25: return "OPEN_PAREN";
        case 26: return "CLOSE_PAREN";
        case 27: return "LESSER_OP";
        case 28: return "GREATER_OP";
        case 29: return "EQUAL_OP";
        case 30: return "NEQUAL_OP";
        case 31: return "LEQUAL_OP";
        case 32: return "GEQUAL_OP";
        case 33: return "MOD_OP";
        case 34: return "BOOL_AND";
        case 35: return "BOOL_OR";
        case 36: return "BOOL_NOT";
        case 37: return "SEMICOLON";
        case 38: return "KEY_IN";
        case 39: return "KEY_OUT";
        case 40: return "KEY_IF";
        case 41: return "KEY_ELSE";
        case 42: return "OPEN_CURL";
        case 43: return "CLOSE_CURL";
        default: return "UNKNOWN";
    }
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();

        if (charClass == LETTER) {
        /* Parse identifiers */
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) {
            addChar();
            getChar();
        }

            /* Handling cases for if, else, input, output and ident */ 
            if (strcmp(lexeme, "if") == 0) {
                nextToken = KEY_IF;
            } else if (strcmp(lexeme, "else") == 0) {
                nextToken = KEY_ELSE;
            } else if (strcmp(lexeme, "input") == 0) {
                nextToken = KEY_IN;
            } else if (strcmp(lexeme, "output") == 0) {
                nextToken = KEY_OUT;
            } else {
                nextToken = IDENT;
            }

        } else if (charClass == DIGIT) {
            /* Parse integer literals */
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
        } else if (charClass == UNKNOWN) {
            /* Parentheses and operators */
            lookup(nextChar);
            getChar();
        } else if (charClass == EOF) {
            /* EOF */
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
        }

        if (strcmp(lexeme, "EOF") != 0){
            printf("%s %s\n", lexeme, tokenCodeToName(nextToken));
        }


        return nextToken;
} /* End of function lex */

