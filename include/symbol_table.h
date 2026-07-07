#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stddef.h>


typedef struct
{
    char name[64];
    double value;

} Symbol;


typedef struct
{
    Symbol *symbols;

    size_t count;
    size_t capacity;

} SymbolTable;


void symbol_init(SymbolTable *table);

void symbol_free(SymbolTable *table);


int symbol_set(SymbolTable *table,
               const char *name,
               double value);


int symbol_get(SymbolTable *table,
               const char *name,
               double *value);

int constant_get(const char *name,
                 double *value);
               
#endif