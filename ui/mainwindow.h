#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QTableWidget>
#include <vector>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <simplexMethod/globals.h>
#include "simplexMethod/simplextableau.h"

using namespace std;

class LineEdit:public QLineEdit{
    Q_OBJECT

public:
    LineEdit();
    void setup();
    void error();

private slots:
    void editingFinishedSlot();

private:
    QString lastValidText;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum Page{Home, Input, Output, About, Tutorials, Help};

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // help go from one tab to the other
    void gotopage(MainWindow::Page page);


    void addTable(QTableWidget *table, int row=-1, int col=-1);
    void addTableauLabel(QString msg);
    void addOutputWidget(QWidget *w);

    void prepCjInput(int count);

    void prepContraintsInput(int i, int j);
    void addRelationOperators(QHBoxLayout *hl);
    void addBi(QHBoxLayout *hl);
    void addALabel(QHBoxLayout *hl, int j);

    void clearCjLabels();
    void clearCjVals();
    void clearALabels();
    void clearA();
    void clearB();
    void clearRelationalOperators();
    void clearConstraintsArea();
    void clearOldSolution();

    vector<vector<double>> getA();
    vector<double> getB();
    vector<double> getC();
    vector<Convension> getRelationalOperators();
    MinMax getObjective();

    void displaySolution();

    void inheritSteps(vector<SimplexTableau> steps);

    void comments();
    void displayComplications(SimplexTableau lastTableau);
    void displaySummary(SimplexTableau lastTableau);

    void goToOuputPage();
    void goToHomePage();

    bool noInput();
    void showMsg(QString title, QString msg);

private slots:
    void on_j_editingFinished();
    void on_i_editingFinished();

    void on_pushButton_clicked();
    void on_gotoinput_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    vector<LineEdit*> C;
    vector<QLabel*> CLables;

    vector<vector<LineEdit*>> A;
    vector<QLabel*> ALabels;

    vector<LineEdit*> B;
    vector<QComboBox*> relationalOPerators;

    vector<QWidget*> outputWidgets;
    vector<SimplexTableau> steps;
};

#endif // MAINWINDOW_H
