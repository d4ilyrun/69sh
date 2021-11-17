#pragma once

#include "token.h"

/**
 * \page Lexer
 *
 * The lexer cuts some input text into block called tokens.

 * This process is done **on demand**: the lexer doesn't read the
 * input more than it needs, and only creates tokens when lexer_peek
 * or lexer_pop is called, and no token is available.
 *
 * "2 + 3" will produce 3 tokens:
 *   - TOKEN_NUMBER { .value = 2 }
 *   - TOKEN_PLUS
 *   - TOKEN_NUMBER { .value = 3 }
 */

struct lexer
{
    const char *input; ///< The input data
    size_t pos; ///< The current offset inside the input data
    struct token *current_tok; ///< The next token, if processed
};

/**
 * \brief Creates a new lexer given an input string.
 */
struct lexer *lexer_new(const char *input);

/**
 ** \brief Free the given lexer, but not its input.
 */
void lexer_free(struct lexer *lexer);

/**
 * \brief Returns the next token, but doesn't move forward: calling lexer_peek
 * multiple times in a row always returns the same result. This functions is
 * meant to help the parser check if the next token matches some rule.
 */
struct token *lexer_peek(struct lexer *lexer);

/**
 * \brief Returns the next token, and removes it from the stream:
 *   calling lexer_pop in a loop will iterate over all tokens until EOF.
 */
struct token *lexer_pop(struct lexer *lexer);
