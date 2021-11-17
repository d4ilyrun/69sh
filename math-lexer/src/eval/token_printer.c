#include <stdio.h>

#include "lexer.h"

char tab[] = {
    [TOKEN_PLUS] = '+', [TOKEN_MINUS] = '-',    [TOKEN_MUL] = '*',
    [TOKEN_DIV] = '/',  [TOKEN_LEFT_PAR] = '(', [TOKEN_RIGHT_PAR] = ')'
};

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    struct lexer *lexer = lexer_new(argv[1]);
    struct token *token = lexer_pop(lexer);

    while (token->type != TOKEN_EOF && token->type != TOKEN_ERROR)
    {
        if (token->type == TOKEN_NUMBER)
            printf("%zu\n", token->value);
        else
            printf("%c\n", tab[token->type]);

        token_free(token);
        token = lexer_pop(lexer);
    }

    if (token->type == TOKEN_EOF)
        printf("EOF\n");

    token_free(token);
    lexer_free(lexer);

    return 0;
}
