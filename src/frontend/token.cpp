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
        if (token.begin >= source.length() || token.length == 0)
        {
            return false;
        }

        auto seeker = source.begin() + token.begin;
        auto end = source.begin() + token.begin + token.length;

        while (seeker != end)
        {
            sout << *seeker;
            seeker++;
        }

        return true;
    }
};
