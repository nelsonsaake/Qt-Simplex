#ifndef AUGMENTEDLPMODEL_H
#define AUGMENTEDLPMODEL_H

#include "lpmodel.h"

// Augmented Matrix
struct AugmentedLPModel:LPModel
{
    AugmentedLPModel();
    AugmentedLPModel(LPModel lpm):LPModel(lpm){}

    void addSlackVariable(int i, double val);
    void addArtificialVariable(int i);

    // sort, this is an after taught
    // to arrange the cols, so that the decision variables come first,
    // then the slack variables, then the artificial variables
    // however the entire class structure can be redone and re-organised to make more sense
    void sort();

    void swap(int i, int j);

    // variable list
    // the variable is identified by the type and the index
    // the index can be gotten can be gotten by simply going through the vector
    // the type is what is stored
    // if we have 3 elements => we have 3 variables
    // if the first two are objective variables, the type will be ObjectiveVar
    // and if the last is slack variable, the type will be SlackVar
    // so if you take the coefficient matrix and take an element, the variable can be gotten by check the -
    // corresponding index the var list, you combine the type and index and you can get -
    // ObjectiveVar1 a.k.a x1 or SlackVar3 a.k.a s3
    vector<Var> vars;

    // the variables in the basis
    // we capture the first set when we are augmenting the LPModel
    // the values of the basis is index in the C
    // that is, the basis don't store values but rely on the C
    // it know that Basis[0] = 2;
    // which means that the first Basis variable(moving from top to down) is the same as the third element in the C
    vector<int> Basis;

};

#endif // AUGMENTEDLPMODEL_H
