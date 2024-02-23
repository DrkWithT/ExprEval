#include <stdexcept>
#include <iostream>
#include "backend/parser.hpp"

using MyMathExpr = eeval::ast::Expr;
using MyAST = eeval::backend::ExprAST;

void printInfo()
{
    std::cout << "ExprEval: evalutate arithmetic expressions.\n";
    std::cout << "Enter \"end\" to stop the input to output loop.\n";
}

int main()
{
    eeval::backend::Parser parser {};
    std::string input_string;

    printInfo();

    while (true)
    {
        std::cout << ">> ";
        bool read_bad = std::getline(std::cin, input_string, '\n').fail();

        if (read_bad)
        {
            break;
        }

        if (input_string == "end")
        {
            break;
        }

        /// @note Catch all parse exceptions gracefully to avoid unexpected program quit.
        try
        {
            MyAST parse_tree = parser.parseSource(input_string);

            std::cout << evaluateIt(parse_tree) << '\n';
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        input_string.clear();
    }

    return 0;
}
