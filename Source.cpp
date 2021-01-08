#include <iostream>
#pragma once
#include "IMLAlgorithms.hpp"
#include "IMLParser.hpp"

int main()
{
    IMLDocument doc;
    ParseIMLDocument(&doc, "test1.iml");
}
