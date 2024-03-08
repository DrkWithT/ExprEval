#include <stdexcept>
#include <iostream>
#include "backend/parser.hpp"

using MyAST = eeval::backend::ExprAST;

int main()
{
    eeval::backend::Parser parser {};
    std::string input_string {};

    std::cout << "ExprEval\nEnter end to stop the REPL.\n";

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin >> std::ws, input_string);

        if (input_string == "end")
        {
            break;
        }

        try
        {
            MyAST parse_tree = parser.parseSource(input_string);

            std::cout << evaluateIt(parse_tree) << '\n';
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    return 0;
}
