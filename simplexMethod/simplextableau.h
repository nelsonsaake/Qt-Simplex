#ifndef SIMPLEXTABLEAU_H
#define SIMPLEXTABLEAU_H

#include "augmentedlpmodel.h"
#include "globals.h"
#include "matrix.h"

struct SimplexTableau:AugmentedLPModel
{
    //
    SimplexTableau();
    SimplexTableau(AugmentedLPModel alpm);

    Var basisVar(int i);
    int colCount() const;
    vector<double> cb();
    vector<double> pivotCol();
    vector<double> pivotRow();

    double calZj(const vector<double> &basisVar, const vector<double> &colj);
    void calZ();
    void calDeltaCZ();
    void inVar();
    void outVar();
    vector<int> newBasis();
    void calNewPivotRow();
    vector<double> newOtherRow(int rowIndex);
    Matrix newA();
    vector<double> newBiValues();

    SimplexTableau nextTableau();

    bool isToStop();
    bool isInfeasible();
    bool isUnbounded();
    bool isSolution();

    vector<vector<string>> stToStrMat();

    vector<double> newPivotRow;
    vector<double> Z;
    vector<double> deltaCZ;
    int pivotColId;
    int pivotRowId;
    double pivotNumber;
    QString statement;
};


QString stepSummary(int i, SimplexTableau step, bool makeShort);

QTableWidget *summary(SimplexTableau lt);

QString complications(SimplexTableau t);

vector<SimplexTableau> solve(LPModel lpm);

SimplexTableau solve_finalTableau(LPModel lpm);
#endif // SIMPLEXTABLEAU_H
