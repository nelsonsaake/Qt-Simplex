#ifndef ADJUST_H
#define ADJUST_H

#include "simplexMethod/simplextableau.h"

struct Adjust
{    
    Adjust(vector<double> a);

    LPModel doTheThing();

    LPModel currentLPModel();
    bool stop();
    void increment();

    int upperLimit = 50;
    vector<double> a;
    vector<double> y;
};

#endif // ADJUST_H
