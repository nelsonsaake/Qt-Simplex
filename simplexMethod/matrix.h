#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

using namespace std;

struct Matrix: vector<vector<double>>
{
public:
    Matrix();
    Matrix(int row, int col);
    vector<double>& row(int i);
    vector<double> col(int j);
    vector<double> lastRow();
    vector<double> lastCol();
    void appendRow();
    void appendCol();
    void appendCol(vector<double> lc);
    void removeRow(int i);
    void removeCol(int i);
    int rowCount() const;
    int colCount() const;
    double cell(int i, int j) const;


    void colSwap(int p1, int p2);
};

#endif // MATRIX_H
