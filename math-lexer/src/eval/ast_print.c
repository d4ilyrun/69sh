#include <stdio.h>

#include "lexer.h"
#include "parser.h"

char tab[] = { [AST_PLUS] = '+',
               [AST_MINUS] = '-',
               [AST_MUL] = '*',
               [AST_DIV] = '/' };

void print_ast(struct ast *ast)
{
    if (ast == NULL)
        return;

    if (ast->type == AST_NUMBER)
        printf("%zu", ast->value);
    else if (ast->type == AST_NEG)
        printf("-%zu", (ast->left)->value);
    else
    {
        printf("(");

        print_ast(ast->left);

        printf("%c", tab[ast->type]);

        print_ast(ast->right);

        printf(")");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    struct lexer *lexer = lexer_new(argv[1]);

    struct ast *ast;
    enum parser_status status = parse(&ast, lexer);
    if (status != PARSER_OK)
    {
        lexer_free(lexer);
        return 1;
    }

    print_ast(ast);
    printf("\n");

    ast_free(ast);
    lexer_free(lexer);

    return 0;
}
