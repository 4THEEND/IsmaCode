#include <iostream>
#include <string>

#include "Parser.h"

int main()
{
    std::string entry{};
    while (true)
    {
        std::cout << ">> ";
        std::getline(std::cin, entry);

        if (entry == "exit")
            break;

        Lexer lexer = entry;
        if (lexer.isGood)
        {
            Parser parser(lexer.getInstructions(), {lexer.getToken(), lexer.getTokenReversed()});
        }
    }
    std::cin.get();
}