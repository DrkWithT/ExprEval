#include <stdexcept>
#include <iostream>
#include "backend/parser.hpp"

using MyAST = eeval::backend::ExprAST;

int main()
{
    eeval::backend::Parser parser {};
    std::string input_string {};

    std::cout << "Enter an expression:\n";
    std::getline(std::cin, input_string);

    try
    {
        MyAST parse_tree = parser.parseSource(input_string);

        std::cout << evaluateIt(parse_tree) << '\n';
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
