#include "parser.h"

#include <err.h>
#include <stdbool.h>

static enum parser_status parse_exp(struct ast **res, struct lexer *lexer);
static enum parser_status parse_sexp(struct ast **res, struct lexer *lexer);
static enum parser_status parse_texp(struct ast **res, struct lexer *lexer);
static enum parser_status parse_number(struct ast **res, struct lexer *lexer);
static enum parser_status parse_parenthesis(struct ast **res,
                                            struct lexer *lexer);

static enum parser_status handle_parse_error(enum parser_status status,
                                             struct ast **res)
{
    warnx("unexpected token");
    ast_free(*res);
    *res = NULL;
    return status;
}

/**
 * \brief Parse either an expression, or nothing
 *
 * input:  EOF  |  exp EOF
 */
enum parser_status parse(struct ast **res, struct lexer *lexer)
{
    // If we're at the end of file, there's no input
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_EOF)
    {
        *res = NULL;
        return PARSER_OK;
    }

    // try to parse an expression. if an error occured, free the
    // produced ast and return the same error code
    enum parser_status status = parse_exp(res, lexer);
    if (status != PARSER_OK)
        return handle_parse_error(status, res);

    // once parsing the expression is done, we should have
    // reached the end of file.
    if (lexer_peek(lexer)->type == TOKEN_EOF)
        return PARSER_OK;

    // if we didn't reach the end of file, it's an error
    return handle_parse_error(PARSER_UNEXPECTED_TOKEN, res);
}

/**
 * \brief Parse sexp expressions separated by + and -
 *
 * exp:      sexp  ('+' sexp | '-' sexp)*
 */
static enum parser_status parse_exp(struct ast **res, struct lexer *lexer)
{
    enum parser_status status = parse_sexp(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        // have a look at the type of the next token. Stop if not interested
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_PLUS && tok->type != TOKEN_MINUS)
            return PARSER_OK;

        // make the new ast node and discard the token
        struct ast *new_ast;
        if (tok->type == TOKEN_PLUS)
            new_ast = ast_new(AST_PLUS);
        else
            new_ast = ast_new(AST_MINUS);
        token_free(lexer_pop(lexer));

        // attach the new node to the ast
        new_ast->left = *res;
        *res = new_ast;

        // parse the stuff at the right
        if ((status = parse_sexp(&new_ast->right, lexer)) != PARSER_OK)
            return status;
    }
}

/**
 * \brief Parse texp expressions separated by * and /
 *
 * sexp:      texp  ('*' texp | '/' texp)*
 */
static enum parser_status parse_sexp(struct ast **res, struct lexer *lexer)
{
    enum parser_status status = parse_texp(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        // have a look at the type of the next token. Stop if not interested
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_MUL && tok->type != TOKEN_DIV)
            return PARSER_OK;

        // make the new ast node and discard the token
        struct ast *new_ast;
        if (tok->type == TOKEN_MUL)
            new_ast = ast_new(AST_MUL);
        else
            new_ast = ast_new(AST_DIV);
        token_free(lexer_pop(lexer));

        // attach the new node to the ast
        new_ast->left = *res;
        *res = new_ast;

        // parse the stuff at the right
        if ((status = parse_texp(&new_ast->right, lexer)) != PARSER_OK)
            return status;
    }
}

/**
 * \brief Parse either a number, a - a number, or a parenthesized expression
 *
 * texp:      NUMBER
 *         |  '-' NUMBER
 *         |  '(' exp ')'
 *         |  '-' '(' exp ')'
 */
static enum parser_status parse_texp(struct ast **res, struct lexer *lexer)
{
    // if we meet a -, create a negate node, and change the result pointer to
    // its inside
    if (lexer_peek(lexer)->type == TOKEN_MINUS)
    {
        token_free(lexer_pop(lexer));
        struct ast *negate_node = ast_new(AST_NEG);
        res = &negate_node->left;
    }

    // if we meet a number, parse it
    if (lexer_peek(lexer)->type == TOKEN_NUMBER)
        return parse_number(res, lexer);

    // if we meet an opening parenthesis, parses a parenthesized group
    if (lexer_peek(lexer)->type == TOKEN_LEFT_PAR)
        return parse_parenthesis(res, lexer);

    // otherwise, that's an error
    return PARSER_UNEXPECTED_TOKEN;
}

/**
 * \brief Parse a number. Assumes the next token is a number.
 */
static enum parser_status parse_number(struct ast **res, struct lexer *lexer)
{
    struct token *tok = lexer_pop(lexer);

    struct ast *ast = ast_new(AST_NUMBER);
    ast->value = tok->value;
    *res = ast;

    token_free(tok);
    return PARSER_OK;
}

/**
 * \brief Parse a parenthesized expression. Assumes the next token is an opening
 * parenthesis.
 */
static enum parser_status parse_parenthesis(struct ast **res,
                                            struct lexer *lexer)
{
    // get rid of the opening parenthesis
    token_free(lexer_pop(lexer));

    // parse what's between the parenthesis
    enum parser_status status = parse_exp(res, lexer);
    if (status != PARSER_OK)
        return status;

    // if everything went well, the parse_exp should have
    // returned because it met a ')'. If not, that's an error
    if (lexer_peek(lexer)->type != TOKEN_RIGHT_PAR)
        return PARSER_UNEXPECTED_TOKEN;

    token_free(lexer_pop(lexer));
    return PARSER_OK;
}
