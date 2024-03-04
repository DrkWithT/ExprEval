#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string_view>
#include <sstream>

namespace eeval::frontend
{
    enum TokenType
    {
        unknown,
        eof,
        whitespace,
        number,
        op_plus,
        op_minus,
        op_times,
        op_slash,
        op_expo,
        lparen,
        rparen
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