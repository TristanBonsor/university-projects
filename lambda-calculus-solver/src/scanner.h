/*******************************************************************************
 * File:    scanner.h
 * Author:  Tristan W. Bonsor
 *
 * Description: Scanner interface.
 ******************************************************************************/
#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <stdio.h>
#include "token.h"

token** scan(FILE* input);

#endif /* _SCANNER_H_ */
