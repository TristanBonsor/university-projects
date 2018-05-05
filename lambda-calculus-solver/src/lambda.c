/*******************************************************************************
 * File:    lambda.c
 * Author:  Tristan W. Bonsor
 *
 * Description: Implementation of lambda expression ADT.
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lambda.h"
#include "symbol.h"

#define ANSI_UNDERLINED_PRE "\033[4m"
#define ANSI_UNDERLINED_POST "\033[0m"

int table_size = 0;
lambda* expr_table[256];
char* id_table[256];

int UNDERLINE = 0;
int U_SUB = 0;
char* REDEX_ID = 0;

char* BOUND_VARS[256];
int BOUND_VARS_SIZE = 0;
lambda* NEW_SCOPE = 0;

int RENAME = 0;

static lambda* get_next_redex(lambda* expr);

static void print_beta_reduction(lambda* expr, lambda* redex);

static lambda* apply_redex(lambda* redex);

static lambda* search_replace(lambda* expr, lambda* replace, char* id);

static int is_collision(lambda* expr);

static void alpha_conversion(lambda* expr, lambda* scope, char* id);

static lambda* is_replace_encoding(lambda* expr);

static lambda* replace_encoding(lambda* expr, lambda* rename);

static void print_rename(lambda* expr, lambda* rename);

struct lambda {
    lambda_type type;
    char* id;
    lambda *left, *right, *parent;
};

lambda* new_lambda(lambda_type type, char* id, lambda* left, lambda* right,
                   lambda* parent) {
    lambda* expr = (lambda*)malloc(sizeof(lambda));

    if(!expr) return 0;

    expr->type = type;
    expr->id = id;
    expr->left = left;
    expr->right = right;
    expr->parent = parent;

    return expr;
}

void delete_lambda(lambda* expr) {
    if(!expr) return;

    free(expr);
}

void delete_all_lambda(lambda* expr) {
    if(!expr) return;

    if(expr->id) free(expr->id);

    delete_all_lambda(expr->left);
    delete_all_lambda(expr->right);

    delete_lambda(expr);
}

lambda* copy_lambda(lambda* expr) {
    char* id = 0;
    int ix = 0;

    lambda* nexpr, *lexpr, *rexpr;

    if(!expr) return 0;

    if(expr->id) {
        id = (char*)calloc(strlen(expr->id) + 1, sizeof(char));

        while(expr->id[ix] != '\0') {
            id[ix] = expr->id[ix];
            ++ix;
        }

        id[ix] = '\0';
    }

    lexpr = copy_lambda(expr->left);
    rexpr = copy_lambda(expr->right);

    nexpr = new_lambda(expr->type, id, lexpr, rexpr, 0);

    set_lambda_parent(lexpr, expr);
    set_lambda_parent(rexpr, expr);

    return nexpr;
}

void print_lambda(lambda* expr) {
    if(!expr) return;

    switch(expr->type) {
    case lt_APPC:
        if(expr->parent && expr->left && expr->right) printf("(");
        print_lambda(expr->left);
        if(expr->right) printf(" ");
        print_lambda(expr->right);
        if(expr->parent && expr->left && expr->right) printf(")");

        break;

    case lt_ABST:
        if(expr->parent && expr->parent->type != lt_ABST) printf("(");
        printf("l%s.", expr->id);
        print_lambda(expr->left);
        if(expr->parent && expr->parent->type != lt_ABST) printf(")");

        break;

    case lt_ID:
        printf("%s", expr->id);

        break;

    default: break;
    }
}

int compare_lambda(lambda* expr1, lambda* expr2) {
    int rtn;

    if(!expr1 && !expr2) return 1;
    if(!expr1 || !expr2) return 0;

    if(expr1->type == expr2->type) {
        if(expr1->id && expr2->id) {
            if(!strcmp(expr1->id, expr2->id)) {
                rtn = compare_lambda(expr1->left, expr2->left);
                if(rtn) return compare_lambda(expr1->right, expr2->right);
                return 0;
            }
        }
        else {
            rtn = compare_lambda(expr1->left, expr2->left);
            if(rtn) return compare_lambda(expr1->right, expr2->right);
            return 0;
        }
    }
    return 0;
}

char* reverse_encode(lambda* expr) {
    int ix;
    lambda* texpr;

    if(!expr) return 0;

    for(ix = 0; ix < table_size; ++ix) {
        texpr = expr_table[ix]->left;
        if(compare_lambda(expr, texpr)) break;
        else texpr = 0;
    }

    if(texpr) return id_table[ix];

    return 0;
}

lambda* reduce(lambda* expr) {
    lambda* redex = 0, *rename;
    char id[256];

    if(!expr) return 0;

    while(is_collision(expr)) {
        strcpy(id, NEW_SCOPE->id);
        alpha_conversion(expr, NEW_SCOPE, id);
        printf(" ->->\t//alpha-conversion: rename ");
        printf(ANSI_UNDERLINED_PRE "%s" ANSI_UNDERLINED_POST, id);
        printf(" to %s", NEW_SCOPE->id);
        printf("\n");
        BOUND_VARS_SIZE = 0;
    }

    redex = get_next_redex(expr);

    if(!redex) {
        rename = is_replace_encoding(expr);
        if(rename && rename->parent) {
            print_rename(expr, rename);
            printf(" ->->\t//replace ");
            printf(ANSI_UNDERLINED_PRE "%s" ANSI_UNDERLINED_POST, rename->id);
            printf(" with encoding\n");

            expr = replace_encoding(expr, rename);
            redex = get_next_redex(expr);
        }
    }
    while(redex) {
        BOUND_VARS_SIZE = 0;

        print_beta_reduction(expr, redex);
        printf(" ->->\t//beta-reduction: replacing ");
        printf(ANSI_UNDERLINED_PRE "%s" ANSI_UNDERLINED_POST, redex->left->id);
        printf(" with ");
        print_lambda(redex->right);
        printf("\n");

        expr = apply_redex(redex);

        while(is_collision(expr)) {
            strcpy(id, NEW_SCOPE->id);
            alpha_conversion(expr, NEW_SCOPE, NEW_SCOPE->id);
            printf(" ->->\t//alpha-conversion: rename ");
            printf(ANSI_UNDERLINED_PRE "%s" ANSI_UNDERLINED_POST, id);
            printf(" to %s", NEW_SCOPE->id);
            printf("\n");
            BOUND_VARS_SIZE = 0;
        }

        redex = get_next_redex(expr);

        if(!redex){
            rename = is_replace_encoding(expr);

            if(rename && rename->parent) {
                print_rename(expr, rename);
                printf(" ->->\t//replace ");
                printf(ANSI_UNDERLINED_PRE "%s" ANSI_UNDERLINED_POST, rename->id);
                printf(" with encoding\n");
                expr = replace_encoding(expr, rename);
                redex = get_next_redex(expr);
            }  
        }
    }

    return expr;
}

lambda_type get_lambda_type(lambda* expr) {
    if(!expr) return lt_NONE;

    return expr->type;
}

char* get_lambda_id(lambda* expr) {
    if(!expr) return 0;

    return expr->id;
}

lambda* get_lambda_left(lambda* expr) {
    if(!expr) return 0;

    return expr->left;
}

lambda* get_lambda_right(lambda* expr) {
    if(!expr) return 0;

    return expr->right;
}

lambda* get_lambda_parent(lambda* expr) {
    if(!expr) return 0;

    return expr->parent;
}

void set_lambda_type(lambda* expr, lambda_type type) {
    if(!expr) return;

    expr->type = type;
}

void set_lambda_id(lambda* expr, char* id) {
    if(!expr) return;

    expr->id = id;
}

void set_lambda_left(lambda* expr, lambda* left) {
    if(!expr) return;

    expr->left = left;
}

void set_lambda_right(lambda* expr, lambda* right) {
    if(!expr) return;

    expr->right = right;
}

void set_lambda_parent(lambda* expr, lambda* parent) {
    if(!expr) return;

    expr->parent = parent;
}

static lambda* get_next_redex(lambda* expr) {
    lambda* nexpr = 0;

    if(!expr) return 0;

    switch(expr->type) {
    case lt_APPC:
        if(expr->left && expr->right) {
            if(expr->left->type == lt_ABST) nexpr = expr;
            else {
                nexpr = get_next_redex(expr->left);
                if(!nexpr) nexpr = get_next_redex(expr->right);
            }
        }
        else {
            nexpr = get_next_redex(expr->left);
            if(!nexpr) nexpr = get_next_redex(expr->right);
        }

        break;

    case lt_ABST:
        nexpr = get_next_redex(expr->left);

        break;

    default: break;
    }

    return nexpr;
}

static void print_beta_reduction(lambda* expr, lambda* redex) {
    if(!expr || !redex) return;

    switch(expr->type) {
    case lt_APPC:
        if(expr->parent) {
            if(!REDEX_ID && UNDERLINE) printf(ANSI_UNDERLINED_PRE "(" ANSI_UNDERLINED_POST);
            else printf("(");
        }

        if(redex->right == expr) {
            UNDERLINE = 1;
            U_SUB = 1;
            print_beta_reduction(expr->left, redex);
            if(expr->right) printf(ANSI_UNDERLINED_PRE " " ANSI_UNDERLINED_POST);
            print_beta_reduction(expr->right, redex);
            U_SUB = 0;
            UNDERLINE = 0;
        }
        else {
            print_beta_reduction(expr->left, redex);
            if(expr->right){
                if(UNDERLINE && U_SUB)
                    printf(ANSI_UNDERLINED_PRE " " ANSI_UNDERLINED_POST);
                else printf(" ");
            }
            print_beta_reduction(expr->right, redex);
        }

        if(expr->parent) {
            if(!REDEX_ID && UNDERLINE) printf(ANSI_UNDERLINED_PRE ")" ANSI_UNDERLINED_POST);
            else printf(")");
        }

        break;

    case lt_ABST:
        if(!REDEX_ID && UNDERLINE) printf(ANSI_UNDERLINED_PRE "(" ANSI_UNDERLINED_POST);
        else printf("(");

        if(redex->left == expr) {
            printf("l");
            printf(ANSI_UNDERLINED_PRE "%s" ANSI_UNDERLINED_POST, expr->id);
            printf(".");

            UNDERLINE = 1;
            REDEX_ID = expr->id;

            print_beta_reduction(expr->left, redex);

            REDEX_ID = 0;
            UNDERLINE = 0;
        }
        else if(redex->right == expr) {
            UNDERLINE = 1;
            printf(ANSI_UNDERLINED_PRE "l%s." ANSI_UNDERLINED_POST, expr->id);
            print_beta_reduction(expr->left, redex);
            UNDERLINE = 0;
        }
        else if(!REDEX_ID && UNDERLINE){
            printf(ANSI_UNDERLINED_PRE "l%s." ANSI_UNDERLINED_POST, expr->id);
            print_beta_reduction(expr->left, redex);
        }
        else {
            printf("l%s.", expr->id);
            print_beta_reduction(expr->left, redex);
        }

        if(!REDEX_ID && UNDERLINE) printf(ANSI_UNDERLINED_PRE ")" ANSI_UNDERLINED_POST);
        else printf(")");

        break;

    case lt_ID:
        if(redex->right == expr) printf(ANSI_UNDERLINED_PRE "%s" ANSI_UNDERLINED_POST, expr->id);
        else if(!REDEX_ID && UNDERLINE) printf(ANSI_UNDERLINED_PRE "%s" ANSI_UNDERLINED_POST, expr->id);
        else if(REDEX_ID && UNDERLINE) {
            if(!strcmp(REDEX_ID, expr->id)) printf(ANSI_UNDERLINED_PRE "%s" ANSI_UNDERLINED_POST, expr->id);
            else printf("%s", expr->id);
        }
        else printf("%s", expr->id);

        break;

    default: break;
    }
}

static lambda* apply_redex(lambda* redex) {
    lambda* expr, *tmp;

    if(!redex) return 0;

    if(redex->type != lt_APPC || !redex->left || !redex->right
       || redex->left->type != lt_ABST)
        return 0;

    expr = search_replace(redex->left->left, redex->right, redex->left->id);

    if(redex->parent) {
        if(redex->parent->left == redex) redex->parent->left = expr;
        else if(redex->parent->right == redex) redex->parent->right = expr;

        expr->parent = redex->parent;
    }
    else {
        if(expr->left) {
            if(expr->left->type == lt_APPC) {
                if(!expr->left->right){
                    tmp = expr->left;
                    expr->left = expr->left->left;
                    delete_lambda(tmp);
                }
            }
            expr->left->parent = expr;
        }
        if(expr->right) {
            if(expr->right->type == lt_APPC) {
                if(!expr->right->right){
                    tmp = expr->right;
                    expr->right = expr->right->left;
                    delete_lambda(tmp);
                }
            }
            expr->right->parent = expr;
        }
        
        expr->parent = 0;
    }

    free(redex->left->id);
    delete_lambda(redex->left);
    redex->left = 0;

    while(expr->parent) expr = expr->parent;

    return expr;
}

static lambda* search_replace(lambda* expr, lambda* replace, char* id) {
    lambda* nexpr = 0;

    if(!expr || !replace || !id) return 0;

    switch(expr->type) {
    case lt_APPC:
        nexpr = search_replace(expr->left, replace, id);
        expr->left = nexpr;
        if(nexpr) nexpr->parent = expr;
        nexpr = search_replace(expr->right, replace, id);
        expr->right = nexpr;
        if(nexpr) nexpr->parent = expr;

        nexpr = expr;

        break;

    case lt_ABST:
        nexpr = search_replace(expr->left, replace, id);
        expr->left = nexpr;
        nexpr->parent = expr;
        nexpr = expr;

        break;

    case lt_ID:
        if(!strcmp(expr->id, id)) {
            nexpr = copy_lambda(replace);
            nexpr->parent = expr->parent;

            if(expr->parent->left == expr) expr->parent->left = nexpr;
            else if(expr->parent->right == expr) expr->parent->right = nexpr;

            free(expr->id);
            delete_lambda(expr);
        }
        else nexpr = expr;

        break;

    default: break;
    }

    return nexpr;
}

static int is_collision(lambda* expr) {
    int ix;

    if(!expr) return 0;

    switch(expr->type) {
    case lt_APPC:
        if(!is_collision(expr->left))
            return is_collision(expr->right);

        return 1;

        break;

    case lt_ABST:
        for(ix = 0; ix < BOUND_VARS_SIZE; ++ix)
            if(!strcmp(expr->id, BOUND_VARS[ix])) {
                NEW_SCOPE = expr;
                return 1;
            }

        BOUND_VARS[BOUND_VARS_SIZE++] = expr->id;

        return is_collision(expr->left);

        break;

    case lt_ID:
        if(!is_collision(expr->left))
            return is_collision(expr->right);

        break;

    default: break;
    }

    return 0;
}

static void alpha_conversion(lambda* expr, lambda* scope, char* id) {
    char* new_id;
    int ix;

    if(!expr || !scope || !id) return;

    switch(expr->type) {
    case lt_APPC:
        if(expr->parent) printf("(");

        alpha_conversion(expr->left, scope, id);

        if(expr->right) printf(" ");

        alpha_conversion(expr->right, scope, id);

        if(expr->parent) printf(")");

        break;

    case lt_ABST:
        if(expr->parent) printf("(");

        if(expr == scope) {
            printf("l");
            printf(ANSI_UNDERLINED_PRE "%s" ANSI_UNDERLINED_POST, expr->id);
            printf(".");

            RENAME = 1;
            alpha_conversion(expr->left, scope, id);
            RENAME = 0;

            new_id = (char*)calloc(strlen(expr->id) + 2, sizeof(char));

            for(ix = 0; ix < strlen(expr->id); ++ix) new_id[ix] = expr->id[ix];

            new_id[ix++] = '$';
            new_id[ix] = '\0';

            free(expr->id);

            expr->id = new_id;

            NEW_SCOPE = expr;
        }
        else {
            printf("l%s.", expr->id);
            alpha_conversion(expr->left, scope, id);
        }

        if(expr->parent) printf(")");

        break;

    case lt_ID:
        if(RENAME && !strcmp(expr->id, id)) {
            printf(ANSI_UNDERLINED_PRE "%s" ANSI_UNDERLINED_POST, expr->id);

            new_id = (char*)calloc(strlen(expr->id) + 2, sizeof(char));

            for(ix = 0; ix < strlen(expr->id); ++ix) new_id[ix] = expr->id[ix];

            new_id[ix++] = '$';
            new_id[ix] = '\0';

            free(expr->id);

            expr->id = new_id;
            
        }
        else printf("%s", expr->id);

        break;
    }
}

static lambda* is_replace_encoding(lambda* expr) {
    int ix;

    lambda* nexpr;

    if(!expr) return 0;

    if(expr->type == lt_ID) {
        for(ix = 0; ix < table_size; ++ix) {
            if(!strcmp(id_table[ix], expr->id)) {
                return expr;
            }
        }
    }


    nexpr = is_replace_encoding(expr->left);

    if(!nexpr)
        return is_replace_encoding(expr->right);

    return nexpr;
}

static lambda* replace_encoding(lambda* expr, lambda* rename) {
    int ix;
    lambda* nexpr = 0;

    if(!expr || !rename) return 0;

    switch(expr->type) {
    case lt_APPC:
        nexpr = replace_encoding(expr->left, rename);
        expr->left = nexpr;
        if(nexpr) nexpr->parent = expr;
        nexpr = replace_encoding(expr->right, rename);
        expr->right = nexpr;
        if(nexpr) nexpr->parent = expr;

        nexpr = expr;

        break;

    case lt_ABST:
        nexpr = replace_encoding(expr->left, rename);
        expr->left = nexpr;
        nexpr->parent = expr;
        nexpr = expr;

        break;

    case lt_ID:
        if(expr == rename) {
            for(ix = 0; ix < table_size; ++ix) {
                if(!strcmp(id_table[ix], expr->id)) {
                    nexpr = copy_lambda(expr_table[ix]);
                    if(nexpr->type == lt_APPC && !nexpr->right)
                        nexpr = nexpr->left;
                    else if(nexpr->type == lt_APPC && !nexpr->left)
                        nexpr = nexpr->right;

                    nexpr->parent = expr->parent;
                    if(expr->parent && expr->parent->left == expr)
                        expr->parent->left = nexpr;
                    else if(expr->parent && expr->parent->right == expr)
                        expr->parent->right = nexpr;

                    free(expr->id);
                    delete_lambda(expr);
                }
            }
        }
        else nexpr = expr;

        break;

    default: break;
    }

    return nexpr;
}

static void print_rename(lambda* expr, lambda* rename) {
    if(!expr || !rename) return;

    switch(expr->type) {
    case lt_APPC:
        if(expr->parent && expr->left && expr->right) printf("(");
        print_rename(expr->left, rename);
        if(expr->right) printf(" ");
        print_rename(expr->right, rename);
        if(expr->parent && expr->left && expr->right) printf(")");

        break;

    case lt_ABST:
        if(expr->parent && expr->parent->type != lt_ABST) printf("(");
        printf("l%s.", expr->id);
        print_rename(expr->left, rename);
        if(expr->parent && expr->parent->type != lt_ABST) printf(")");

        break;

    case lt_ID:
        if(expr == rename)
            printf(ANSI_UNDERLINED_PRE "%s" ANSI_UNDERLINED_POST, expr->id);
        else
            printf("%s", expr->id);

        break;

    default: break;
    }
}
