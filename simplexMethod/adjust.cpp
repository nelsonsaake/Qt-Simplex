#include "adjust.h"
#include "simplexMethod/sm1_simplex_approach.h"

Adjust::Adjust(vector<double> a)
    : a(a)
{
    y={1,1,1};
}

LPModel Adjust::doTheThing()
{
    while(!stop()){
        LPModel lpm=currentLPModel();
        SimplexTableau finalTableau;
        finalTableau=solve_finalTableau(lpm);
        if(finalTableau.isSolution()) return lpm;
        increment();
    }

    error("Error adjusting, didn't find any suitable replacement.");
    return LPModel();
}

bool Adjust::stop()
{
    if(y.size()<3) error("Error checking for stopping criterion, adjust: not enough elements in list y.");

    for(double d: y)
        if(d < upperLimit)
            return false;

    return true;
}

void Adjust::increment()
{
    if(y.size()<3) error("Error incrementing, adjust: not enough elements in list y.");

    y[2]++;

    if(y[2]>upperLimit){
        y[2]=1;
        y[1]++;
    }

    if(y[1]>upperLimit){
        y[1]=1;
        y[0]++;
    }

    if(y[0]>upperLimit) return;
}

LPModel Adjust::currentLPModel()
{
    if(a.size()<3) error("Error adjusting lpm: not enough elements in list a.");
    return make_smi_lpm(a[0],a[1],a[2],y[0],y[1],y[2]);
}
