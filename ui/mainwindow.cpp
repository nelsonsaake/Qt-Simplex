#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QGraphicsDropShadowEffect>
#include <QHeaderView>
#include <QMessageBox>
#include <simplexMethod/globals.h>
#include <simplexMethod/simplextableau.h>
#include <stdexcept>
#include "simplexMethod/outputcastingformarting.h"
#include "simplexMethod/util.h"
#include "messagedialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::gotopage(MainWindow::Page page)
{
    ui->tabWidget->setCurrentIndex(page);
}

void MainWindow::addTable(QTableWidget *table, int row, int col)
{
    if(table==nullptr) return;

    if(row>0 && row<table->columnCount()
            &&
            col>0 && row<table->rowCount()){

        QTableWidgetItem *item = new QTableWidgetItem();

        QString text = table->item(row,col)->text();
        item->setText(text);

        QColor color(100,95,95);
        item->setBackground(QBrush(Qt::red));

        table->setItem(row,col,item);
    }

    adjustTableau(table);
    addOutputWidget(table);
}

void MainWindow::addTableauLabel(QString msg)
{
    addOutputWidget(new QLabel(msg));
}

void MainWindow::addOutputWidget(QWidget *w)
{
    ui->outputarea->addWidget(w);
    outputWidgets.push_back(w);
}


LineEdit *newDoubleSpinBox(){
    LineEdit *input = new LineEdit;
    return input;
}

void MainWindow::prepCjInput(int colCount)
{
    clearCjVals();
    clearCjLabels();
    
    QHBoxLayout *cjvalsarea = ui->horizontalLayout_4;
    
    for(int j=0; j<colCount; j++){
        LineEdit *cjVal = newDoubleSpinBox(); // create label
        C.push_back(cjVal); // save label  pointer
        cjvalsarea->addWidget(cjVal); // add label to ui

        QLabel *varLabel = new QLabel(objVar(j));// create label
        CLables.push_back(varLabel); // save label pointer
        cjvalsarea->addWidget(varLabel); // add label to ui
    }
}

void MainWindow::prepContraintsInput(int imax, int jmax)
{
    clearConstraintsArea();
    
    if(imax==0||jmax==0) return;
    
    for(int i=0; i<imax; i++){
        
        QHBoxLayout *row = new QHBoxLayout;
        vector<LineEdit*> constraint_i;
        
        for(int j=0; j<jmax; j++){
            LineEdit *aij = newDoubleSpinBox();
            constraint_i.push_back(aij);
            row->addWidget(aij);
            
            addALabel(row, j);
        }
        
        A.push_back(constraint_i);
        ui->constraintsLayout->addLayout(row);
        
        addRelationOperators(row);
        
        addBi(row);
    }
}

void MainWindow::addRelationOperators(QHBoxLayout *hl)
{
    if(hl == nullptr) return;
    
    QComboBox *cb = new QComboBox(this);
    cb->addItems({LessEqualStr,EqualStr,GreaterEqualStr});
    hl->addWidget(cb);
    relationalOPerators.push_back(cb);
}

void MainWindow::addBi(QHBoxLayout *hl)
{
    if(hl == nullptr) return;
    
    LineEdit *bi = newDoubleSpinBox();
    B.push_back(bi);
    hl->addWidget(bi);
}

void MainWindow::addALabel(QHBoxLayout *hl, int j)
{
    QLabel *varLabel = new QLabel(objVar(j));// create label
    ALabels.push_back(varLabel);
    hl->addWidget(varLabel);
}


void MainWindow::clearCjLabels()
{
    for(int i=0; i<CLables.size(); i++){
        QLabel *label = CLables[i];
        label->deleteLater();
    }
    CLables.clear();
}

void MainWindow::clearCjVals()
{
    for(int i=0; i<C.size(); i++){
        LineEdit *lineEdit = C[i];
        lineEdit->deleteLater();
    }
    C.clear();
}


void MainWindow::clearALabels()
{
    for(int i=0; i<ALabels.size(); i++){
        QLabel *label = ALabels[i];
        label->deleteLater();
    }
    ALabels.clear();
}

void MainWindow::clearA()
{
    for(int i=0; i<A.size(); i++)
    {
        vector<LineEdit*> row=A[i];
        for(int j=0; j<row.size(); j++){
            LineEdit *lineEdit = row[j];
            lineEdit->deleteLater();
        }
        row.clear();
    }
    A.clear();
}

void MainWindow::clearB()
{
    for(int i=0; i<B.size(); i++){
        LineEdit *lineEdit = B[i];
        lineEdit->deleteLater();
    }
    B.clear();
}

void MainWindow::clearRelationalOperators()
{
    for(int i=0; i<relationalOPerators.size(); i++){
        QComboBox *cb = relationalOPerators[i];
        cb->deleteLater();
    }
    relationalOPerators.clear();
}

void MainWindow::clearConstraintsArea()
{
    clearA();
    clearALabels();
    clearB();
    clearRelationalOperators();
}

void MainWindow::clearOldSolution()
{
    for(int i=0; i<outputWidgets.size(); i++){
        QWidget *t = outputWidgets[i];
        t->deleteLater();
    }
    outputWidgets.clear();
}


vector<vector<double>> MainWindow::getA()
{
    vector<vector<double>> res;
    
    for(int i=0; i<A.size(); i++){
        vector<double> Ai;

        for(int j=0; j<A[i].size(); j++){
            LineEdit *aij = A[i][j];
            double d = aij->text().toDouble();
            Ai.push_back(d);
        }
        res.push_back(Ai);
    }

    return res;
}

vector<double> MainWindow::getB()
{
    vector<double> res;
    
    for(int i=0; i<B.size(); i++){
        LineEdit *bi = B[i];
        double d = bi->text().toDouble();
        res.push_back(d);
    }
    
    return res;
}

vector<double> MainWindow::getC()
{
    vector<double> res;
    
    for(int j=0; j<C.size(); j++){
        LineEdit *cj = C[j];
        double d = cj->text().toDouble();
        res.push_back(d);
    }
    
    return res;
}

vector<Convension> MainWindow::getRelationalOperators()
{
    vector<Convension> res;

    for(QComboBox *cb: relationalOPerators){
        QString sel = cb->currentText();

        if(sel == EqualStr) res.push_back(Equal);
        if(sel == LessEqualStr) res.push_back(LessEqual);
        if(sel == GreaterEqualStr) res.push_back(GreaterEqual);
    }

    return res;
}

MinMax MainWindow::getObjective()
{
    return (ui->max->isChecked())? Maximisation: Minimisation;
}

void MainWindow::inheritSteps(vector<SimplexTableau> steps)
{
    this->steps = steps;
}


void MainWindow::displaySolution()
{
    clearOldSolution();

    for(int i=0; i<steps.size(); i++) {

        // add to ui
        QString stepiSummary =
                (!(i==steps.size()-1) || steps[i].isUnbounded())?
                    stepSummary(i, steps[i], false):
                    stepSummary(i, steps[i], true);

        addTableauLabel(stepiSummary);

        // make table
        QTableWidget *table = new QTableWidget();

        // fill
        SimplexTableau st=steps[i];
        marshal(st, table);

        // add to ui
        int row = st.pivotRowId+2; // 2 = offset caused by C and labels
        int col = st.pivotColId+3; // 3, offset cased by cb, basis, bi
        addTable(table, row, col);
    }
    comments();
}

void MainWindow::comments()
{
    if(steps.empty()) return;

    SimplexTableau lastTableau = steps[steps.size()-1];

    if(lastTableau.isUnbounded() ||
       lastTableau.isInfeasible())
    {
        displayComplications(lastTableau);
    }
    else displaySummary(lastTableau);
}

void MainWindow::displayComplications(SimplexTableau lastTableau)
{
    QString conclusion = complications(lastTableau);
    ui->inputLabel_6_comments->setText(conclusion);
}

void MainWindow::displaySummary(SimplexTableau lt)
{
    ui->inputLabel_6_comments->setText("Optimal solution");

    QTableWidget *lt_summary = summary(lt);

    lt_summary->adjustSize();

    adjustTableau(lt_summary);

    outputWidgets.push_back(lt_summary);

    ui->summry->addWidget(lt_summary);
}

bool MainWindow::noInput()
{
    if(C.size()<=0) return true;

    if(B.size()<=0) return true;

    if(A.size()<=0) return true;

    return false;
}

void MainWindow::showMsg(QString title, QString msg)
{
    MessageDialog *msgdlg = new MessageDialog(this);
    msgdlg->setMessage(title,msg);
    msgdlg->show();
}


void MainWindow::on_j_editingFinished()
{
    try{
        QString strval = ui->j->text();
        int val=0;
        bool *ok = new bool(false);
        val = strval.toInt(ok);

        prepCjInput(val);
        on_i_editingFinished();
    }catch(...){}
}

void MainWindow::on_i_editingFinished()
{
    QString strval = ui->i->text();
    int i=0;
    bool *ok = new bool(false);
    i = strval.toInt(ok);
    
    strval = ui->j->text();
    int j=0;
    *ok = new bool(false);
    j = strval.toInt(ok);
    
    prepContraintsInput(i,j);
}

void MainWindow::on_pushButton_clicked()
{
    try{
        if(noInput())return;

        LPModel lpm;
        marshal(lpm.A,getA());
        lpm.B = getB();
        lpm.C = getC();
        lpm.relationalOperators = getRelationalOperators();
        lpm.objective = getObjective();
        steps = solve(lpm);

        displaySolution();
        gotopage(Page::Output);
    }catch(...){}
}

void MainWindow::on_gotoinput_pushButton_clicked()
{
    gotopage(Page::Input);
}





LineEdit::LineEdit()
{
    setup();
}

void LineEdit::setup()
{
    lastValidText = "";

    setText("0");
    setObjectName("btn");
    connect(this,SIGNAL(editingFinished()),this,SLOT(editingFinishedSlot()));
    connect(this,SIGNAL(textEdited(QString)),this,SLOT(editingFinishedSlot()));
}

void LineEdit::error()
{
    QString msg{"Input is not a number, provide a number"};
    MessageDialog *msgdlg = new MessageDialog(this);
    msgdlg->setMessage("Error",msg);
    msgdlg->show();
}

void LineEdit::editingFinishedSlot()
{
    QString str = text();

    if(str=="-") return;
    if(str.isEmpty()) setText("0");

    bool *ok = new bool(false);
    str.toDouble(ok);

    QString style;
    if(!*ok){
        setText(lastValidText);
    }else{
        lastValidText = str;
    }

    setStyleSheet(style);
}
