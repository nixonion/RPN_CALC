/* Fill in your Name and GNumber in the following two comment fields
 * Name:
 * GNumber:
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "token.h"
#include "hash.h"

/* Local Function Declarations (that you need to finish implementing) */
static int read_file(char *filename, char *line);
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok);

/* Pre-Written Local Function Declarations */
static void print_header(char *filename, int step);
static void print_step_header(int step);
static void print_step_footer(Symtab *symtab, Stack_head *stack);
static void print_step_output(int val);

/* Defines the largest line that can be read from a file */
#define MAX_LINE_LEN 255

/* This has been written for you.
 * Main function to run your program (written for you).
 * 1) Opens the file using the passed in filename. (your function)
 * -- If the file is not found (ie. fopen returns NULL), then exit(-1);
 * 2) Reads one line from the file.
 * -- The programs will all only be one line in size.
 * -- That line may be up to MAX_LINE_LEN long.
 * 3) Closes the file.
 * 4) Calls token_read_line(line, strlen(line))
 * -- This parses the line and prepares the tokens to be ready to get.
 * 5) While there are tokens remaining to parse: token_has_next() != 0
 * -- a) Get the next token: token_get_next()
 * 6) Parse the token (your function)
 * 7) Print out all of the relevant information
 */
int rpn(Stack_head *stack, Symtab *symtab, char *filename) {
  int step = 0; /* Used to track the program steps */
  int ret = 0;
  char line[MAX_LINE_LEN];
  Token *tok = NULL;

  /* Complete the read_file function that is defined later in this file. */
  ret = read_file(filename, line);
  if(ret != 0) {
    printf("Error: Cannot Read File %s.  Exiting\n", filename);
    exit(-1);
  }

  /* Pass the line into the tokenizer to initialize that system */
  token_read_line(line, strlen(line));

  /* Prints out the nice program output header */
  print_header(filename, step);

  /* Iterate through all tokens */
  while(token_has_next()) {
    /* Begin the next step of execution and print out the step header */
    step++; /* Begin the next step of execution */
    print_step_header(step);

    /* Get the next token */
    tok = token_get_next();
    /* Complete the implementation of this function later in this file. */
    ret = parse_token(symtab, stack, tok);
    if(ret != 0) {
      printf("Critical Error in Parsing.  Exiting Program!\n");
      exit(-1);
    }

    /* Prints out the end of step information */
    print_step_footer(symtab, stack);
  }

  return 0;
}

/* (IMPLEMENT THIS FUNCTION)
 * Local function to open a file or exit.
 * Follow the Directions in the Project Documentation for this Function
 * Open filename, read its contents (up to MAX_LINE_LEN) into line, then
 *   close the file and return 0.
 * On any file error, return -1.
 */
static int read_file(char *filename, char *line) {
  /* Implement This Function */
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) 
    {
      return -1;
    }
    if(fgets(line, MAX_LINE_LEN, fp) == NULL)
    {
      return -1;
    }
    fclose(fp);
    return 0;
}

/* (IMPLEMENT THIS FUNCTION)
 * Parses the Token to implement the rpn calculator features
 * Follow the Directions in the Project Documentation for this Function
 * You may implement this how you like, but many small functions would be good!
 * If the token you are passed in is NULL, return -1.
 * If there are any memory errors, return -1.
 */
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok) {
  /* Implement This Function */
    if(tok == NULL)
    {
        return -1;
    }
	Token *tok1= NULL;
	Token *tok2= NULL;
    switch(tok->type)
    {
        case TYPE_ASSIGNMENT:
            tok1 = stack_pop(stack);
            tok2 = stack_pop(stack);
            if(tok1->type == TYPE_VARIABLE)
            {
                if(hash_put(symtab, tok1->variable, tok2->value) != 0)
                {
                    return -1;
                }
            }
            else
            {
                if(hash_put(symtab, tok2->variable, tok1->value) != 0)
                {
                    return -1;
                }
            }
            token_free(tok1);
            token_free(tok2);

            break;


        case TYPE_OPERATOR:
            tok1 = stack_pop(stack);
            tok2 = stack_pop(stack);
            int temp_1=0;
            int temp_2=0;
            int temp=0;

            if(tok1->type == TYPE_VARIABLE)
            {
                Symbol *sym1=hash_get(symtab,tok1->variable);
                temp_1=sym1->val;
                symbol_free(sym1);
            }
            else
            {
                temp_1=tok1->value;
            }
            if(tok1->type == TYPE_VARIABLE)
            {
                Symbol *sym2=hash_get(symtab,tok2->variable);
                temp_2=sym2->val;
                symbol_free(sym2);
            }
            else
            {
                temp_2=tok2->value;
            }
            switch(tok->oper)
            {
                case OPERATOR_PLUS:
                    temp=temp_2+temp_1;
                    break;  
                case OPERATOR_MINUS:
                    temp=temp_2-temp_1;
                    break;
                case OPERATOR_MULT:
                    temp=temp_2*temp_1;
                    break;
                case OPERATOR_DIV:
                    temp=temp_2/temp_1;
                    break;
                default:
                    return -1;
                    break;
            }

            Token *tok_temp=token_create_value(temp);
            token_free(tok1);
            token_free(tok2);
            stack_push(stack,tok_temp);
            //token_free(tok_temp);
            break;


        case TYPE_VARIABLE:
            if(stack_push(stack,tok) != 0)
            {
                return -1;
            }
            break;


        case TYPE_VALUE:
            if(stack_push(stack,tok) != 0)
            {
                return -1;
            }
            break;


        case TYPE_PRINT:
            tok1 = stack_pop(stack);
            print_step_output(tok1->value);
            break;


        default:
            return -1;
            break;
    }

    return 0;
}

/* This has been written for you.
 * Prints out the main output header
 */
static void print_header(char *filename, int step) {
  printf("######### Beginning Program (%s) ###########\n", filename);
  printf("\n.-------------------\n");
  printf("| Program Step = %2d\n", step);
  token_print_remaining();
  printf("o-------------------\n");
}

/* This has been written for you.
 * Prints out the information at the top of each step
 */
static void print_step_header(int step) {
  printf("\n.-------------------\n");
  printf("| Program Step = %2d\n", step++);
}

/* This has been written for you.
 * Prints out the output value (print token) nicely
 */
static void print_step_output(int val) {
  printf("|-----Program Output\n");
  printf("| %d\n", val);
}

/* This has been written for you.
 * Prints out the information at the bottom of each step
 */
static void print_step_footer(Symtab *symtab, Stack_head *stack) {
  hash_print_symtab(symtab);
  stack_print(stack);
  token_print_remaining();
  printf("o-------------------\n");
}
