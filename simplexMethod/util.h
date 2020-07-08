#ifndef UTIL_H
#define UTIL_H

#include "globals.h"
#include "lpmodel.h"

double val(vector<vector<double>> vvd, int i, int j);

vector<double> range(vector<double> &vd, int begin, int end);

vector<double> stripCol(vector<vector<double>> &vvd, int i);

vector<double> stripRow(vector<vector<double>> &vvd);

double sum(const vector<double> &vd);

vector<double> divide(vector<double> num, vector<double> deno);

int minNonNegativeIndex(vector<double> num);

vector<double> scalar(double k, vector<double> a);

vector<double> dotProduct(vector<double> a, vector<double> b);

vector<double> add(vector<double>a, double b);

vector<vector<double>> transpose(vector<vector<double>> mat);

int maxColCount(vector<vector<string>> mat);

int lastColIndex(vector<vector<double>> &vvd);

void appendCol(vector<vector<double>> &vvd);


#endif // UTIL_H
