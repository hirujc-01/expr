#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "ast.h"
#include "semantic.h"
#include "evaluator.h"
#include "symbol_table.h"
#include "color.h"
#include "angle_mode.h"
#include "optimizer.h"

static int handle_command(const char *input)
{
    if (strcmp(input, "quit") == 0 ||
        strcmp(input, "exit") == 0)
    {
        return 1;
    }

    if (strncmp(input, "mode", 4) == 0)
    {
        if (strcmp(input, "mode") == 0)
        {
            printf(BOLD COLOR_CYAN
                   "Current angle mode: %s\n"
                   COLOR_RESET,
                   get_angle_mode() == ANGLE_DEG ?
                   "degrees" : "radians");

            return 2;
        }

        if (strcmp(input, "mode deg") == 0)
        {
            set_angle_mode(ANGLE_DEG);

            printf(BOLD COLOR_GREEN
                   "Angle mode: degrees\n"
                   COLOR_RESET);

            return 2;
        }

        if (strcmp(input, "mode rad") == 0)
        {
            set_angle_mode(ANGLE_RAD);

            printf(BOLD COLOR_GREEN
                   "Angle mode: radians\n"
                   COLOR_RESET);

            return 2;
        }

        printf(BOLD COLOR_RED
               "Unknown mode.\n"
               COLOR_RESET);

        printf("Available modes:\n");
        printf(BOLD COLOR_GREEN
                "  mode rad\n"
                COLOR_RESET);
        printf(BOLD COLOR_GREEN
                "  mode deg\n"
                COLOR_RESET);

        return 2;
    }

    return 0;
}

int main(void)
{
    char input[256];

    SymbolTable symbols;

    symbol_init(&symbols);


    printf(BOLD COLOR_CYAN
           "Math Parser\n"
           COLOR_RESET);

    printf(COLOR_YELLOW
           "Type 'quit' to exit.\n\n"
           COLOR_RESET);



    while (1)
    {
        printf(COLOR_GREEN "> " COLOR_RESET);


        if (fgets(input,
                  sizeof(input),
                  stdin) == NULL)
            break;


        input[strcspn(input,"\n")] = '\0';


        int command = handle_command(input);


        if (command == 1)
        {
            printf(BOLD COLOR_YELLOW
                "Goodbye!\n"
                COLOR_RESET);

            break;
        }


        if (command == 2)
            continue;



        ASTNode *tree = parse(input);


        if (tree == NULL)
            continue;



        printf("\n" BOLD COLOR_BLUE
            "Syntax Tree:\n"
            COLOR_RESET);

        ast_print(tree,0);

        tree = optimize(tree);

        printf("\n" BOLD COLOR_CYAN
            "Optimized Tree:\n"
            COLOR_RESET);

        ast_print(tree,0);

        if (semantic_check(tree))
        {
            double result;


            if (evaluate(tree,
                         &symbols,
                         &result))
            {
                printf("\n" BOLD COLOR_MAGENTA
                       "Result: "
                       COLOR_RESET);

                printf("%g\n", result);
            }
        }


        ast_free(tree);

        putchar('\n');
    }


    symbol_free(&symbols);


    return 0;
}