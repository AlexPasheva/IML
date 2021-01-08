#pragma once
#include <unordered_set>
#include <algorithm>
bool IsIntAndPositive(double num)
{
    return num == abs(static_cast<int>(num));
}
//------------------Map------------------//
void MAPINC(std::vector<double>& data, double inc)
{
    for (size_t i = 0; i < data.size(); i++)
        data[i] += inc;
}
void MAPMLT(std::vector<double>& data, double mlt)
{
    for (size_t i = 0; i < data.size(); i++)
        data[i] *= mlt;
}
//---------------Aggregate---------------//
void AGGSUM(std::vector<double>& data)
{
    double res = 0;
    for (size_t i = 0; i < data.size(); i++)
        res += data[i];
    data = {};
    data.push_back(res);
}
void AGGPRO(std::vector<double>& data)
{
    double res = 1;
    for (size_t i = 0; i < data.size(); i++)
        res *= data[i];
    data = {};
    data.push_back(res);
}
void AGGAVG(std::vector<double>& data)
{
    double res = 0;
    for (size_t i = 0; i < data.size(); i++)
        res += data[i];
    res /= data.size();
    data = {};
    data.push_back(res);
}
void AGGFST(std::vector<double>& data)
{
    double res = data[0];
    data = {};
    data.push_back(res);
}
void AGGLST(std::vector<double>& data)
{
    double res = data[data.size()];
    data = {};
    data.push_back(res);
}
//----------------Sorting----------------//
void SRTREV(std::vector<double>& data)
{
    std::reverse(std::begin(data), std::end(data));
}
void SRTORD(std::vector<double>& data, std::string att)
{
    if (att == "DSC")
    {
        sort(data.begin(), data.end());
        std::reverse(begin(data), end(data));
    }
    else if (att == "ASC")
        sort(data.begin(), data.end());
    else
    {
        std::cout << "The attribute must be a positive whole number.";
        throw "Error with sorting attribute.";
    }
}
void SRTSLC(std::vector<double>& data, double inc)
{
    if (!IsIntAndPositive(inc))
    {
        std::cout << "The attribute must be a positive whole number.";
        throw "The attribute must be a positive whole number.";
    }
    if (inc > data.size())
    {
        std::cout<< "Attribute out of array size.";
        throw "Attribute out of array size.";
    }
    std::reverse(begin(data), end(data));
    for (size_t i = 0; i < inc; i++)
    {
        data.pop_back();
    }
    std::reverse(begin(data), end(data));
}
void SRTDST(std::vector<double>& data)
{
    std::unordered_set<double> SeenNums; //log(n) existence check

    auto itr = begin(data);
    while (itr != end(data))
    {
        if (SeenNums.find(*itr) != end(SeenNums)) //seen? erase it
            itr = data.erase(itr); //itr now points to next element
        else
        {
            SeenNums.insert(*itr);
            itr++;
        }
    }
}