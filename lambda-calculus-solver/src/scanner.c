/*******************************************************************************
 * File:    scanner.c
 * Author:  Tristan W. Bonsor
 *
 * Description: Scanner implementation.
 ******************************************************************************/
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

#define DEF_CAP 8

static token** resize_token_buffer(token** tb, int n, int* ncap);

static void bomb(token** tb);

static char* get_identifier(FILE* input);

static char* resize_string(char* s, int n, int* ncap);

static int match_given(char* s);

static int match_solve(char* s);

token** scan(FILE* input) {
    token** t;
    char* id;
    int c, ix = 0, cap = DEF_CAP;

    if(!input) return 0;

    t = (token**)calloc(cap, sizeof(token*));

    if(!t) return 0;

    c = fgetc(input);

    while(c != EOF ) {
        switch(c) {
        case '(': t[ix++] = new_token(tt_LPAREN, 0); break;
        case ')': t[ix++] = new_token(tt_RPAREN, 0); break;
        case 'l': t[ix++] = new_token(tt_LAMBDA, 0); break;
        case '=': t[ix++] = new_token(tt_ASSIGN, 0); break;
        case '.': t[ix++] = new_token(tt_DOT,    0); break;
        case ':': t[ix++] = new_token(tt_COLON,  0); break;
        case ' ':
        case '\t':
        case '\v': t[ix++] = new_token(tt_SPACE, 0); break;
        case '\n': t[ix++] = new_token(tt_ENDL, 0); break;
        default:
            if(isalpha(c) || isdigit(c)) {
                ungetc(c, input);

                id = get_identifier(input);

                if(match_given(id)) t[ix] = new_token(tt_GIVEN, id);
                else if(match_solve(id)) t[ix] = new_token(tt_SOLVE, id);
                else t[ix] = new_token(tt_ID, id);
            }
            else {
                t[ix] = new_token(tt_INVALID, 0);
                printf("Invalid char: ");
                bomb(t);
            }
            ++ix;

            break;
        }

        if(ix == cap) t = resize_token_buffer(t, ix, &cap);

        c = fgetc(input);
    }

    return t;
}

static token** resize_token_buffer(token** tb, int n, int* ncap) {
    token** nb;
    int cap, i;

    if(!tb || !ncap) return 0;

    cap = *ncap;

    if(!n || cap + n < 0) return tb;

    *ncap = cap + n;

    nb = (token**)calloc(*ncap, sizeof(token*));

    for(i = 0; i < cap; ++i) nb[i] = tb[i];

    free(tb);

    return nb;
}

static void bomb(token** tb) {
    int i = 0;

    char* s;

    if(!tb) exit(1);

    while(tb[i]) {
        s = get_token_value(tb[i]);

        if(s) {
            printf("%s", s);
            free(s);
        }

        free(tb[i++]);
    }

    printf("\n");

    exit(1);
}

static char* get_identifier(FILE* input) {
    int c, ix = 0, cap = DEF_CAP;

    char* id;

    if(!input) return 0;

    c = fgetc(input);

    if(!isalpha(c) && !isdigit(c)) return 0;

    id = (char*)calloc(cap, sizeof(char));

    if(!id) return 0;

    do {
        id[ix++] = c;

        if(ix == cap) id = resize_string(id, ix, &cap);

        c = fgetc(input);

    }while(c != EOF && (isalpha(c) || isdigit(c)));

    id[ix] = '\0';

    ungetc(c, input);

    return id;
}

static char* resize_string(char* s, int n, int* ncap) {
    char* ns;
    int cap, i;

    if(!s || !ncap) return 0;

    cap = *ncap;

    if(!n || cap + n < 0) return s;

    *ncap = cap + n;

    ns = (char*)calloc(*ncap, sizeof(char));

    for(i = 0; i < cap; ++i) ns[i] = s[i];

    free(s);

    return ns;
}

static int match_given(char* s) {
    if(!s) return 0;

    return !strcmp("Given", s);
}

static int match_solve(char* s) {
    if(!s) return 0;

    return !strcmp("Solve", s);
}
