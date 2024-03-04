#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <memory>
#include "ast/base_expr.hpp"

namespace eeval::ast
{
    /// TODO: Replace character operators with deduced enum values. These should be assigned based on operator symbols in the parser.
    enum MathOperator
    {
        math_add,
        math_subtract,
        math_multiply,
        math_divide,
        math_exponentiate
    };

    class ValueExpr : public Expr
    {
    private:
        double inner_value;

    public:
        ValueExpr(double value);

        [[nodiscard]] double interpret() const override;
    };

    class UnaryExpr : public Expr
    {
    private:
        ValueExpr inner_value;
        char op_symbol;

    public:
        UnaryExpr(ValueExpr value, char op);

        [[nodiscard]] double interpret() const override;
    };

    class BinaryExpr : public Expr
    {
    private:
        std::unique_ptr<Expr> lhs;
        std::unique_ptr<Expr> rhs;
        char op_symbol;

    public:
        BinaryExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, char symbol);

        [[nodiscard]] double interpret() const override;
    };
}

#endif