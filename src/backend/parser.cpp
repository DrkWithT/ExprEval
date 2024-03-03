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

namespace eeval::backend
{
    /* ExprAST impl. */

    ExprAST::ExprAST()
    : root {nullptr} {}

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

        if (current.type == eeval::frontend::TokenType::token_eof)
        {
            return TokenConsumeStatus::consume_eof;
        }

        throw std::invalid_argument {"Unexpected token (misplaced or EOF)!"};
    }

    std::unique_ptr<ExprGeneralNode> Parser::parseUnary()
    {
        const ExprToken& temp = getCurrent();
        double num_value = 0;

        if (temp.type == ExprTokenType::token_number)
        {
            eeval::frontend::stringifyToken(temp, lexer.peekSource(), sout);
            num_value = std::stod(sout.str());
            sout.str("");

            consumeToken(ExprTokenType::token_number, ExprTokenType::token_number);

            return std::make_unique<eeval::ast::ValueExpr>(num_value);
        }

        consumeToken(ExprTokenType::token_op_minus, ExprTokenType::token_op_minus);

        if (current.type == ExprTokenType::token_number)
        {
            eeval::frontend::stringifyToken(getCurrent(), lexer.peekSource(), sout);
            num_value = std::stod(sout.str());
            eeval::ast::ValueExpr value {num_value};
            sout.str("");

            return std::make_unique<eeval::ast::UnaryExpr>(value, '-');
        }

        consumeToken(ExprTokenType::token_number, ExprTokenType::token_number);

        return std::make_unique<eeval::ast::ValueExpr>(0);
    }

    std::unique_ptr<ExprGeneralNode> Parser::parseTerm()
    {
        auto left_expr = parseFactor();

        while (getCurrent().type != ExprTokenType::token_eof)
        {
            if (getCurrent().type != ExprTokenType::token_op_plus && getCurrent().type != ExprTokenType::token_op_minus)
            {
                break;
            }

            consumeToken(ExprTokenType::token_op_plus, ExprTokenType::token_op_minus);

            char op = (getPrevious().type == ExprTokenType::token_op_plus)
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

        while (getCurrent().type != ExprTokenType::token_eof)
        {
            if (getCurrent().type != ExprTokenType::token_op_times && getCurrent().type != ExprTokenType::token_op_slash)
            {
                break;
            }

            consumeToken(ExprTokenType::token_op_times, ExprTokenType::token_op_slash);

            char op = (getPrevious().type == ExprTokenType::token_op_times)
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

        while (getCurrent().type != ExprTokenType::token_eof)
        {
            if (getCurrent().type != ExprTokenType::token_op_expo)
            {
                break;
            }

            consumeToken(ExprTokenType::token_op_expo, ExprTokenType::token_op_expo);
            char op = '^';

            auto right_expr = parseUnary();

            left_expr = std::make_unique<eeval::ast::BinaryExpr>(std::move(left_expr), std::move(right_expr), op);
        }

        return left_expr;
    }

    /* Parser public impl. */

    Parser::Parser()
    : sout {}, lexer {}, previous {.type = ExprTokenType::token_eof}, current {.type = ExprTokenType::token_eof} {}

    ExprAST Parser::parseSource(std::string_view source_view)
    {
        lexer.resetSelf(source_view);

        if (consumeToken(ExprTokenType::token_eof, ExprTokenType::token_eof) == consume_eof)
        {
            return ExprAST {};
        }

        auto expr_root = parseTerm();

        return ExprAST {std::move(expr_root)};
    }
}
