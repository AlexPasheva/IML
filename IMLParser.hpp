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
    std::string tag;//name of the tag used for interpretation
    std::vector<double> array;//array of nubers that sits inbetween two tags
    IMLNode* parent;// a parent tag so the parsing can go down the tree...or up whatever
    double attributed;// attributes for tags tags must have double or int (for int it is checked in interpreted functions) attributes
    std::string attributes;// attributes for tags must have string attributes
    int PositionOfArray; // this field holds the position that the current array must be when interpreted into the parent array
    std::vector<IMLNode*> children; //children vector that enables nesting not only with one child but many 
    bool HasBeenInterpreted; // for the interpreter function a way to tell if this array has been interpreted
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
    IMLNode(IMLNode* parent) //constructor with parent node
    {
        this->parent = parent;
        if (parent)
            parent->children.push_back(this);
        PositionOfArray = this->parent->array.size();
        HasBeenInterpreted = false; //made false bu default
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
    IMLNode* root; //exists only for readability
};
void free(IMLDocument* doc)
{
    free(doc->root);
}
//----------------Helper functions for double vector----------------//
std::vector<std::string> split(std::string str)//splits the string by spaces and spits vector of strings
{
    std::string buf;
    std::stringstream ss(str);
    std::vector<std::string> tokens;

    while (ss >> buf)
        tokens.push_back(buf);
    return tokens;
}
std::vector<double> ConvertStringVectortoDoubleVector(const std::vector<std::string>& StringVector)//makes a vector of strings into a vector of doubles
{
    std::vector<double> DoubleVector(StringVector.size());
    std::transform(StringVector.begin(), StringVector.end(), DoubleVector.begin(), [](const std::string& val)
        {
            return stod(val);
        });
    return DoubleVector;
}
std::vector<double> ConvertStringtoDoubleVector(std::string str) //catches the errors of the function above 
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
int Found(std::string searched) //checks if the tag is one of the available options and then checks if its one with attribute
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
void InterpretTagOntoArray(IMLNode* CurrentNode) //for the interpretation part it converts the array by given function
{                                                //and I know these things can be done with lambda expressions and not a whole another hpp but i have no time for that sorry
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
std::string ParseFileToString(std::string path) // function that parses a file to a string
{                                               // that isn't a really good way to get all the info from file but it does the work with small files
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
    std::string buf = ParseFileToString(path);// a buffer that holds all chars from the input file
    //these are all lexers and if I had time I would have done it all properly with lexer tokenizer and parser but...no time
    std::string tag;//holds tag
    std::string ClosingTag;//holds closing tag
    std::string array;//holds double array
    std::string attribute;// holds attributes
    std::stack<std::string> tags;// stack for checking if tag nesting isnt fucked up
    int i = 0;
    bool InTag = true;//I...dont think i need that but its easily removed anyways
    std::string root = "root";
    doc->root = new IMLNode(root);
    IMLNode* CurrentNode = doc->root;//makes a root that holds all tags that are independent of each other

    while (buf[i] != buf[buf.size()])
    {
        //this is if for parsing the array of numbers
        if (!InTag && !array.empty() && (buf[i] == '<' || buf[i] == '>'))
        {
            std::vector<double> arr = ConvertStringtoDoubleVector(array);
            for (int i = 0; i < arr.size(); i++)
            {
                CurrentNode->array.push_back(arr[i]);
            }
            array = {};
        }
        //this is if we are in tag
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
                //saw an attribute
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
            // if the tag is not one of the supported throw error
            if (Found(tag) == 0)
            {
                std::cout << "Tag not supported!!!";
                throw "Tag not supported!";
            }
            // else set node tag to lexer tag
            tags.push(tag);
            CurrentNode->tag = tag;
            InTag = false;
            i++;
            continue;
        }
        else
        {
            //goes throw the whole array
            array.push_back(buf[i++]);
        }
    }
    if (!tags.empty())//finaly checks if everything with nesting is ok
    {
        std::cout << "Tag not supported!";
        throw "Tag not supported!";
    }
}
//---------Functions for interpreting the parsed tree---------//
bool AllChildrenInterpreted(IMLNode* child) //checks if all children of that node have been interpreted
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
void InterpretChildren(IMLNode* child);
void InterpretChild(IMLNode* child)//that functions inteprets a certain child
{
    if (!(child->parent->tag == "root") && (child->children.empty() || AllChildrenInterpreted(child)))
    {
        auto itPos = child->parent->array.begin() + child->PositionOfArray; //I know that using auto is not very cool but just one time
        InterpretTagOntoArray(child);//that does the magic with interpeting the array with certain function
        child->parent->array.insert(itPos, child->array.begin(), child->array.end());//puts it in the place it shoud be in the parent array
        child->HasBeenInterpreted = true;//marks it as intepreted
        InterpretChildren(child->parent);//and goes up..or down
    }
    else
    {
        InterpretChildren(child);//in case its first iteration or not all children have been interpreted
    }
}
void InterpretChildren(IMLNode* child)//it makes sure that all children have been intepreted and if not it just...goes to the other child
{
    int i = 0;
    while (i < child->children.size())
    {
        if (child->children[i]->HasBeenInterpreted)
        {
            i++;
        }
        else
        {
            InterpretChild(child->children[i]);
        }
    }
}
std::vector<std::vector<double>> InterpretRoot(IMLDocument* doc)//this inteprets the document root and spits a vector of vector of all the arrays of independent nodes
{
    std::vector<std::vector<double>> res = {};

    for (int i = 0; i < doc->root->children.size(); i++)//iterates and inteprets all children of the children of the document root
    {
        InterpretChild(doc->root->children[i]);
        InterpretTagOntoArray(doc->root->children[i]);
    }
    for (int i = 0; i < doc->root->children.size(); i++)//puts the results in vector
    {
        res.push_back(doc->root->children[i]->array);
    }

    return res;//returns the vector so it can be put in a file
}
//-And driver function for making output file with the result-//
void InterpretToFile(std::string InputFile, std::string OutputFile)//that puts all vector from the function above to a new file with the given name
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
    void free();//gets rid of the whole tree
}
//Helper functions for printing vector<double> and vector<vector<double>>//
void print(std::vector<double> const& input)//that was for debugging
{
    for (int i = 0; i < input.size(); i++)
    {
        std::cout << input.at(i) << ' ';
    }
}
void Print(std::vector<std::vector<double>> const& input)//that was for debugging too
{
    for (int i = 0; i < input.size(); i++)
    {
        print(input[i]);
        std::cout << std::endl;
    }
}
