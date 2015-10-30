#include "gridsubinterval.h"

GridSubInterval::GridSubInterval(double l, double u)
{
    lowerBound = l;
    upperBound = u;
}


GridSubInterval::~GridSubInterval()
{

}

bool GridSubInterval::belongsToSubinterval(double value, bool last)
{
    if (last)
    {
        if ( (value >= lowerBound) && (value <= upperBound) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }else
    {

        if ( (value >= lowerBound) && (value < upperBound) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

}

double GridSubInterval::getLowerBound()
{
    return lowerBound;
}

double GridSubInterval::getUpperBound()
{
    return upperBound;
}


void GridSubInterval::printSubInterval()
{

}
