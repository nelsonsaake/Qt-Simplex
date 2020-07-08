#include "sm1_simplex_approach.h"
#include "outputcastingformarting.h"

LPModel make_smi_lpm(double a, double b, double c, double ya, double yb, double yc)
{
    LPModel lpm;
    lpm.objective = Minimisation;
    lpm.C = {1,1,1};
    lpm.B = {ya,yb,yc,1,1,1};
    lpm.relationalOperators = {GreaterEqual,
                               GreaterEqual,
                               GreaterEqual,
                               GreaterEqual,
                               GreaterEqual,
                               GreaterEqual};
    marshal(lpm.A,
            {
                {a-1,-1,-1},
                {-1,b-1,-1},
                {-1,-1,c-1},
                {1,0,0},
                {0,1,0},
                {0,0,1}
            });

    return lpm;
}
