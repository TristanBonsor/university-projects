/*******************************************************************************
 * File:    symbol.h
 * Author:  Tristan W. Bonsor
 *
 * Description:
 ******************************************************************************/
#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "lambda.h"

extern lambda* expr_table[256];
extern char* id_table[256];
extern int table_size;

#endif /* _SYMBOL_H_ */
