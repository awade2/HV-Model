#ifndef PAISAGEM_H
#define PAISAGEM_H
#include "HV-Individual.hpp"
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<cmath>
///** Dimensão maxima de propriedades matriciais da academia */
#define dim 10000 /*Em algum momento pode ser alterado para um argumento do
		    construtor. No momento não é prioritário. */

using namespace std;

class academia
{
  private:
    
    //propriedades privadas
    int colaborationMatrix[dim][dim];
    double probIntMatrix[dim][dim];
    vector <double> minIndTimes;
    vector <int> positions; // is the position in the vector colabTimes of an individual corresponding to the shortest colaboration time of this individual.
    vector <individual*> popInd;
    const unsigned long N;
    double tolRiskSpace[2];
    const double intercTime;
    const double intercIF;
    const double slopeTime;
    const double slopeIF;
    const double varianceTime;
    const double varianceIF;
    const int var_IF;
    const int var_Time;
    const double minTime;
    
    //metodos privados
    
    /* method for creating intitutions
    void institutionalizing ();
    */
    
    void populating(int corTraitRisk);
    
    public:
    
    /** Contador de quanto tempo já transcorreu no mundo simulado */
    double simTime;
    //metodos
    
    /** Construtor da classe paisagem */
    academia(const int N,
             double TRspace[],
             const double intercTime,
             const double intercIF,
             const double slopeTime,
             const double slopeIF,
             const double varianceTime,
             const double varianceIF,
             const double minTime,
             int domination,
             const int var_IF,
             const int var_Time,
             int correlationTrR
              );
    
    void initColaborations(double domination);
    double calcProbInterac(double domination, double risk1, double risk2, double trait1, double trait2);
    double smpIF(int lider, int cols);
    double smpTimes(double impFac);
    double sumVectorValues(vector <double> vec1);
    void updateAcademia();    
    individual* get_ind(int posit) const {return this->popInd[posit];}
    int popSize(){return this->popInd.size();}
};

#endif // PAISAGEM_H
