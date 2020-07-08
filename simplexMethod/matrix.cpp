#include "matrix.h"
#include "stdexcept"
#include "globals.h"

Matrix::Matrix()
    : vector<vector<double>> (0,vector<double>(0, 0))
{

}

Matrix::Matrix(int row, int col)
    : vector<vector<double>> (row, vector<double>(col, 0))
{

}

vector<double> &Matrix::row(int i)
{
    // return the ith row
    return operator[](i);
}

vector<double> Matrix::col(int j)
{
    // return a particular row
    // we want to remove a col from the matrix
    // we move from row to row delete element at a particular position

    vector<double> col;

    for(int i=0; i<size(); i++)
    {
        // for each row
        // get the element at that index

            if(j >= at(i).size()) // i is too big, row does not exist
                throw(runtime_error("Error trying to refer row element, row does not exist! j is too big."));

            if(j <0) // i is too small, row does not exist
                throw(runtime_error("Error trying to refer row element, row does not exist! j is less than zero."));

            col.push_back(at(i).at(j));
    }

    return col;
}

vector<double> Matrix::lastRow()
{
    // now we have row() we can just specify the last row index

    if(empty()) return vector<double>();

    return row(size()-1);
}

vector<double> Matrix::lastCol()
{
    // now we have col() we can just specify the last col index

    if(empty()) return vector<double>();

    return col(at(0).size()-1);
}

void Matrix::appendRow()
{
    // we want to add a new row
    // we want this row to have the same number of element as those before it
    // we assume all row have the same number of element
    // if this is the first we add an empty row

    int len = 0;

    // length of the first row
    if(!empty())
        int len = size();

    // add a new row
    push_back(vector<double>(len,0));
}

void Matrix::appendCol()
{
    // move from row to row and add one new element to each row

    // just to make it more readable
    for(int i=0; i<size(); i++)
        at(i).push_back(0);
}

void Matrix::appendCol(vector<double> lc)
{
    if(lc.size() != rowCount()) error("Error appending col, new col has different length.");
    
    for(int i=0; i<rowCount(); i++){
        at(i).push_back(lc[i]);
    }
}

void Matrix::removeRow(int i)
{
    // we want to remove a row from the matrix

    if(i >= size()) // i is too big, row does not exist
        throw(runtime_error("Error trying to delete row, row does not exist! i is too big."));

    if(i <0) // i is too small, row does not exist
        throw(runtime_error("Error trying to delete row, row does not exist! i is less than zero."));

    erase(begin()+i);
}

void Matrix::removeCol(int j)
{
    // we want to remove a col from the matrix
    // we move from row to row delete element at a particular position

    for(int i=0; i<size(); i++)
    {
        // for each row

        // remove a particular row element

        if(j >= at(i).size()) // i is too big, row does not exist
            throw(runtime_error("Error trying to delete row element, row does not exist! j is too big."));

        if(j <0) // i is too small, row does not exist
            throw(runtime_error("Error trying to delete row element, row does not exist! j is less than zero."));

         at(i).erase( at(i).begin()+j);
    }
}

int Matrix::rowCount() const
{
    return size();
}

int Matrix::colCount() const
{
    int count = 0;

    if(rowCount() > 0)
        count = at(0).size();

    return count;
}

double Matrix::cell(int i, int j) const
{
    if(i >= size()) // i is too big, row does not exist
        throw(runtime_error("Error trying to refer ot i, a row that does not exist! i is too big."));

    if(i <0) // i is too small, row does not exist
        throw(runtime_error("Error trying to refer ot i, a row that does not exist! i is smaller than zero."));

    if(j <0) // j is too small, row does not exist
        throw(runtime_error("Error trying to refer ot j, an element that does not exist! j is smaller than zero."));

    if(j >= at(i).size()) // j is too big, row does not exist
        throw(runtime_error("Error trying to refer ot j, an element that does not exist! j is too big."));

    return at(i).at(j);
}

void Matrix::colSwap(int p1, int p2)
{
    if(p1<0 || p2<0 || p1>colCount() || p2>colCount())  throw(runtime_error("Error swaping columns"));

    for(int i=0; i<rowCount(); i++){
        std::swap(at(i).at(p1),at(i).at(p2));
    }
}
