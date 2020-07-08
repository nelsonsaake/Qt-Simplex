#include "augmentedlpmodel.h"
#include "util.h"
#include "alg.h"

AugmentedLPModel::AugmentedLPModel()
{

}

void AugmentedLPModel::addSlackVariable(int i, double val)
{
    // add variable to variable list
    vars.push_back(SlackVar);

    // reflect in objective function
    C.push_back(0);

    // add variable to contraints
    // add column
    appendCol(A);

    // set the value of that matrix value
    A[i][A[i].size()-1] = val;
}

void AugmentedLPModel::addArtificialVariable(int i)
{
    // add variable to variable list
    vars.push_back(ArtificialVar);

    // reflect in objective function
    // atificial variable bears M in objective function
    // M is a really huge number
    // for minimisation positive M
    if(objective == Minimisation) C.push_back(M);

    // maximisation negative M
    else if(objective == Maximisation) C.push_back(-M);

    // add a column to represent the artificial variable in all constraints equations
    appendCol(A);

    // set the coefficients of the variable the required contraints equation
    A[i][A[i].size()-1] = 1;
}



void AugmentedLPModel::sort()
{
    if (vars.size() < 1) return;

    const int startIndex = 1;

    for(int i=startIndex; i<vars.size(); i++){

        if(vars[i-1] > vars[i]) {

            this->swap(i,i-1);

            i = startIndex;

            continue;
        }
    }
}

void AugmentedLPModel::swap(int i, int j)
{
    if(i==j) return;

    A.colSwap(i,j);
    std::swap(C[i], C[j]);
    std::swap(vars[i], vars[j]);
}
