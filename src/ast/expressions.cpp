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

    UnaryExpr::UnaryExpr(ValueExpr value, MathOperator oper)
    : inner_value {value}, op {oper} {}

    double UnaryExpr::interpret() const
    {
        switch (op)
        {
        case math_none:
            return inner_value.interpret();
        case math_subtract:
            return inner_value.interpret() * -1;
        default:
            throw std::runtime_error{"Invalid operator in UnaryExpr!"};
        }
    }

    BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, MathOperator oper) : lhs(std::move(left)), rhs(std::move(right)), op {oper} {}

    double BinaryExpr::interpret() const
    {
        auto left_result = lhs->interpret();
        auto right_result = rhs->interpret();

        switch (op)
        {
        case math_add:
            return left_result + right_result;
        case math_subtract:
            return left_result - right_result;
        case math_multiply:
            return left_result * right_result;
        case math_divide:
            if (right_result == 0)
                throw std::runtime_error{"Cannot divide by 0 !"};

            return left_result / right_result;
        case math_exponentiate:
            return std::pow(left_result, right_result);
        default:
            throw std::runtime_error{"Invalid operator in BinaryExpr!"};
        }
    }
}
