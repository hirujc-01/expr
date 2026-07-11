#include "lexer_tests.h"

void lexer_number_tests(void);
void lexer_identifier_tests(void);
void lexer_operator_tests(void);
void lexer_function_tests(void);
void lexer_error_tests(void);

void run_lexer_tests(void)
{
    lexer_number_tests();
    lexer_identifier_tests();
    lexer_operator_tests();
    lexer_function_tests();
    lexer_error_tests();
}