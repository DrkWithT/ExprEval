#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <memory>
#include "ast/base_expr.hpp"

namespace eeval::ast
{
    class ValueExpr : public Expr
    {
    private:
        double _value;

    public:
        ValueExpr(double value);

        [[nodiscard]] double interpret() const override;
    };

    class UnaryExpr : public Expr
    {
    private:
        ValueExpr _value;
        char _op;

    public:
        UnaryExpr(ValueExpr value, char op);

        [[nodiscard]] double interpret() const override;
    };

    class BinaryExpr : public Expr
    {
    private:
        std::unique_ptr<Expr> _left;
        std::unique_ptr<Expr> _right;
        char _symbol;

    public:
        BinaryExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, char symbol);

        [[nodiscard]] double interpret() const override;
    };
}

#endif