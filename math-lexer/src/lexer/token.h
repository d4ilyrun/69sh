#pragma once

#include <unistd.h>

enum token_type
{
    TOKEN_PLUS, ///< '+'
    TOKEN_MINUS, ///< '-'
    TOKEN_MUL, ///< '*'
    TOKEN_DIV, ///< '/'
    TOKEN_NUMBER, ///< "[0-9]+"
    TOKEN_LEFT_PAR, ///< '('
    TOKEN_RIGHT_PAR, ///< ')'
    TOKEN_EOF, ///< end of input marker
    TOKEN_ERROR ///< tt's not a real token, it's returned in case of invalid
                ///< input
};

struct token
{
    enum token_type type; ///< The kind of token
    ssize_t value; ///< If the token is a number, its value
};

/**
 * \brief Allocate a new token
 */
struct token *token_new(enum token_type type);

/**
 * \brief Frees a token
 */
void token_free(struct token *token);
