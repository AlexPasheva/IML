#pragma once
#include <iostream>
#include "IMLParser.hpp"

int main()
{
    InterpretToFile("test1.iml", "testOut.txt");

    /*IMLDocument doc;
    ParseIMLDocument(&doc, "test1.iml");
    Print(InterpretRoot(&doc));
    std::cout << doc.root->children[0]->tag;
    std::cout << doc.root->children[0]->children[0]->tag;
    std::vector<double> data = { 1,2,3,4,5,6,7 };
    SRTSLC(data, 8);
    for (size_t i = 0; i < data.size(); i++)
    {
        std::cout << data[i];
    }
    std::vector<std::string> Tags = { "MAP-INC", "MAP-MLT", "AGG-SUM", "AGG-PRO", "AGG-AVG",
                                  "AGG-FST", "AGG-LST", "SRT-REV", "SRT-ORD", "SRT-SLC", "SRT-DST" };
    std::string n = "SRT-ORD";
    int i = 0;
    for (i; i < Tags.size(); i++)
    {
        if (n == Tags[i])
        {
            break;
        }
    }
    std::cout << i;
    std::vector <double> data = { 4, 8, 4, 3, 2, 10, 30, 10, 9, 30 };
    
    SRTDST(data);
    for (int i=0 ; i < data.size(); i++)
    {
        std::cout << data[i] << ", ";
    }*/
}
