#ifndef LPMODEL_H
#define LPMODEL_H

#include "matrix.h"
#include "globals.h"

using namespace std;

struct LPModel{
    // cj
    vector<double> C;

    // constraints coefficients
    Matrix A;

    // bi
    vector<double> B;

    // min max
    MinMax objective;

    // ≤, =, ≥
    vector<Convension> relationalOperators;
};

#endif // LPMODEL_H
