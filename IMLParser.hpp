#pragma once
#include "IMLAlgorithms.hpp"
#include <fstream>
#include <sstream>
#include <stack>
#include <list> 
#include <vector>
#include <string>
#include <iterator>
//----------------Structures for parse trees----------------//
struct IMLNode
{
public:
    std::string tag;
    std::vector<double> array;
    IMLNode* parent;
    double attributed;
    std::string attributes;
    int PositionOfArray;
    std::vector<IMLNode*> children;
    bool HasBeenInterpreted;
    IMLNode(std::string tag)
    {
        this->parent = nullptr;
        this->tag = tag;
        this->attributed;
        this->attributes = "";
        this->array = {};
        this->PositionOfArray = 0;
        this->children = {};
        HasBeenInterpreted = false;
    }
    IMLNode(IMLNode* parent)
    {
        this->parent = parent;
        if (parent)
            parent->children.push_back(this);
        PositionOfArray = this->parent->array.size();
        HasBeenInterpreted = false;
    }
    void free(IMLNode* root)
    {
        for (int i = 0; i < root->children.size(); i++)
            free(root->children[i]);

        delete root;
    }
};
struct IMLDocument
{
    IMLNode* root;

};
void free(IMLDocument* doc)
{
    free(doc->root);
}
//----------------Helper functions for double vector----------------//
std::vector<std::string> split(std::string str)
{
    std::string buf;
    std::stringstream ss(str);
    std::vector<std::string> tokens;

    while (ss >> buf)
        tokens.push_back(buf);
    return tokens;
}
std::vector<double> ConvertStringVectortoDoubleVector(const std::vector<std::string>& StringVector) {
    std::vector<double> DoubleVector(StringVector.size());
    std::transform(StringVector.begin(), StringVector.end(), DoubleVector.begin(), [](const std::string& val)
        {
            return stod(val);
        });
    return DoubleVector;
}
std::vector<double> ConvertStringtoDoubleVector(std::string str)
{
    try
    {
        return ConvertStringVectortoDoubleVector(split(str));
    }
    catch (...)
    {
        std::cout << "Wrong value of an element of the array.\n";
    }
}
//---------Functions for tag checks and tag interpretation---------//
int Found(std::string searched)
{
    std::vector<std::string> TagsWithAtributeD = { "MAP-INC", "MAP-MLT", "SRT-SLC" };
    std::vector<std::string> TagsWithoutAtribute = { "AGG-SUM", "AGG-PRO", "AGG-AVG",
                                                    "AGG-FST", "AGG-LST", "SRT-REV", "SRT-DST" };
    std::string TagsWithoutAtributeS = "SRT-ORD";
    for (size_t i = 0; i < TagsWithAtributeD.size(); i++)
    {
        if (searched == TagsWithAtributeD[i])
        {
            return 1;
        }
    }
    for (size_t i = 0; i < TagsWithoutAtribute.size(); i++)
    {
        if (searched == TagsWithoutAtribute[i])
        {
            return 2;
        }
    }
    if (searched == TagsWithoutAtributeS)
    {
        return 3;
    }
    return 0;
}
void InterpretTagOntoArray(IMLNode* CurrentNode)
{
    std::vector<std::string> Tags = { "MAP-INC", "MAP-MLT", "AGG-SUM", "AGG-PRO", "AGG-AVG",
                                      "AGG-FST", "AGG-LST", "SRT-REV", "SRT-ORD", "SRT-SLC", "SRT-DST" };
    int i = 0;
    for (i; i < Tags.size(); i++)
    {
        if (CurrentNode->tag == Tags[i])
        {
            break;
        }
    }
    double num = 0;
    switch (i)
    {
    case 0: MAPINC(CurrentNode->array, CurrentNode->attributed);
        break;
    case 1: MAPMLT(CurrentNode->array, CurrentNode->attributed);
        break;
    case 2: AGGSUM(CurrentNode->array);
        break;
    case 3: AGGPRO(CurrentNode->array);
        break;
    case 4: AGGAVG(CurrentNode->array);
        break;
    case 5: AGGFST(CurrentNode->array);
        break;
    case 6: AGGLST(CurrentNode->array);
        break;
    case 7: SRTREV(CurrentNode->array);
        break;
    case 8: SRTORD(CurrentNode->array, CurrentNode->attributes);
        break;
    case 9: SRTSLC(CurrentNode->array, CurrentNode->attributed);
        break;
    case 10: SRTDST(CurrentNode->array);
        break;

    default: std::cout << "I don't know how but you broke it, happy now?"; throw "WTF error!";
        break;
    }
}
//------------Parser for building the language tree------------//
std::string ParseFileToString(std::string path)
{
    std::ifstream file;
    if (file.fail()) {
        std::cout << "Failed to open file!";
        throw "Failed to open file!";
    }
    file.open(path); //open the input file

    std::stringstream sstream;
    sstream << file.rdbuf(); //read the file
    std::string buf = sstream.str(); //buf holds the content of the file
    return buf;
}
void ParseIMLDocument(IMLDocument* doc, std::string path)
{
    std::string buf = ParseFileToString(path);

    std::string tag;
    std::string ClosingTag;
    std::string array;
    std::string attribute;
    std::stack<std::string> tags;
    int i = 0;
    bool InTag = true;
    std::string root = "root";
    doc->root = new IMLNode(root);
    IMLNode* CurrentNode = doc->root;

    while (buf[i] != buf[buf.size()])
    {
        if (!InTag && !array.empty() && (buf[i] == '<' || buf[i] == '>'))
        {
            std::vector<double> arr = ConvertStringtoDoubleVector(array);
            for (int i = 0; i < arr.size(); i++)
            {
                CurrentNode->array.push_back(arr[i]);
            }
            array = {};
        }
        if (buf[i] == '<')
        {
            //closing tag
            if (buf[i + 1] == '/')
            {
                i += 2;
                while (buf[i] != '>')
                    ClosingTag.push_back(buf[i++]);

                if (CurrentNode->tag == ClosingTag)
                {
                    tags.pop();
                    CurrentNode = CurrentNode->parent;
                    ClosingTag = "";
                    InTag = false;
                    i++;
                    continue;
                }
                else
                {
                    std::cout << "Mismatched tags!";
                    throw "Mismatched tags!";
                }
            }

            //searching for tag
            tag = "";
            IMLNode* NewNode = new IMLNode(CurrentNode);
            CurrentNode = NewNode;
            i++;
            while (buf[i] != '>')
            {
                if (buf[i] == ' ')
                {
                    i++;
                }
                if (buf[i] == '\"')
                {
                    i++;
                    while (buf[i] != '\"')
                        attribute.push_back(buf[i++]);

                    if (Found(tag) == 1)
                    {
                        try
                        {
                            CurrentNode->attributed = stod(attribute);
                        }
                        catch (...)
                        {
                            std::cout << "Attribute with wrong value.\n";
                            throw "Attribute with wrong value.\n";
                        }
                    }
                    else if (Found(tag) == 3)
                    {
                        if (attribute == "ASC" || attribute == "DSC")
                        {
                            CurrentNode->attributes = attribute;
                        }
                        else
                        {
                            std::cout << "Attribute must be either ASC or DSC.\n";
                            throw "Attribute with wrong value.\n";
                        }

                    }
                    else if (Found(tag) == 2)
                    {
                        std::cout << "Added attribute whitout it being needed.\n";
                        throw "Added attribute whitout it being needed.\n";
                    }
                    else if (Found(attribute) == 0)
                    {
                        std::cout << "This tag is not supported.\n";
                        throw "This tag musn't have an attribute.\n";
                    }
                }
                if (!attribute.empty())
                {
                    attribute = "";
                    i++;
                    break;
                }
                tag.push_back(buf[i++]);
            }

            if (Found(tag) == 0)
            {
                std::cout << "Tag not supported!!!";
                throw "Tag not supported!";
            }
            tags.push(tag);
            CurrentNode->tag = tag;
            InTag = false;
            i++;
            continue;
        }
        else
        {
            array.push_back(buf[i++]);
        }
    }
    if (!tags.empty())
    {
        std::cout << "Tag not supported!";
        throw "Tag not supported!";
    }
}
//---------Functions for interpreting the parsed tree---------//
bool AllChildrenInterpreted(IMLNode* child)
{
    for (int i = 0; i < child->children.size(); i++)
    {
        if (!child->children[i]->HasBeenInterpreted)
        {
            return false;
        }
    }
    return true;
}
void InterpretChildren(IMLNode* child , int i)
{
    if (child->children.empty() || AllChildrenInterpreted(child))
    {
        if (!(child->parent->tag == "root"))
        {
            auto itPos = child->parent->array.begin() + child->PositionOfArray;
            InterpretTagOntoArray(child);
            child->parent->array.insert(itPos, child->array.begin(), child->array.end());
            child->HasBeenInterpreted = true;
            InterpretChildren(child->parent,i);
        }
        else
        {
            InterpretTagOntoArray(child);
            return;
        }
    }
    else
    {
        while (i < child->children.size())
        {
          InterpretChildren(child->children[i],i);
          i++;
        }

    }
}
std::vector<std::vector<double>> InterpretRoot(IMLDocument* doc)
{
    std::vector<std::vector<double>> res = {};

    for (int i = 0; i < doc->root->children.size(); i++)
    {
        InterpretChildren(doc->root->children[i],0);
    }
    for (int i = 0; i < doc->root->children.size(); i++)
    {
        res.push_back(doc->root->children[i]->array);
    }

    return res;
}
//-And driver function for making output file with the result-//
void InterpretToFile(std::string InputFile, std::string OutputFile)
{
    IMLDocument doc;
    ParseIMLDocument(&doc, InputFile);
    std::ofstream outputFile(OutputFile);
    std::vector<std::vector<double>> res = InterpretRoot(&doc);

    for (int i = 0; i < res.size(); i++)
    {
        std::reverse(begin(res[i]), end(res[i]));
    }
    std::reverse(begin(res), end(res));

    for (int i = 0; i < res.size(); i++) 
    {
        std::copy(res[i].rbegin(), res[i].rend(), std::ostream_iterator<int>(outputFile, " "));
    }
    std::cout << "Done.";
    void free();
}
//Helper functions for printing vector<double> and vector<vector<double>>//
void print(std::vector<double> const& input)
{
    for (int i = 0; i < input.size(); i++)
    {
        std::cout << input.at(i) << ' ';
    }
}
void Print(std::vector<std::vector<double>> const& input)
{
    for (int i = 0; i < input.size(); i++)
    {
        print(input[i]);
        std::cout << std::endl;
    }
}
