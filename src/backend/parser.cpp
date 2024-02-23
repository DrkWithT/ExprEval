/**
 * @file parser.cpp
 * @author DrkWithT
 * @brief Implements recursive descent parser for math expressions. Generates AST for walking.
 * @date 2024-02-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdexcept>
#include <array>
#include "backend/parser.hpp"

using ExprTokenType = eeval::frontend::TokenType;
using ExprToken = eeval::frontend::Token;
using ExprGeneralNode = eeval::ast::Expr;

namespace eeval::backend
{
    /* ExprAST impl. */

    ExprAST::ExprAST(std::unique_ptr<eeval::ast::Expr> expr_root)
    : root(std::move(expr_root)) {}

    [[nodiscard]] double evaluateIt(const ExprAST& ast)
    {
        if (!ast.root)
        {
            /// @note I'm not sure about how to handle a null AST likely from a premature EOF, so I'll just return placeholder 0.
            return 0;
        }

        return (*ast.root).interpret();
    }

    /* Parser private impls. */

    [[nodiscard]] const eeval::frontend::Token& Parser::getPrevious() const
    {
        return previous;
    }

    [[nodiscard]] const eeval::frontend::Token& Parser::getCurrent() const
    {
        return current;
    }

    [[nodiscard]] bool Parser::isAtEOF() const
    {
        return current.type == ExprTokenType::token_eof;
    }

    [[nodiscard]] eeval::frontend::Token Parser::advanceByToken()
    {
        ExprToken temp;

        do
        {
            temp = lexer.lexNext();

            if (temp.type == ExprTokenType::token_eof)
            {
                break;
            }

            if (temp.type == ExprTokenType::token_bad)
            {
                throw std::invalid_argument {"Invalid token passed."};
            }

            if (temp.type != ExprTokenType::token_whitespace)
            {
                break;
            }

        } while (true);

        return temp;
    }

    bool Parser::consumeToken(eeval::frontend::TokenType type_main, eeval::frontend::TokenType type_optional)
    {
        previous = current;
        current = advanceByToken();

        if (current.type == eeval::frontend::TokenType::token_eof)
        {
            return false;
        }

        if (current.type == type_main || current.type == type_optional)
        {
            return true;
        }

        throw std::invalid_argument {"Unexpected token, possibly a mismatch or EOF."};
    }

    std::unique_ptr<ExprGeneralNode> Parser::parseUnary()
    {
        if (!consumeToken(ExprTokenType::token_number, ExprTokenType::token_op_minus))
        {
            return std::make_unique<eeval::ast::ValueExpr>(0);
        }

        const ExprToken& temp = getCurrent();
        eeval::frontend::stringifyToken(temp, lexer.peekSource(), sout);
        double num_value = 0;

        if (temp.type == ExprTokenType::token_number)
        {
            num_value = std::stod(sout.str());
            sout.clear();
            return std::make_unique<eeval::ast::ValueExpr>(num_value);
        }

        sout.clear();

        consumeToken(ExprTokenType::token_number, ExprTokenType::token_number);
        eeval::frontend::stringifyToken(getCurrent(), lexer.peekSource(), sout);

        eeval::ast::ValueExpr value {num_value};

        sout.clear();

        return std::make_unique<eeval::ast::UnaryExpr>(value, '-');
    }

    std::unique_ptr<ExprGeneralNode> Parser::parseTerm()
    {
        auto left_expr = parseFactor();

        while (consumeToken(ExprTokenType::token_op_plus, ExprTokenType::token_op_minus))
        {
            char op = (getCurrent().type == ExprTokenType::token_op_plus)
                ? '+'
                : '-';

            auto right_expr = parseFactor();

            left_expr = std::make_unique<eeval::ast::BinaryExpr>(std::move(left_expr), std::move(right_expr), op);
        }

        return left_expr;
    }

    std::unique_ptr<ExprGeneralNode> Parser::parseFactor()
    {
        auto left_expr = parsePower();

        while (consumeToken(ExprTokenType::token_op_times, ExprTokenType::token_op_slash))
        {
            char op = (getCurrent().type == ExprTokenType::token_op_times)
                ? '*'
                : '/';

            auto right_expr = parsePower();

            left_expr = std::make_unique<eeval::ast::BinaryExpr>(std::move(left_expr), std::move(right_expr), op);
        }

        return left_expr;
    }

    std::unique_ptr<ExprGeneralNode> Parser::parsePower()
    {
        auto left_expr = parseUnary();

        while (consumeToken(ExprTokenType::token_op_expo, ExprTokenType::token_op_expo))
        {
            char op = '^';

            auto right_expr = parseUnary();

            left_expr = std::make_unique<eeval::ast::BinaryExpr>(std::move(left_expr), std::move(right_expr), op);
        }

        return left_expr;
    }

    /* Parser public impl. */

    Parser::Parser()
    : sout {}, lexer {}, previous {}, current {} {}

    ExprAST Parser::parseSource(std::string_view source_view)
    {
        lexer.resetSelf(source_view);

        auto expr_root = parsePower();

        return ExprAST {std::move(expr_root)};
    }
}
