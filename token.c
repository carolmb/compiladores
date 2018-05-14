#ifndef _TOKEN_
#define _TOKEN_

#include "constants.h"

//STRUCTS
typedef struct {
    int key;
    const char* name;
    const char* value;
    int line;
    int column;
}Token;

#endif