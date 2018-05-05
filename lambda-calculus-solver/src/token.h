/*******************************************************************************
 * File:    token.h
 * Author:  Tristan W. Bonsor
 *
 * Description: Token ADT interface. Remember to call delete_token when finished
 *              with a token or memory leaks will occur.
 ******************************************************************************/
#ifndef _TOKEN_H_
#define _TOKEN_H_

typedef struct token token;

typedef enum {
    tt_LPAREN,
    tt_RPAREN,
    tt_LAMBDA,
    tt_INVALID,
    tt_ASSIGN,
    tt_DOT,
    tt_COLON,
    tt_SPACE,
    tt_ID,
    tt_GIVEN,
    tt_SOLVE,
    tt_ENDL,
    tt_NONE
} token_type;

token* new_token(token_type type, char* value);

void delete_token(token* t);

token_type get_token_type(token* t);

char* get_token_value(token* t);

#endif /* _TOKEN_H_ */
