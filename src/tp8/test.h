#include<stdio.h>
#include<stddef.h>

#ifndef TEST_H
#define TEST_H

typedef enum {MALLOC,CALLOC,REALLOC} Type_test;

/*
 * prototype de la fonction de test
 */
void procedure_test(Type_test t, int nb);

#endif /* TEST_H */
