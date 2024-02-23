#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string_view>
#include <sstream>

namespace eeval::frontend
{
    enum TokenType
    {
        token_bad,
        token_eof,
        token_whitespace,
        token_number,
        token_op_plus,
        token_op_minus,
        token_op_times,
        token_op_slash,
        token_op_expo,
        token_lparen,
        token_rparen
    };

    struct Token
    {
        size_t begin;
        size_t length;
        TokenType type;
    };

    bool stringifyToken(const Token& token, std::string_view source, std::ostringstream& sout);
}

#endif