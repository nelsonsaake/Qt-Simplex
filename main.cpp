#include "ui/mainwindow.h"
#include <QApplication>
#include "simplexMethod/outputcastingformarting.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //
    LPModel lpm;
    lpm.objective = Maximisation;
    lpm.C = {5,4};
    lpm.B = {8,3,27};
    lpm.relationalOperators = {GreaterEqual, Equal, LessEqual};
    marshal(lpm.A,{{2,2},{0,1},{9,3}});

    //
    w.inheritSteps(solve(lpm));
    w.displaySolution();

    return a.exec();
}

/*
add tutorial, user video tutorials

add information on the about tab

add help

add icon
*/
