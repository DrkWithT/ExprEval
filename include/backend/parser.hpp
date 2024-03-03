#ifndef PARSER_HPP
#define PARSER_HPP

/**
 * Summary of math expression grammar: BNF
 * See: README.md
 */

#include "frontend/lexer.hpp"
#include "ast/expressions.hpp"

namespace eeval::backend
{
    class ExprAST
    {
    private:
        std::unique_ptr<eeval::ast::Expr> root;
    public:
        ExprAST();
        ExprAST(std::unique_ptr<eeval::ast::Expr> expr_root);
        ~ExprAST() = default;

        friend double evaluateIt(const ExprAST& ast);
    };

    enum TokenConsumeStatus
    {
        consume_eof,
        consume_ok,
        consume_optional
    };

    class Parser
    {
    private:
        std::ostringstream sout;
        eeval::frontend::Lexer lexer;
        eeval::frontend::Token previous;
        eeval::frontend::Token current;

        [[nodiscard]] const eeval::frontend::Token& getPrevious() const;
        [[nodiscard]] const eeval::frontend::Token& getCurrent() const;
        [[nodiscard]] bool isAtEOF() const;

        [[nodiscard]] eeval::frontend::Token advanceByToken();
        TokenConsumeStatus consumeToken(eeval::frontend::TokenType type_main, eeval::frontend::TokenType type_optional);

        std::unique_ptr<eeval::ast::Expr> parseUnary();
        std::unique_ptr<eeval::ast::Expr> parseTerm();
        std::unique_ptr<eeval::ast::Expr> parseFactor();
        std::unique_ptr<eeval::ast::Expr> parsePower();

    public:
        Parser();

        ExprAST parseSource(std::string_view source_view);
    };
}

#endif