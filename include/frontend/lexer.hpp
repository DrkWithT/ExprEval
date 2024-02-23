#ifndef LEXER_HPP
#define LEXER_HPP

#include "frontend/token.hpp"

namespace eeval::frontend
{
    constexpr bool matchSpacing(char c)
    {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n';
    }

    constexpr bool matchNumeric(char c)
    {
        return (c >= '0' && c <= '9') || c == '.';
    }

    class Lexer
    {
    private:
        std::string_view source_view;
        size_t position;
        size_t limit;

    public:
        Lexer();
        Lexer(std::string_view source_strview);

        std::string_view peekSource() const;
        void resetSelf(std::string_view new_source_strview);

        Token lexSingle(TokenType token_type);
        Token lexSpacing();
        Token lexNumeric();
        Token lexNext();
    };
}

#endif