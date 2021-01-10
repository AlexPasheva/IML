#pragma once
#include <iostream>
#include "IMLParser.hpp"

int main()
{
    std::string input;
    std::string output;
    std::cin >> input;
    std::cin >> output;
    InterpretToFile(input, output);
}