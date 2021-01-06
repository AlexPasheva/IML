#include <iostream> 
#include <fstream>
#include <sstream>
#include <algorithm>
#include <list> 
#include <vector>
#include <string>
#include <iterator>
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
struct IMLDocument
{
    IMLNode* root;
};

struct IMLNode
{
    std::string tag;
    std::vector<double> array;
    IMLNode* parent;
    double attributed;
    std::string attributes;
    std::vector<IMLNode*> children;
    IMLNode(std::string tag) : tag(tag) {}
    IMLNode(IMLNode* parent)
    {
        this->parent = parent;
        this->tag = "";
        this->attributed=NULL;
        this->attributes="";
        this->array={};
        this->children={};
        if (parent)
            parent->children.push_back(this);
    }
};

void free(IMLNode* root)
{
    for (int i = 0; i < root->children.size(); i++)
        free(root->children[i]);

    delete root->parent;
    delete root;
}

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

struct Tags
{
    std::vector<std::string> TagsWithAtributeD = {"MAP-INC", "MAP-MLT", "SRT-SLC"};
    std::vector<std::string> TagsWithoutAtribute = {"AGG-SUM", "AGG-PRO", "AGG-AVG",
    "AGG-FST", "AGG-LST", "SRT-REV", "SRT-ORD"};
    std::string TagsWithoutAtributeS = "SRT-ORD";

    int Foundd(std::string searched)
    {
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
};

 int Found(std::string searched)
{
    std::vector<std::string> TagsWithAtributeD = {"MAP-INC", "MAP-MLT", "SRT-SLC"};
    std::vector<std::string> TagsWithoutAtribute = {"AGG-SUM", "AGG-PRO", "AGG-AVG",
                                                    "AGG-FST", "AGG-LST", "SRT-REV", "SRT-ORD"};
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
int IMLDocument_load(IMLDocument* doc, std::string path)
{
    std::ifstream file;
    file.open(path); //open the input file

    std::stringstream sstream;
    sstream << file.rdbuf(); //read the file
    std::string buf = sstream.str(); //buf holds the content of the file

    doc->root = nullptr;

    std::string lex;
    int lexi = 0;// lexer iterator
    int i = 0;// buffer iterator
    //std::string LexCopy;
    IMLNode* CurrentNode = doc->root;

    while (buf[i] != '\0')
    {
        if (buf[i] == '<') {
            lex[lexi] = '\0';

            // Array between opening and closing tag
            if (lexi > 0) {
                if (!CurrentNode) {
                    throw "Text outside of document\n";
                }

                CurrentNode->array = ConvertStringtoDoubleVector(lex);
                lexi = 0;
            }

            // End of node
            if (buf[i + 1] == '/') {
                i += 2;
                while (buf[i] != '>')
                    lex[lexi++] = buf[i++];
                lex[lexi] = '\0';

                if (!CurrentNode) {
                    throw "Already at the root!";
                }

                if (CurrentNode->tag == lex) {
                    throw "Mismatched tags!";
                }

                CurrentNode = CurrentNode->parent;
                i++;
                continue;
            }

            // Set current node
            IMLNode* NewNode(CurrentNode);
            CurrentNode = NewNode;

            // Start tag
            i++;
            std::string CurrentAttribute;
            while (buf[i] != '>') {
                lex[lexi++] = buf[i++];
                // Tag name
                if (buf[i] == ' ' && CurrentNode->tag=="") {
                    lex[lexi] = '\0';
                    CurrentNode->tag = lex;
                    lexi = 0;
                    i++;
                    continue;
                }
                // Usually ignore spaces
                if (lex[lexi - 1] == ' ') {
                    lexi--;
                    continue;
                }

                // Attribute
                if (buf[i] == '"') {
                    lexi = 0;
                    i++;
                    while (buf[i] != '"')
                        lex[lexi++] = buf[i++];
                    lex[lexi] = '\0';
                    CurrentAttribute = lex;

                    if (Found(CurrentAttribute) == 1)
                    {
                        try
                        {
                            CurrentNode->attributed = stod(CurrentAttribute);
                        }
                        catch (...)
                        {
                            throw "Attribute with wrong value.\n";
                        }
                    }
                    else if (Found(CurrentAttribute) == 3)
                    {            
                        if (!(CurrentAttribute=="ASC"||CurrentAttribute=="DSC"))
                        {
                            throw "Attribute with wrong value.\n";
                        }                        
                        CurrentNode->attributes = CurrentAttribute;
                    }
                    else if (Found(CurrentAttribute) == 2 && CurrentAttribute != "")
                    {
                        throw "Added attribute whitout it being needed.\n";
                    }
                    else if (Found(CurrentAttribute) == 2)
                    {
                        CurrentNode->attributes = "";
                        CurrentNode->attributed = NULL;
                    }
                    else if (Found(CurrentAttribute)==0)
                    {
                        throw "Tag not supported.\n";                        
                    }

                    lexi = 0;
                    i++;
                    continue;
                }
            }
            // Set tag name if none... that must be kinda not needed...
            lex[lexi] = '\0';
            if (CurrentNode->tag=="")
                throw "There cannot exist nameless tag";
            // Reset lexer
            lexi = 0;
            i++;
            continue;
        }
        else {
            lex[lexi++] = buf[i++];
        }
    }
}