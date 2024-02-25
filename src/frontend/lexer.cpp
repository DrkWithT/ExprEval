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

        return (Token) {.begin = mark_pos, .length = 1, .type = token_type};
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

        return (Token) {.begin = token_start, .length = token_length, .type = token_whitespace};
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
            return (Token) {.begin = token_start, .length = token_length, .type = token_bad};

        return (Token) {.begin = token_start, .length = token_length, .type = token_number};
    }

    Token Lexer::lexNext()
    {
        if (position >= limit)
            return (Token) {.begin = limit, .length = 1, .type = token_eof};

        char symbol = source_view[position];

        switch (symbol)
        {
        case '+':
            return lexSingle(token_op_plus);
        case '-':
            return lexSingle(token_op_minus);
        case '*':
            return lexSingle(token_op_times);
        case '/':
            return lexSingle(token_op_slash);
        case '^':
            return lexSingle(token_op_expo);
        default:
            break;
        }

        if (matchSpacing(symbol))
            return lexSpacing();
        else if (matchNumeric(symbol))
            return lexNumeric();

        position++;

        return (Token) {.begin = position - 1, .length = 1, .type = token_bad};
    }
}
