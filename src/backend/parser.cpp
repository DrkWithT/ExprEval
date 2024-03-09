/**
 * @file parser.cpp
 * @author DrkWithT
 * @brief Implements recursive descent parser for math expressions. Generates AST for walking.
 * @note This uses an "interpreter" interface pattern to make the AST-walk evaluation a bit cleaner. :)
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
using ExprOperator = eeval::ast::MathOperator;

namespace eeval::backend
{
    /* ExprAST impl. */

    ExprAST::ExprAST()
    : root {nullptr} {}

    ExprAST::ExprAST(std::unique_ptr<eeval::ast::Expr> expr_root)
    : root(std::move(expr_root)) {}

    double evaluateIt(const ExprAST& ast)
    {
        if (!ast.root)
        {
            /// @note I'm not sure about how to handle a null AST likely from a premature EOF, so I'll just return placeholder 0.
            return 0;
        }

        return ast.root->interpret();
    }

    /* Parser private impls. */

    const eeval::frontend::Token& Parser::getPrevious() const
    {
        return previous;
    }

    const eeval::frontend::Token& Parser::getCurrent() const
    {
        return current;
    }

    bool Parser::isAtEOF() const
    {
        return current.type == ExprTokenType::eof;
    }

    eeval::frontend::Token Parser::advanceByToken()
    {
        ExprToken temp;

        do
        {
            temp = lexer.lexNext();

            if (temp.type == ExprTokenType::eof)
            {
                break;
            }

            if (temp.type == ExprTokenType::unknown)
            {
                throw std::invalid_argument {"Invalid token passed."};
            }

            if (temp.type != ExprTokenType::whitespace)
            {
                break;
            }

        } while (true);

        return temp;
    }

    TokenConsumeStatus Parser::consumeToken(eeval::frontend::TokenType type_main, eeval::frontend::TokenType type_optional)
    {
        if (current.type == type_main)
        {
            previous = current;
            current = advanceByToken();
            return TokenConsumeStatus::consume_ok;
        }

        if (current.type == type_optional)
        {
            previous = current;
            current = advanceByToken();
            return TokenConsumeStatus::consume_optional;
        }

        if (current.type == eeval::frontend::TokenType::eof)
        {
            return TokenConsumeStatus::consume_eof;
        }

        throw std::invalid_argument {"Unexpected token (misplaced or EOF)!"};
    }

    std::unique_ptr<ExprGeneralNode> Parser::parseUnary()
    {
        const ExprToken& temp = getCurrent();
        double num_value = 0;

        if (temp.type == ExprTokenType::number)
        {
            eeval::frontend::stringifyToken(temp, lexer.peekSource(), sout);
            num_value = std::stod(sout.str());
            sout.str("");

            consumeToken(ExprTokenType::number, ExprTokenType::number);

            return std::make_unique<eeval::ast::ValueExpr>(num_value);
        }

        consumeToken(ExprTokenType::op_minus, ExprTokenType::op_minus);

        if (current.type == ExprTokenType::number)
        {
            eeval::frontend::stringifyToken(getCurrent(), lexer.peekSource(), sout);
            num_value = std::stod(sout.str());
            eeval::ast::ValueExpr value {num_value};
            sout.str("");

            return std::make_unique<eeval::ast::UnaryExpr>(value, ExprOperator::math_subtract);
        }

        consumeToken(ExprTokenType::number, ExprTokenType::number);

        return std::make_unique<eeval::ast::ValueExpr>(0);
    }

    std::unique_ptr<ExprGeneralNode> Parser::parseTerm()
    {
        auto left_expr = parseFactor();

        while (getCurrent().type != ExprTokenType::eof)
        {
            if (getCurrent().type != ExprTokenType::op_plus && getCurrent().type != ExprTokenType::op_minus)
            {
                break;
            }

            consumeToken(ExprTokenType::op_plus, ExprTokenType::op_minus);

            ExprOperator op = (getPrevious().type == ExprTokenType::op_plus)
                ? ExprOperator::math_add
                : ExprOperator::math_subtract;

            auto right_expr = parseFactor();

            left_expr = std::make_unique<eeval::ast::BinaryExpr>(std::move(left_expr), std::move(right_expr), op);
        }

        return left_expr;
    }

    std::unique_ptr<ExprGeneralNode> Parser::parseFactor()
    {
        auto left_expr = parsePower();

        while (getCurrent().type != ExprTokenType::eof)
        {
            if (getCurrent().type != ExprTokenType::op_times && getCurrent().type != ExprTokenType::op_slash)
            {
                break;
            }

            consumeToken(ExprTokenType::op_times, ExprTokenType::op_slash);

            ExprOperator op = (getPrevious().type == ExprTokenType::op_times)
                ? ExprOperator::math_multiply
                : ExprOperator::math_divide;

            auto right_expr = parsePower();

            left_expr = std::make_unique<eeval::ast::BinaryExpr>(std::move(left_expr), std::move(right_expr), op);
        }

        return left_expr;
    }

    std::unique_ptr<ExprGeneralNode> Parser::parsePower()
    {
        auto left_expr = parseUnary();

        while (getCurrent().type != ExprTokenType::eof)
        {
            if (getCurrent().type != ExprTokenType::op_expo)
            {
                break;
            }

            consumeToken(ExprTokenType::op_expo, ExprTokenType::op_expo);

            auto right_expr = parseUnary();

            left_expr = std::make_unique<eeval::ast::BinaryExpr>(std::move(left_expr), std::move(right_expr), ExprOperator::math_exponentiate);
        }

        return left_expr;
    }

    /* Parser public impl. */

    Parser::Parser()
    : sout {}, lexer {}, previous {.type = ExprTokenType::eof}, current {.type = ExprTokenType::eof} {}

    ExprAST Parser::parseSource(std::string_view source_view)
    {
        lexer.resetSelf(source_view);

        if (consumeToken(ExprTokenType::eof, ExprTokenType::eof) == consume_eof)
        {
            return ExprAST {};
        }

        auto expr_root = parseTerm();

        return ExprAST {std::move(expr_root)};
    }
}
