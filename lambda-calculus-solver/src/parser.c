/*******************************************************************************
 * File:    parser.c
 * Author:  Tristan W. Bonsor
 *
 * Description: Implementation of lambda expression parser. Refer to readme for
 *              correct grammar.
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "symbol.h"

static int get_token_count(token** tokens);

static void strip_tokens(token** tokens, token_type type, int* start, int* end);

static int match_paren(token** tokens, int start);

static int strip_outer_paren(token** tokens, int* start, int* end);

static lambda* build_expression(token** tokens, int* start, int end);

static int match_abstraction(token** tokens, int start);

static lambda* complex_parse(token** tokens, int start, int end);

static int get_endl(token** tokens, int start);

lambda* parse(token** tokens) {
    int ix = 0;
    int end;

    lambda* expr, *root_expr;

    if(!tokens) return 0;

    end = get_token_count(tokens);

    strip_tokens(tokens, tt_SPACE, &ix, &end);
    strip_tokens(tokens, tt_ENDL, &ix, &end);

    if(get_token_type(tokens[ix]) == tt_GIVEN) return complex_parse(tokens, ix, end);

    strip_outer_paren(tokens, &ix, &end);

    expr = new_lambda(lt_APPC, 0, 0, 0, 0);

    if(!expr) return 0;

    do {
        set_lambda_left(expr, build_expression(tokens, &ix, end));
        set_lambda_right(expr, build_expression(tokens, &ix, end));

        set_lambda_parent(get_lambda_left(expr), expr);
        set_lambda_parent(get_lambda_right(expr), expr);

        if(get_lambda_left(root_expr)) set_lambda_right(root_expr, expr);
        else root_expr = expr;

        if(ix < end) {
            root_expr = new_lambda(lt_APPC, 0, expr, 0, 0);
            set_lambda_parent(expr, root_expr);
            expr = new_lambda(lt_APPC, 0, 0, 0, 0);
        }

    }while(ix < end);

    for(ix = 0; ix < end; ++ix) delete_token(tokens[ix]);

    return root_expr;
}

static int get_token_count(token** tokens) {
    int ix = -1;

    if(!tokens) return 0;

    while(tokens[++ix]);

    return ix;
}

static void strip_tokens(token** tokens, token_type type, int* start, int* end) {
    int match = 0;

    if(!tokens || !start || !end || *start < 0 || *start >= *end) return;

    if(tokens[*start] && get_token_type(tokens[*start]) == type) {
        match = 1;
        *start += 1;
    }

    if(tokens[*end-1] && get_token_type(tokens[*end-1]) == type) {
        match = 1;
        *end -= 1;
    }

    if(match) strip_tokens(tokens, type, start, end);
}

static int match_paren(token** tokens, int start) {
    int ix = start;
    int count = 0;

    if(!tokens || start < 0) return -1;

    if(!tokens[ix] || get_token_type(tokens[ix]) != tt_LPAREN) return -1;

    do {
        if(get_token_type(tokens[ix]) == tt_LPAREN) ++count;
        else if(get_token_type(tokens[ix]) == tt_RPAREN) --count;
    }while(count && tokens[++ix]);

    if(count) return -1;

    return ix;
}

static int strip_outer_paren(token** tokens, int* start, int* end) {
    int rtn;

    if(!tokens || !start || !end || *start < 0 || *start >= *end) return 0;

    if(!tokens[*start] || get_token_type(tokens[*start]) != tt_LPAREN) return 0;

    rtn = match_paren(tokens, *start);

    if(rtn == *end - 1) {
        *start += 1;
        *end -= 1;
        return strip_outer_paren(tokens, start, end) + 1;
    }

    return 0;
}

static lambda* build_expression(token** tokens, int* start, int end) {
    int* ix = start;
    int expr_end, dsp;
    lambda* expr = 0, *lexpr = 0, *rexpr = 0, *root_expr = 0;

    char* id;

    if(!tokens || !start || *start < 0 || *start >= end) return 0;

    if(!tokens[*ix]) return 0;

    strip_tokens(tokens, tt_SPACE, ix, &end);
    strip_tokens(tokens, tt_ENDL, ix, &end);

    switch(get_token_type(tokens[*ix])) {
    case tt_LPAREN:
        expr_end = match_paren(tokens, *ix) + 1;

        dsp = strip_outer_paren(tokens, ix, &expr_end);

        switch(get_token_type(tokens[*ix])) {
        case tt_LPAREN:
        case tt_ID:
            do {
                lexpr = build_expression(tokens, ix, expr_end);
                rexpr = build_expression(tokens, ix, expr_end);

                expr = new_lambda(lt_APPC, 0, lexpr, rexpr, 0);

                set_lambda_parent(lexpr, expr);
                set_lambda_parent(rexpr, expr);

                if(get_lambda_left(root_expr)) set_lambda_right(root_expr, expr);
                else root_expr = expr;

                if(*ix < expr_end) {
                    root_expr = new_lambda(lt_APPC, 0, expr, 0, 0);
                    set_lambda_parent(expr, root_expr);
                    expr = new_lambda(lt_APPC, 0, 0, 0, 0);
                }

            }while(*ix < expr_end);

            *ix += dsp;

            break;

        case tt_LAMBDA:
            root_expr = build_expression(tokens, ix, expr_end);

            *ix += dsp;

            break;
            
        }

        break;

    case tt_SPACE:
    case tt_ENDL:
        *ix += 1;

        root_expr = build_expression(tokens, ix, end);

        break;

    case tt_LAMBDA:
        if(match_abstraction(tokens, *ix)) {
            *ix += 3;

            id = get_token_value(tokens[*ix-2]);

            lexpr = build_expression(tokens, ix, end);

            root_expr = new_lambda(lt_ABST, id, lexpr, 0, 0);

            set_lambda_parent(lexpr, root_expr);
        }
        else {
            /* ERROR */
        }

        break;

    case tt_ID:
        root_expr = new_lambda(lt_ID, get_token_value(tokens[*ix]), 0, 0, 0);

        *ix += 1;

        break;

    default:
        /* ERROR */

        break;
    }

    return root_expr;
}

static int match_abstraction(token** tokens, int start) {
    int ix = start;

    if(!tokens || start < 0) return 0;

    if(tokens[ix] && get_token_type(tokens[ix++]) == tt_LAMBDA)
        if(tokens[ix] && get_token_type(tokens[ix++]) == tt_ID)
            if(tokens[ix] && get_token_type(tokens[ix]) == tt_DOT)
                return 1;

    return 0;
}

static lambda* complex_parse(token** tokens, int start, int end) {
    int ix = start;
    int sub_end, i;
    token_type type;
    token* t;
    token** sub_tokens;

    lambda* expr;

    if(!tokens || start < 0 || start >= end) return 0;

    type = get_token_type(tokens[ix]);

    while(type != tt_SOLVE) {
        switch(type) {
        case tt_COLON:
        case tt_SPACE:
        case tt_ENDL:
        case tt_GIVEN:
            ++ix;

            break;

        case tt_ID:
            t = tokens[ix];

            while(get_token_type(tokens[++ix]) == tt_SPACE);
            while(get_token_type(tokens[++ix]) == tt_SPACE);

            id_table[table_size] = (char*)calloc(strlen(get_token_value(t)) + 1,
                                                 sizeof(char));

            strcpy(id_table[table_size], get_token_value(t));

            sub_end = get_endl(tokens, ix);

            sub_tokens = (token**)calloc(sub_end - ix + 1, sizeof(token*));

            for(i = 0; i < sub_end - ix; ++i) sub_tokens[i] = tokens[ix+i];

            expr_table[table_size] = parse(sub_tokens);

            ++table_size;

            free(sub_tokens);

            ix = sub_end;

            break;

        default:
            ++ix;

            break;
        }

        type = get_token_type(tokens[ix]);
    }

    ix = get_endl(tokens, ix);

    sub_end = get_endl(tokens, ix);

    sub_tokens = (token**)calloc(sub_end - ix + 1, sizeof(token*));

    for(i = 0; i < sub_end - ix; ++i) sub_tokens[i] = tokens[ix+i];

    expr = parse(sub_tokens);

    free(sub_tokens);

    return expr;
}

static int get_endl(token** tokens, int start) {
    int ix = start;

    if(!tokens || start < 0) return -1;

    while(tokens[ix] && get_token_type(tokens[ix++]) != tt_ENDL);

    return ix;
}
