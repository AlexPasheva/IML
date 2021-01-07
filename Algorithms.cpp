#include "IML.hpp"

bool IsIntAndPositive(double num)
{
    return num == abs(static_cast<int>(num));
}
std::vector<double> MAPINC(std::vector<double>& data, double inc)
{
    for (size_t i = 0; i < data.size(); i++)
        data[i]+=inc;

    return data;
}
std::vector<double> MAPMLT(std::vector<double>& data, double mlt)
{
    for (size_t i = 0; i < data.size(); i++)
        data[i]*=mlt;
        
    return data;
}
double AGGSUM(std::vector<double>& data)
{
    double res=0; 
    for (size_t i = 0; i < data.size(); i++)
        res += data[i];

    return res;
}
double AGGPRO(std::vector<double>& data)
{
    double res=1; 
    for (size_t i = 0; i < data.size(); i++)
        res *= data[i];

    return res;
}
double AGGAVG(std::vector<double>& data)
{
    double res=1;
    size_t i = 0; 
    for (i ; i < data.size(); i++)
        res *= data[i];

    return res;
}
double AGGAVG(std::vector<double>& data)
{
    double res=1;
    size_t i = 0; 
    for (i ; i < data.size(); i++)
        res *= data[i];

    return res;
}
double AGGFST(std::vector<double>& data)
{
    return data[0];
}
double AGGFST(std::vector<double>& data)
{
    return data[data.size()];
}

std::vector<double> QuickSort(std::vector<double>& vec1){

    double i = 0;
    double j = vec1.size()-2;
    double tmp;
    int pivotindex = vec1.size()-1 ;
    double pivot = vec1[pivotindex];

    if ( vec1.size()<=1 )
        return vec1 ;

    while (i <= j) {
        while (vec1[i] < pivot) {
            i++;
        }
        while (vec1[j] > pivot)
            j--;
         if (i <= j) {
            tmp = vec1[i];
            vec1[i] = vec1[j];
            vec1[j] = tmp;
            i++;
            j--;
        }
    }
    // pivot change
    vec1[pivotindex] = vec1[i] ;
    vec1[i]=pivot ;
    pivotindex=i ;

    if (vec1.size()<=2 )
        return vec1 ;
    // partition
    std::vector<double> left_vec, right_vec ;
    std::vector<double>::iterator pivotiter = vec1.begin()+pivotindex ;
    copy(vec1.begin(), pivotiter, back_inserter(left_vec)) ;
    copy(pivotiter+1, vec1.end(), back_inserter(right_vec)) ;

   if (left_vec.size()>0 ) {
        QuickSort(left_vec);
        copy(left_vec.begin(), left_vec.end(), vec1.begin()) ;
    }
   if (right_vec.size()>0 ) {
        QuickSort(right_vec);
        copy(right_vec.begin(), right_vec.end(), pivotiter+1) ;
    }
    return vec1;
}
std::vector<double> SRTREV(std::vector<double>& data)
{
    return reverse(data.begin(), data.end());
}
std::vector<double> SRTORD (std::vector<double>& data, std::string att)
{
    if (att=="DSC")
    {
        std::vector<double>=QuickSort(data);
        return reverse(a.begin(), a.end());
    }
    else if (att=="ASC")
        return QuickSort(data);
    else
        throw "Error with sorting attribute.";   
}
std::vector<double> SRTSLC(std::vector<double>& data, double inc)
{
    if (!IsIntAndPositive(inc))
    {
        throw "The attribute must be a positive whole number.";
    }
    if (inc >= data.size())
    {
        throw "Attribute out of array size.";
    }    
}