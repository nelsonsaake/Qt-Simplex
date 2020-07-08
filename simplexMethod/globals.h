#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <QtCore>
#include <QTableWidget>

using namespace std;

extern const string tab;

// -1 => less than or equal
// 1 => greater than or equal
// 0 => equal to
enum Convension {LessEqual=-1, Equal=0, GreaterEqual=1};

// maximisation => 2
// minimisation => -2
enum MinMax {Minimisation=-2, Maximisation=2};

enum Var{DecisionVar, SlackVar, ArtificialVar};

extern const double  M;

extern const char* LessEqualStr;

extern const char* GreaterEqualStr;

extern const char* EqualStr;


void resizeTableRows(QTableWidget *table);

void hideTableHeaders(QTableWidget *table);

void removeTableGrid(QTableWidget *table);

void adjustTableau(QTableWidget *table);

void addShadow(QWidget *w);

QString objVar(int j);

void error(string msg);

#endif // GLOBALS_H
