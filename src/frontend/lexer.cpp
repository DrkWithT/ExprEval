/**
 * @file lexer.cpp
 * @author DrkWithT
 * @brief Implements lexer for expressions.
 * @date 2024-02-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "frontend/lexer.hpp"

namespace eeval::frontend
{
    Lexer::Lexer()
    : source_view {}, position {0}, limit {0} {}

    Lexer::Lexer(std::string_view source_strview)
    : source_view {source_strview}, position {0}, limit {source_view.length()} {}

    std::string_view Lexer::peekSource() const
    {
        return source_view;
    }

    void Lexer::resetSelf(std::string_view new_source_strview)
    {
        source_view = new_source_strview;
        position = 0;
        limit = source_view.length();
    }

    Token Lexer::lexSingle(TokenType token_type)
    {
        size_t mark_pos = position;

        position++;

        return {.begin = mark_pos, .length = 1, .type = token_type};
    }

    Token Lexer::lexSpacing()
    {
        size_t token_start = position;
        size_t token_length = 0;
        char temp = '\0';

        while (position < limit)
        {
            temp = source_view[position];

            if (!matchSpacing(temp))
                break;

            token_length++;
            position++;
        }

        return {.begin = token_start, .length = token_length, .type = whitespace};
    }

    Token Lexer::lexNumeric()
    {
        size_t token_start = position;
        size_t token_length = 0;
        int dot_count = 0;
        char temp = '\0';

        while (position < limit)
        {
            temp = source_view[position];

            if (!matchNumeric(temp))
                break;

            if (temp == '.')
                dot_count++;

            token_length++;
            position++;
        }
        
        if (dot_count > 1)
            return {.begin = token_start, .length = token_length, .type = unknown};

        return {.begin = token_start, .length = token_length, .type = number};
    }

    Token Lexer::lexNext()
    {
        if (position >= limit)
            return {.begin = limit, .length = 1, .type = eof};

        char symbol = source_view[position];

        switch (symbol)
        {
        case '+':
            return lexSingle(op_plus);
        case '-':
            return lexSingle(op_minus);
        case '*':
            return lexSingle(op_times);
        case '/':
            return lexSingle(op_slash);
        case '^':
            return lexSingle(op_expo);
        case '(':
            return lexSingle(lparen);
        case ')':
            return lexSingle(rparen);
        default:
            break;
        }

        if (matchSpacing(symbol))
            return lexSpacing();
        else if (matchNumeric(symbol))
            return lexNumeric();

        position++;

        return {.begin = position - 1, .length = 1, .type = unknown};
    }
}
