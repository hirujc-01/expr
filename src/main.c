#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "ast.h"

int main(void)
{
    char input[256];

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
            ast_print(tree, 0);
            ast_free(tree);
        }

        putchar('\n');
    }

    return 0;
}