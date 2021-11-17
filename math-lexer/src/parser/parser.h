#pragma once

#include "ast.h"
#include "lexer.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
};

/**
 * \brief Creates an AST by reading and interpreting tokens
 *   from the parser.
 */
enum parser_status parse(struct ast **res, struct lexer *lexer);
