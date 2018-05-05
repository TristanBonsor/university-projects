/*******************************************************************************
 * File:    lambda.h
 * Author:  Tristan W. Bonsor
 *
 * Description: ADT interface for a lambda expression.
 ******************************************************************************/
#ifndef _LAMBDA_H_
#define _LAMBDA_H_

typedef struct lambda lambda;

typedef enum {
    lt_NONE,
    lt_ABST,
    lt_APPC,
    lt_ID
}lambda_type;

lambda* new_lambda(lambda_type type, char* id, lambda* left, lambda* right,
                   lambda* parent);

void delete_lambda(lambda* expr);

void delete_all_lambda(lambda* expr);

lambda* copy_lambda(lambda* expr);

void print_lambda(lambda* expr);

int compare_lambda(lambda* expr1, lambda* expr2);

char* reverse_encode(lambda* expr);

lambda* reduce(lambda* expr);

lambda_type get_lambda_type(lambda* expr);

char* get_lambda_id(lambda* expr);

lambda* get_lambda_left(lambda* expr);

lambda* get_lambda_right(lambda* expr);

lambda* get_lambda_parent(lambda* expr);

void set_lambda_type(lambda* expr, lambda_type type);

void set_lambda_id(lambda* expr, char* id);

void set_lambda_left(lambda* expr, lambda* left);

void set_lambda_right(lambda* expr, lambda* right);

void set_lambda_parent(lambda* expr, lambda* parent);

#endif /* _LAMBDA_H_ */
