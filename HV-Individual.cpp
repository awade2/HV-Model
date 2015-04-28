#include "HV-Individual.hpp"
#include <cmath>
#include "HV-Academia.hpp"
#include <cstdlib>
#include <iostream>
#include <R.h>
#include <Rmath.h>

using namespace std;

unsigned long individual::MAXID = 0;

individual::individual(double trait, double toleranceRisk):
    id(++MAXID), // pega o próximo ID livre
    trait(trait),
    toleranceRisk(toleranceRisk)
{
    
}

int individual::checkMinTime()
{
    double mti=this->colabTime[0];
    int inPosition=0;
    if(this->colabTime.size()>1)
    {
        for(int i=1;i<this->colabTime.size();i++)
        {
            if(this->colabTime[i]<mti)
            {
                mti=this->colabTime[i];
                inPosition=i;
            }
        }
    }    
    return inPosition;
}


/* Method for creating new individuals after initial conditions
individual::individual(const individual& rhs):
    id(++MAXID),
    trait(rhs.trait),
    toleranceRisk(rhs.toleranceRisk)
{ //precisamos dessa chave e da que fecha ela?
	
}
*/
