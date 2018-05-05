/*******************************************************************************
 * File:    parser.h
 * Author:  Tristan W. Bonsor
 *
 * Description: Interface for lambda expression parser.
 ******************************************************************************/
#ifndef _PARSER_H_
#define _PARSER_H_

#include "token.h"
#include "lambda.h"

lambda* parse(token** tokens);

#endif /* _PARSER_H_ */
