#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "ast.h"
#include "semantic.h"
#include "evaluator.h"
#include "symbol_table.h"

int main(void)
{
    char input[256];
    SymbolTable symbols;

    symbol_init(&symbols);

    puts("Math Parser");
    puts("Type 'quit' to exit.\n");

    while (1)
    {
        printf("> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        /* Remove trailing newline */
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "quit") == 0)
            break;

        ASTNode *tree = parse(input);

        if (tree != NULL)
        {
            if (semantic_check(tree))
            {
                double result;

                if (evaluate(tree,
                             &symbols,
                             &result))
                {
                    printf("%g\n", result);
                }
            }

            ast_free(tree);
        }

        putchar('\n');
    }

    symbol_free(&symbols);

    return 0;
}