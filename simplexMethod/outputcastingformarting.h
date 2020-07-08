#ifndef OUTPUTCASTINGFORMARTING_H
#define OUTPUTCASTINGFORMARTING_H

#include "globals.h"
#include "simplextableau.h"
#include "util.h"
#include <QTableWidget>
#include <sstream>
#include <stdexcept>
#include <iomanip>


void marshal(const LPModel &lpm, AugmentedLPModel &alpm);

void marshal(const AugmentedLPModel &alpm, SimplexTableau &st);


string varToStr(Var v, int j);

string numToStr(double d);


void marshal(SimplexTableau &st, QTableWidget *table);

void marshal(Matrix &m, const vector<vector<double>> &vvd);


vector<string> C_TableauRow(vector<double> arg_C);

vector<string> labels_TableauRow(vector<Var> vars);

vector<string> Ai_TableauRow(vector<double> arg_Ai);

vector<string> Z_TableauRow(vector<double> arg_Z);

vector<string> C_Z_TableauRow(vector<double> arg_C_Z, MinMax maxmin);


QString outVarStr(SimplexTableau st);

QString inVarStr(SimplexTableau st);

double pivotNumber(SimplexTableau st);


double toDouble(QString val);

vector<vector<string>> str(vector<vector<double>> mat);

QTableWidget *toTable(vector<vector<string>> strMat);


vector<string> varsTostr(vector<Var> vars);

#endif // OUTPUTCASTINGFORMARTING_H
