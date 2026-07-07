#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"


#define INITIAL_CAPACITY 16


/*=========================================================
 * Initialize
 *========================================================*/

void symbol_init(SymbolTable *table)
{
    table->count = 0;

    table->capacity = INITIAL_CAPACITY;

    table->symbols =
        malloc(sizeof(Symbol) * table->capacity);


    if (table->symbols == NULL)
    {
        fprintf(stderr,
                "Failed to allocate symbol table\n");

        exit(EXIT_FAILURE);
    }
}


/*=========================================================
 * Cleanup
 *========================================================*/

void symbol_free(SymbolTable *table)
{
    free(table->symbols);

    table->symbols = NULL;
    table->count = 0;
    table->capacity = 0;
}


/*=========================================================
 * Find symbol index
 *========================================================*/

static int find_symbol(SymbolTable *table,
                       const char *name)
{
    for (size_t i = 0;
         i < table->count;
         i++)
    {
        if (strcmp(table->symbols[i].name,
                   name) == 0)
        {
            return (int)i;
        }
    }


    return -1;
}


/*=========================================================
 * Set variable
 *========================================================*/

int symbol_set(SymbolTable *table,
               const char *name,
               double value)
{
    int index =
        find_symbol(table, name);


    /*
        Existing variable
    */
    if (index >= 0)
    {
        table->symbols[index].value = value;

        return 1;
    }


    /*
        Resize if needed
    */
    if (table->count >= table->capacity)
    {
        size_t new_capacity = table->capacity * 2;

        Symbol *new_symbols =
            realloc(table->symbols,
                    sizeof(Symbol) *
                    new_capacity);


        if (new_symbols == NULL)
            return 0;


        table->symbols = new_symbols;
        table->capacity = new_capacity;
    }


    /*
        Add new symbol
    */
    strncpy(table->symbols[table->count].name,
            name,
            sizeof(table->symbols[table->count].name) - 1);


    table->symbols[table->count]
        .name[sizeof(table->symbols[table->count].name)-1]
        = '\0';


    table->symbols[table->count].value = value;


    table->count++;


    return 1;
}


/*=========================================================
 * Get variable
 *========================================================*/

int symbol_get(SymbolTable *table,
               const char *name,
               double *value)
{
    int index =
        find_symbol(table, name);


    if (index < 0)
        return 0;


    *value =
        table->symbols[index].value;


    return 1;
}