#include <iostream> 
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <list> 
#include <vector>
#include <string>
#include <iterator>


struct IMLNode
{
public:
    std::string tag;
    std::vector<double> array;
    IMLNode* parent;
    double attributed;
    std::string attributes;
    std::vector<IMLNode*> children;
    IMLNode(std::string tag)
    {
        this->parent = nullptr;
        this->tag = tag;
        this->attributed;
        this->attributes = "";
        this->array = {};
        this->children = {};
    }
    IMLNode(IMLNode* parent)
    {
        this->parent = parent;
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
struct IMLDocument
{
    IMLNode* root;
};

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

int Found(std::string searched)
{
    std::vector<std::string> TagsWithAtributeD = { "MAP-INC", "MAP-MLT", "SRT-SLC" };
    std::vector<std::string> TagsWithoutAtribute = { "AGG-SUM", "AGG-PRO", "AGG-AVG",
                                                    "AGG-FST", "AGG-LST", "SRT-REV" };
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

std::string ParseFileToString(std::string path)
{
    std::ifstream file;
    file.open(path); //open the input file

    std::stringstream sstream;
    sstream << file.rdbuf(); //read the file
    std::string buf = sstream.str(); //buf holds the content of the file
    return buf;
}

int ParseIMLDocument(IMLDocument* doc, std::string path)
{
    std::string buf = ParseFileToString(path);

    std::string tag;
    std::string ClosingTag;
    std::string array;
    std::string attribute;
    std::stack<std::string> tags;
    int i = 0;// buffer iterator
    //std::string LexCopy;
    bool InTag = true;
    std::string root = "root";
    doc->root = new IMLNode(root);
    IMLNode* CurrentNode = doc->root;

    while (buf[i] != buf[buf.size()])
    {
        if (!InTag && !array.empty() && (buf[i] == '<' || buf[i] == '>'))
        {
            std::cout << array << std::endl;
            std::vector<double> ar = ConvertStringtoDoubleVector(array);
            for (int i = 0; i < ar.size(); i++)
            {
                CurrentNode->array.push_back(ar[i]);
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
            //std::cout << tag << std::endl;
            if (Found(tag) == 0)
            {
                std::cout << "Tag not supported!!!";
                throw "Tag not supported!";
            }
            tags.push(tag);
            CurrentNode->tag = tag;
            std::cout << CurrentNode->tag << std::endl;
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
