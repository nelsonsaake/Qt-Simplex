#include "util.h"
#include "matrix.h"
#include "outputcastingformarting.h"

double val(vector<vector<double>> vvd, int i, int j)
{
    if(i >= vvd.size()) // i is too big, row does not exist
        throw(runtime_error("Error trying to refer ot i, a row that does not exist! i is too big."));

    if(i <0) // i is too small, row does not exist
        throw(runtime_error("Error trying to refer ot i, a row that does not exist! i is smaller than zero."));

    if(j <0) // j is too small, row does not exist
        throw(runtime_error("Error trying to refer ot j, an element that does not exist! j is smaller than zero."));

    if(j >= vvd[i].size()) // j is too big, row does not exist
        throw(runtime_error("Error trying to refer ot j, an element that does not exist! j is too big."));

    return vvd[i][j];
}

vector<double> range(vector<double> &vd, int begin, int end)
{
    if(begin<0)
        throw(runtime_error("Error trying to get a range of values: begin is less than 0."));

    if(end>vd.size())
        throw(runtime_error("Error trying to get a range of values: end value is too big."));

    // how many element fall in this range
    // (begin - end) => n[begin, end) or n(begin, end]
    // we want n[begin,end] hence we have to add 1

    vector<double> res;
    for(int i=begin; i<=end; i++)
        res.push_back(vd[i]);

    // return that vector
    return res;
}

vector<double> stripCol(vector<vector<double>> &vvd, int i)
{
    // we do the checks later
    // go through the vvd

    vector<double> col;
    for(vector<double> &vd: vvd)
    {
        // for each row

        if(i<0)
            throw(runtime_error("Error getting column value: index is less than 0."));

        if(i>vd.size())
            throw(runtime_error("Error getting column value: index is larger the last index."));

        // get the val
        col.push_back(vd[i]);

        // erase that val
        vd.erase(vd.begin()+i);
    }

    return col;
}

vector<double> stripRow(vector<vector<double>> &vvd)
{
    if(!(vvd.size() > 0) )
        throw(runtime_error("Error trying to read objective function: vvd, vector is empty."));

    vector<double> vd;
    vd = vvd[0];
    vvd.erase(vvd.begin());
    // if we don't have more than one element
    // there is a problem
    // the first element is for max|min
    // the rest is for the objective function
    // so we need more than one element
    if(!(vd.size() > 1))
        throw(runtime_error("Error trying to read objective function: vd, does not have objective function."));

    // vd.size()-1 is index of last element
    // 1 is the index of the second element
    // the first element is part of the function
    vd = range(vd,1,vd.size()-1);

    return vd;
}

int lastColIndex(vector<vector<double>> &vvd)
{
    return vvd[0].size()-1;
}

void appendCol(vector<vector<double>> &vvd)
{
    for(vector<double> &vd: vvd)
    {
        vd.push_back(0);
    }
}

double sum(const vector<double> &vd)
{
    double s = 0;
    for(const double &d : vd)
        s += d;

    return s;
}

vector<double> divide(vector<double> num, vector<double> deno)
{
    // divide corresponding elements in num and deno
    // num is the numerator

    if(num.size() != deno.size())
        throw(runtime_error("Error division of two vectors: the number of elements are not the same."));

    vector<double> res;
    for(int i=0; i<num.size(); i++){
        if(deno[i] == 0) {
            res.push_back(999999999999999999999);
            continue;
        }

        res.push_back(num[i]/deno[i]);
    }

    return res;
}

int minNonNegativeIndex(vector<double> num)
{
    // return the index of the smallest value
    // if it is empty, -1

    int minIndex=-1;
    double minval=0;
    for(int i=0; i<num.size(); i++){
        if(num[i]<0) continue;

        if(minIndex<0 || minval>num[i])
        {
            minval = num[i];
            minIndex = i;
        }
    }
    return minIndex;
}

vector<double> scalar(double k, vector<double> a)
{
    // multiply k with each element and return the result

    // returned vector
    vector<double> res;

    // for each element
    // multiply with k and stor in returned vector
    for(double d:a)
        res.push_back(d*k);

    return res;
}

vector<double> dotProduct(vector<double> a, vector<double> b)
{
    // multiply element at corresponding positions

    if(a.size() != b.size())
        throw(runtime_error("Error dotProduct: a and b are of different length"));

    vector<double> res;

    // a has the same number of elements as b
    for(int i=0; i<a.size(); i++)
    {
        res.push_back(a[i]*b[i]);
    }

    return res;
}

vector<double> add(vector<double> a, double b)
{
    for(double &d: a)
        d+=b;

    return a;
}

vector<vector<double>> transpose(vector<vector<double>> mat)
{
    vector<vector<double>> transpose;
    Matrix matMat;
    marshal(matMat, mat);

    for(int i=0; i<mat[0].size(); i++){
        transpose.push_back(matMat.col(i));
    }

    return transpose;
}

int maxColCount(vector<vector<string>> mat)
{
    int size = 0;
    for(vector<string> row: mat){
        if(size < row.size())
           size = row.size();
    }

    return size;
}




