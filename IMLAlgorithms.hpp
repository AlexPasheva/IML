#pragma once

bool IsIntAndPositive(double num)
{
    return num == abs(static_cast<int>(num));
}
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
double AGGSUM(std::vector<double>& data)
{
    double res = 0;
    for (size_t i = 0; i < data.size(); i++)
        res += data[i];

    return res;
}
double AGGPRO(std::vector<double>& data)
{
    double res = 1;
    for (size_t i = 0; i < data.size(); i++)
        res *= data[i];

    return res;
}
double AGGAVG(std::vector<double>& data)
{
    return AGGSUM(data) / data.size();
}
double AGGFST(std::vector<double>& data)
{
    return data[0];
}
double AGGLST(std::vector<double>& data)
{
    return data[data.size()];
}

std::vector<double> QuickSort(std::vector<double>& vec1) {

    double i = 0;
    double j = vec1.size() - 2;
    double tmp;
    int pivotindex = vec1.size() - 1;
    double pivot = vec1[pivotindex];

    if (vec1.size() <= 1)
        return vec1;

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
    vec1[pivotindex] = vec1[i];
    vec1[i] = pivot;
    pivotindex = i;

    if (vec1.size() <= 2)
        return vec1;
    // partition
    std::vector<double> left_vec, right_vec;
    std::vector<double>::iterator pivotiter = vec1.begin() + pivotindex;
    copy(vec1.begin(), pivotiter, back_inserter(left_vec));
    copy(pivotiter + 1, vec1.end(), back_inserter(right_vec));

    if (left_vec.size() > 0) {
        QuickSort(left_vec);
        copy(left_vec.begin(), left_vec.end(), vec1.begin());
    }
    if (right_vec.size() > 0) {
        QuickSort(right_vec);
        copy(right_vec.begin(), right_vec.end(), pivotiter + 1);
    }
    return vec1;
}
void SRTREV(std::vector<double>& data)
{
    std::reverse(std::begin(data), std::end(data));
}
void SRTORD(std::vector<double>& data, std::string att)
{
    if (att == "DSC")
    {
        QuickSort(data);
        std::reverse(begin(data), end(data));
    }
    else if (att == "ASC")
        QuickSort(data);
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