#include "outputcastingformarting.h"
#include "util.h"

void marshal(const LPModel &lpm, AugmentedLPModel &alpm)
{
    // first we take all data from the lpm
    alpm = AugmentedLPModel(lpm);

    // introducing slack, suplus, and artificial variables
    // enumurate obj vars
    alpm.vars = vector<Var>(lpm.A[0].size(), DecisionVar);

    for(unsigned int i=0; i<lpm.relationalOperators.size(); i++)
    {
        Convension c = lpm.relationalOperators[i];

        switch(c)
        {
        case LessEqual:
            alpm.addSlackVariable(i, 1);
            alpm.Basis.push_back(alpm.vars.size()-1);
            break;

        case Equal:
            alpm.addArtificialVariable(i);
            alpm.Basis.push_back(alpm.vars.size()-1);
            break;

        case GreaterEqual:
            alpm.addSlackVariable(i, -1);
            alpm.addArtificialVariable(i);
            alpm.Basis.push_back(alpm.vars.size()-1);
            break;
        }
    }

    alpm.sort();
}

void marshal(const AugmentedLPModel &alpm, SimplexTableau &st)
{
    st = AugmentedLPModel(alpm);
    st.calZ();
    st.calDeltaCZ();
    st.inVar();
    st.outVar();
}

void marshal(SimplexTableau &st, QTableWidget *tableau)
{
    // null => nothing
    if(tableau == nullptr) tableau = new QTableWidget();

    // convert simplex tableau to a vector of vector of string
    vector<vector<string>> vvs = st.stToStrMat();

    // the simplex tableau has 3 more column than the number of Cj element
    // and 4 more rows than the number of row in the A matrix
    // noticed we are not using the actual vss mxn

    const int col = st.C.size() + 3;
    const int row = st.A.size() + 4;

    tableau->setColumnCount(col);
    tableau->setRowCount(row);

    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            QString val = vvs[i][j].c_str();
            tableau->setItem(i,j,new QTableWidgetItem(val));
        }
    }

    adjustTableau(tableau);
}


void marshal(Matrix &m, const vector<vector<double>> &vvd)
{
    for(vector<double> vd: vvd) m.push_back(vd);
}


string varToStr(Var v, int j)
{
    ostringstream oss;
    switch(v)
    {
    case DecisionVar:
        oss << "X";
        break;

    case SlackVar:
        oss << "S";
        break;

    case ArtificialVar:
        oss << "A";
        break;
    }
    oss /*<< "<span style=' vertical-align:sub;'>"*/ << j /*<< "</span>"*/;
    return oss.str();
}

string numToStr(double d)
{
    if(d==int(d)){
        // full integer digit without e
        // << fixed does it better
        ostringstream oss;
        oss << fixed << int(d);
        return oss.str();
    }

    // fractions without the un-neccessary 0
    // ::number() does it better
    return QString::number(d).toStdString();
}


vector<string> C_TableauRow(vector<double> arg_C)
{

    vector<string>C;
    C.push_back("Cj");
    C.push_back("");
    C.push_back("");

    for(double d : arg_C)
        C.push_back(numToStr(d));

    return C;
}

vector<string> labels_TableauRow(vector<Var> vars)
{
    vector<string>labels;
    labels.push_back("Cb");
    labels.push_back("Basis");
    labels.push_back("Bi");

    for(string str : varsTostr(vars)){
        labels.push_back(str);
    }

    return labels;
}

vector<string> Ai_TableauRow(vector<double> arg_Ai)
{
    vector<string> Ai(3,"");

    for(unsigned int j=0; j<arg_Ai.size(); j++){
        double aij = arg_Ai[j];
        Ai.push_back(numToStr(aij));
    }

    return Ai;
}

vector<string> Z_TableauRow(vector<double> arg_Z)
{
    vector<string>Z(1,"");
    Z.push_back("Zj");

    for(unsigned int j=0; j<arg_Z.size(); j++){
        double zj = arg_Z[j];
        Z.push_back(numToStr(zj));
    }

    return Z;
}

vector<string> C_Z_TableauRow(vector<double> arg_C_Z, MinMax maxmin)
{
    vector<string> C_Z;
    C_Z.push_back("");
    if(maxmin==Maximisation)
        C_Z.push_back("Cj-Zj");
    else C_Z.push_back("Zj-Cj");

    C_Z.push_back("");

    for(int j=0; j<arg_C_Z.size(); j++){
        double cj_zj = arg_C_Z[j];
        C_Z.push_back(numToStr(cj_zj));
    }

    return C_Z;
}


QString outVarStr(SimplexTableau st)
{
    int index = st.Basis[st.pivotColId];
    if(index==-1) return "";

    Var varId = st.vars[index];
    return varToStr(varId, index).c_str();
}

QString inVarStr(SimplexTableau st)
{
    int index = st.pivotColId;
    Var varId = st.vars[index];
    return varToStr(varId, index).c_str();
}

double pivotNumber(SimplexTableau st)
{
    return st.A[st.pivotColId][st.pivotColId];
}


double toDouble(QString val)
{
    if(val.isEmpty()) error("error trying to convert string to double; string is empty.");

    bool *ok=new bool{true};
    double d=0;

    d = val.toDouble(ok);
    if(!ok) error("error trying to convert string to double; is not a number.");

    return d;
}

vector<vector<string>> str(vector<vector<double>> mat)
{
    vector<vector<string>> strMat;
    
    for(int i=0; i<mat.size(); i++){
        vector<string> strRow;
        for(int j=0; j<mat[i].size(); j++){
            strRow.push_back(numToStr(mat[i][j]));
        }
        strMat.push_back(strRow);
    }
    
    return strMat;
}

QTableWidget *toTable(vector<vector<string>> strMat)
{
    QTableWidget *table = new QTableWidget;
    table->setColumnCount(maxColCount(strMat));
    table->setRowCount(strMat.size());

    for(int i=0; i<table->rowCount(); i++){
        for(int j=0; j<table->columnCount(); j++){
            QString itemText = strMat[i][j].c_str();
            QTableWidgetItem *item = new QTableWidgetItem(itemText);
            table->setItem(i,j,item);
        }
    }

    return table;
}


vector<string> varsTostr(vector<Var> vars)
{
    int decisionVarCount = 0;
    int artificialVarCount = 0;
    int slackVarCount = 0;

     vector<string> sl;

    for(Var var: vars){
        switch(var){

        case DecisionVar:
            decisionVarCount++;
            sl.push_back(varToStr(var,decisionVarCount));
            break;

        case ArtificialVar:
            artificialVarCount++;
             sl.push_back(varToStr(var,artificialVarCount));
            break;

        case SlackVar:
            slackVarCount++;
             sl.push_back(varToStr(var,slackVarCount));
            break;
        }
    }

    return sl;
}
