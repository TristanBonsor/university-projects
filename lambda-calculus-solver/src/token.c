/*******************************************************************************
 * File:    token.c
 * Author:  Tristan W. Bonsor
 *
 * Description: Token ADT implementation.
 ******************************************************************************/
#include <stdlib.h>
#include "token.h"

struct token {
    token_type type;
    char* value;
};

token* new_token(token_type type, char* value) {
    token* t = (token*)malloc(sizeof(token));

    if(!t) return 0;

    t->type = type;
    t->value = value;

    return t;
}

void delete_token(token* t) {
    if(!t) return;

    free(t);
}

token_type get_token_type(token* t) {
    if(!t) return tt_NONE;

    return t->type;
}

char* get_token_value(token* t) {
    if(!t) return 0;

    return t->value;
}
