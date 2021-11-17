#pragma once

#include <unistd.h>

enum ast_type
{
    AST_PLUS,
    AST_MINUS,
    AST_MUL,
    AST_DIV,
    AST_NUMBER,
    AST_NEG
};

/**
 * This very simple AST structure should be sufficient for such a simple AST.
 * It is however, NOT GOOD ENOUGH for more complicated projects, such as a
 * shell. Please read the project guide for some insights about other kinds of
 * ASTs.
 */
struct ast
{
    enum ast_type type; ///< The kind of node we're dealing with
    ssize_t value; ///< If the node is a number, it stores its value
    struct ast *left; ///< The left branch if any, unuary or binary
    struct ast *right; ///< The right branch of the binary node
};

/**
 ** \brief Allocate a new ast with the given type
 */
struct ast *ast_new(enum ast_type type);

/**
 ** \brief Recursively free the given ast
 */
void ast_free(struct ast *ast);
