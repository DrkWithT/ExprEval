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
    : _value{value} {}

    double ValueExpr::interpret() const
    {
        return _value;
    }

    UnaryExpr::UnaryExpr(ValueExpr value, char op)
    : _value{value}, _op{op} {}

    double UnaryExpr::interpret() const
    {
        switch (_op)
        {
        case '+':
            return _value.interpret();
        case '-':
            return _value.interpret() * -1;
        default:
            throw std::runtime_error{"Invalid operator in UnaryExpr!"};
        }
    }

    BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, char symbol) : _left(std::move(left)), _right(std::move(right)), _symbol{symbol} {}

    double BinaryExpr::interpret() const
    {
        auto left_result = (*_left).interpret();
        auto right_result = (*_right).interpret();

        switch (_symbol)
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
