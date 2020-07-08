#include "globals.h"
#include <QGraphicsDropShadowEffect>
#include <QHeaderView>
#include <sstream>

// convensions
const string tab {"    "};

const double  M {1000000000};

const char* LessEqualStr {"≤"};

const char* GreaterEqualStr {"≥"};

const char* EqualStr {"="};

void resizeTableRows(QTableWidget *table)
{
    if (table==nullptr) return;

    const int rowHeight = 43;

    for(int i=0; i<table->rowCount(); i++){
        table->setRowHeight(i,rowHeight);
    }
    for(int j=0; j<table->columnCount(); j++){
        table->setColumnWidth(j, 130);
    }

    //table->setFixedWidth(91*table->columnCount());
    table->setFixedHeight((50)*table->rowCount());
}

void hideTableHeaders(QTableWidget *table)
{
    if (table==nullptr) return;

    QHeaderView *horizontalHeader = table->horizontalHeader();
    horizontalHeader = const_cast<QHeaderView*>(horizontalHeader);
    horizontalHeader->setVisible(false);

    QHeaderView *verticalHorizontal = table->verticalHeader();
    const_cast<QHeaderView*>(verticalHorizontal)->setVisible(false);
}

void removeTableGrid(QTableWidget *table)
{
    if (table==nullptr) return;

    table->setShowGrid(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void adjustTableau(QTableWidget *table)
{
    if (table==nullptr) return;

    resizeTableRows(table);
    hideTableHeaders(table);
    removeTableGrid(table);
}

void addShadow(QWidget *w)
{
    // ?
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(w);
}

QString objVar(int j)
{
    return "X<span style=' vertical-align:sub;'>" + QString::number(j+1) + "</span>";
}

void error(string msg)
{
    throw(runtime_error{msg});
}
