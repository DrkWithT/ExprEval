/**
 * @file expressions.cpp
 * @author DrkWithT
 * @brief Implements AST expression nodes.
 * @date 2024-02-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cmath>
#include <stdexcept>
#include <utility>
#include "ast/expressions.hpp"

namespace eeval::ast
{
    ValueExpr::ValueExpr(double value)
    : inner_value{value} {}

    double ValueExpr::interpret() const
    {
        return inner_value;
    }

    UnaryExpr::UnaryExpr(ValueExpr value, char op)
    : inner_value{value}, op_symbol{op} {}

    double UnaryExpr::interpret() const
    {
        switch (op_symbol)
        {
        case '+':
            return inner_value.interpret();
        case '-':
            return inner_value.interpret() * -1;
        default:
            throw std::runtime_error{"Invalid operator in UnaryExpr!"};
        }
    }

    BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, char symbol) : lhs(std::move(left)), rhs(std::move(right)), op_symbol{symbol} {}

    double BinaryExpr::interpret() const
    {
        auto left_result = (*lhs).interpret();
        auto right_result = (*rhs).interpret();

        switch (op_symbol)
        {
        case '+':
            return left_result + right_result;
        case '-':
            return left_result - right_result;
        case '*':
            return left_result * right_result;
        case '/':
            if (right_result == 0)
                throw std::runtime_error{"Cannot divide by 0 !"};

            return left_result / right_result;
        case '^':
            return std::pow(left_result, right_result);
        default:
            throw std::runtime_error{"Invalid operator in BinaryExpr!"};
        }
    }
}
