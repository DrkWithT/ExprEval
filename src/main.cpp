#include <stdexcept>
#include <iostream>
#include "backend/parser.hpp"

using MyMathExpr = eeval::ast::Expr;
using MyAST = eeval::backend::ExprAST;

int main()
{
    eeval::backend::Parser parser {};
    std::string input_string {};

    std::cout << "Enter an expression:\n";
    std::getline(std::cin, input_string);

    /// @note Catch all parse exceptions gracefully to avoid unexpected program quit.
    try
    {
        MyAST parse_tree = parser.parseSource(input_string);

        std::cout << evaluateIt(parse_tree) << '\n';
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
