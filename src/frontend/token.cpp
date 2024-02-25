/**
 * @file token.cpp
 * @author DrkWithT
 * @brief Implements token helper functions.
 * @date 2024-02-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "frontend/token.hpp"

namespace eeval::frontend
{
    bool stringifyToken(const Token& token, std::string_view source, std::ostringstream& sout)
    {
        size_t token_begin = token.begin;

        if (token_begin >= source.length() || token.length == 0)
        {
            return false;
        }

        size_t token_end = token_begin + token.length;

        for (size_t token_pos = token_begin; token_pos < token_end; token_pos++)
        {
            sout << source[token_pos];
        }

        return true;
    }
};
