#ifndef PARSER_HPP
#define PARSER_HPP

/**
 * Synopsis of math expression grammar: BNF
 * 
 * whitespace ::= (SP | TAB | CR | LF)+
 * number ::= (DIGIT | ".")+
 * operators ::= "+" | "-" | "*" | "/"
 * 
 * value ::= number
 * unary ::= ("-"){0,1} value
 * binary-term ::= unary (("+" | "-") unary)*
 * binary-factor ::= term (("*" | "/") term)*
 * binary-power ::= power ("^" power)*
 * expr ::= value | unary | binary
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
        ExprAST(std::unique_ptr<eeval::ast::Expr> expr_root);
        ~ExprAST() = default;

        friend double evaluateIt(const ExprAST& ast);
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
        bool consumeToken(eeval::frontend::TokenType type_main, eeval::frontend::TokenType type_optional);

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