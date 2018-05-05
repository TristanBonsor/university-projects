/*******************************************************************************
 * File:    main.c
 * Author:  Tristan W. Bonsor
 *
 * Description: Main file for the lambda calculus solver.
 ******************************************************************************/
#include "parser.h"
#include "scanner.h"

int main(){
    char file_name[256];
    FILE* file_stream;

    char* encode;

    lambda* expr;

    printf("Enter file name> ");

    scanf("%s", file_name);

    file_stream = fopen(file_name, "r");

    if(!file_stream) {
        printf("Error opening file %s\n", file_name);
        return 1;
    }

    expr = parse(scan(file_stream));

    print_lambda(expr);
    printf("\n");

    expr = reduce(expr);

    encode = reverse_encode(expr);

    print_lambda(expr);

    if(encode)
      printf(" ->->\t//Reverse encoding for %s\n%s", encode, encode);

    printf("\n");

    delete_all_lambda(expr);

    return 0;
}
