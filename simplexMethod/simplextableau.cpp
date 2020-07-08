#include "simplextableau.h"
#include "outputcastingformarting.h"

SimplexTableau::SimplexTableau(){}

SimplexTableau::SimplexTableau(AugmentedLPModel alpm):AugmentedLPModel(alpm){}

Var SimplexTableau::basisVar(int i)
{
    return vars[Basis[i]];
}


int SimplexTableau::colCount() const
{
    return C.size();
}

vector<double> SimplexTableau::cb()
{
    vector<double> cb;

    for(double d: Basis)
        cb.push_back(C[d]);

    return cb;
}

vector<double> SimplexTableau::pivotCol()
{
    return A.col(pivotColId);
}

vector<double> SimplexTableau::pivotRow()
{
    return A[pivotRowId];
}


double SimplexTableau::calZj(const vector<double> &basis, const vector<double> &colj)
{
    vector<double> product = dotProduct(basis,colj);
    return sum(product);
}

void SimplexTableau::calZ()
{
    // the simplex tableau
    // multiply the basis col with col j, sum the product, that is our zj value

    // to be safe, let's make the is nothing there to start with
    Z.clear();

    // calculate the zj value for the
    double Zb = calZj(cb(), B);
    Z.push_back(Zb);

    for(int j=0; j<colCount(); j++)
    {
        vector<double> colj = A.col(j);
        double Zj = calZj(cb(),colj);
        Z.push_back(Zj);
    }
}

void SimplexTableau::calDeltaCZ()
{
    deltaCZ.clear();

    // go col to col and calculate change
    for(int j=0; j<C.size(); j++)
    {
        //
        double changecz = C[j]-Z[j+1];

        // for minisation the change is
        // is zj - cj
        // this particular part depends on how you find your pivot column
        if(objective == Minimisation)
            changecz = Z[j+1]-C[j];

        // now we store it
        deltaCZ.push_back(changecz);
    }
}

void SimplexTableau::inVar()
{
    // we want to make sure we have something even before we start looking
    if(deltaCZ.empty())
        throw("Error finding entering Non Basic Variable: the Z list is empty.");

    // we assume the first element is the largest will we try to disprove that assumption
    int maxValIndex = 0;
    for(int j=0; j<deltaCZ.size(); j++)
    {
        // the assumed max value is less that another number
        // that number is larger than the assumed max val
        // so until we find another number bigger than that one
        // that value become the new assumed max val
        if(deltaCZ[maxValIndex]<deltaCZ[j]) maxValIndex = j;
    }

    pivotColId = maxValIndex;
}

void SimplexTableau::outVar()
{
    vector<double> Bi=B;
    vector<double> res = divide(Bi,pivotCol());
    int minValIndex = minNonNegativeIndex(res);

    pivotRowId = minValIndex;
    pivotNumber = -1;

    if(minValIndex == -1) return;

    pivotNumber = A.cell(pivotRowId,pivotColId);
}

vector<int> SimplexTableau::newBasis()
{
    // we first get the old basis and replace the one leaving
    vector<int> newBasis = Basis;

    // just to be safe we call for the calculation of the leaving basis
    // and the entering non basis
    //    inVar(st);
    //    outVar(st);

    // the st.leavingBasicVar is the index in the Basis of the leaving variable
    // the entringNonBasicVar is the index in C, the Cj value that will be entring the Basis
    // the B values are actually indexes of the actual value in C
    // that is to say the B is referencing the Basis from C and that B is not storing any actual values but indexes
    newBasis[pivotRowId] = pivotColId;

    return newBasis;
}

void SimplexTableau::calNewPivotRow()
{
    // clear the new pivot row vector
    newPivotRow.clear();

    for(double pivotRowVal: pivotRow())
        newPivotRow.push_back(pivotRowVal/pivotNumber);
}

vector<double> SimplexTableau::newOtherRow(int rowIndex)
{
    // new row
    vector<double> otherRow;

    for(int j=0; j<A.colCount(); j++)
    {
        // old row vals
        double d = A.cell(rowIndex,j);

        // [old row vals] - ([corresponding val in pivot column]*[new pivot row value])
        double x  = d - (A.cell(rowIndex,pivotColId) *(newPivotRow[j]));

        // use what we get to create our new row
        otherRow.push_back(x);
    }

    return otherRow;
}

Matrix SimplexTableau::newA()
{
    // we iterate through the A matrix and for each row we calculate the new other rows
    Matrix newA(A.rowCount(), A.colCount());

    calNewPivotRow();
    newA.row(pivotRowId) = newPivotRow;

    // [old row vals] - ([coefficient in pivot column]*[new pivot row value])
    for(int i=0; i<A.rowCount(); i++){
        if(i == pivotRowId) continue;
        newA[i] = newOtherRow(i);
    }

    return newA;
}

vector<double> SimplexTableau::newBiValues()
{
    // first we calculate the bi value for the pivot row
    double newPivotRowVal = B[pivotRowId]/pivotNumber;

    // calcualte the bi values for the other row values
    vector<double> newB(B.size(), 0);

    for(int i=0; i<B.size(); i++){
        if(i == pivotRowId) {
            newB[i] = newPivotRowVal;
            continue;
        }

        // corresponding old pivot col val
        double rowi_pivotCol_val = A[i][pivotColId];

        // new other row value
        newB[i] = B[i] - rowi_pivotCol_val*newPivotRowVal;
    }

    return newB;
}


SimplexTableau SimplexTableau::nextTableau()
{
    SimplexTableau newTableau;

    // start fill the new tableau
    newTableau.objective = objective;
    newTableau.C = C;
    newTableau.vars = vars;

    // do calculations on the old tableau
    // neccessary to get this one
    newTableau.Basis = newBasis();
    newTableau.A = newA();
    newTableau.B = newBiValues();

    newTableau.calZ();
    newTableau.calDeltaCZ();
    newTableau.inVar();
    newTableau.outVar();

    return newTableau;
}


bool SimplexTableau::isToStop()
{
    // if we didn't find the leaving basis var
    if(pivotRowId < 0){
        return true;
    }

    // go through the deltaCZ values
    for(double d : deltaCZ)
        if (d > 0) // if we found that first value that is not less or equal to zero,
            return false; // we know is not optimal

    //

    return true;
}

bool SimplexTableau::isInfeasible()
{
    for(int bi: Basis)
        if(vars[bi] == ArtificialVar)
            return true;

    return false;
}

bool SimplexTableau::isUnbounded()
{
    if(pivotRowId < 0){
        return true;
    }
    return false;
}

bool SimplexTableau::isSolution()
{
    if(isInfeasible()||isUnbounded()) return false;
     return true;
}


vector<vector<string>> SimplexTableau::stToStrMat()
{
    vector<vector<string>> os;

    os.push_back(C_TableauRow(C)); // C
    os.push_back(labels_TableauRow(vars)); // labels

    // A
    for(int i=0; i<A.size(); i++){
        os.push_back(Ai_TableauRow(A[i]));
    }

    // cb, basis, bi
    {

        vector<string> vars = varsTostr(this->vars);
        for(int i=0; i<B.size(); i++){
            // first two rows for labels
            int rowi=2+i;

            // cb
            double cb = C[Basis[i]];
            os[rowi][0] = numToStr(cb);

            // basis
            // the basis list has the var ids
            int index = Basis[i];
            os[rowi][1] = vars[index];

            // bi
            os[rowi][2] = numToStr(B[i]);
        }
    }

    os.push_back(Z_TableauRow(Z)); // Zj
    os.push_back(C_Z_TableauRow(deltaCZ, objective)); // Cj - Zj

    return os;
}


QString stepSummary(int i, SimplexTableau step, bool makeShort)
{
    ostringstream oss;
    // make label
    oss << "Tableau " << i+1 << "<br>";

    if(!makeShort){
        oss << "in: " << inVarStr(step).toStdString() << "<br>";
        oss << "out: " << outVarStr(step).toStdString() << "<br>";
        oss << "pivot number: " << pivotNumber(step);
    }

    // add to ui
    return oss.str().c_str();
}

QTableWidget *summary(SimplexTableau lt)
{
    QTableWidget *summary = new QTableWidget;
    summary->setRowCount(lt.Basis.size()+1);    // +1 for z
    summary->setColumnCount(2);

    vector<string> vars = varsTostr(lt.vars);
    // bi
    for(int i=0; i<lt.Basis.size(); i++){
        int varIndex = lt.Basis[i];
        QString vari = vars[varIndex].c_str();
        summary->setItem(i,0, new QTableWidgetItem(vari));
        summary->setItem(i,1, new QTableWidgetItem(numToStr(lt.B[i]).c_str()));
    }

    // Z
    {
        int i = summary->rowCount()-1;
        summary->setItem(i,0, new QTableWidgetItem("Z"));
        summary->setItem(i,1, new QTableWidgetItem(numToStr(lt.Z[0]).c_str()));
    }

    return summary;
}

QString complications(SimplexTableau t)
{
    QString conclusion = "";

    if(t.isUnbounded())
        conclusion = "This is an unbounded problem.<br>";

    else if(t.isInfeasible())
        conclusion = "Infeasible problem.<br>";

    return conclusion;
}

vector<SimplexTableau> solve(LPModel lpm)
{
    vector<SimplexTableau> steps;
    AugmentedLPModel alpm;
    marshal(lpm,alpm);

    // initial tableau
    SimplexTableau st;
    marshal(alpm, st);

    // find the optimal solution
    for(int i=0; /*(i<3) &&*/ (!st.isToStop()); i++)
    {
        // if our current simplex tableau is not optimal

        // add it to history
        steps.push_back(st);

        // based on this simplex tableau, find the next one
        // go back to the top
        st = st.nextTableau();
    }

    // Save last tableau
    steps.push_back(st);

    return steps;
}

SimplexTableau solve_finalTableau(LPModel lpm)
{
    vector<SimplexTableau> steps = solve(lpm);

    if(steps.empty()) error("Error trying to return last tableau, tableau list is empty.");

    return steps[steps.size()-1];
}
