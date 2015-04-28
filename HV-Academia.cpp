#include "HV-Academia.hpp"
#include "HV-Individual.hpp"
#include <R.h>
#include <Rmath.h>

academia::academia(int N, double TRspace[], double varianceIF, double intercImp, double slopeImp,
                   double varianceTime, double intercTime, double slopeTime, double minTime, int domination, int var_IF, int var_Time, int correlationTrR):

N(N),
simTime(0),
varianceIF(varianceIF),
varianceTime(varianceTime),
intercTime(intercTime),
intercIF(intercImp),
slopeIF(slopeImp),
slopeTime(slopeTime),
minTime(minTime),
var_IF(var_IF),
var_Time(var_Time)

{
    this->tolRiskSpace[0]=TRspace[0];
    this->tolRiskSpace[1]=TRspace[1];
    this->populating(correlationTrR);
    this->initColaborations(domination);
}


void academia::populating(int corTraitRisk)
{
    individual::reset_id(); // reinicia o contador de id dos individuos
    
    if(corTraitRisk==0)
    {
        for(int i=0; i<this->N; i++)
        {
            this->popInd.push_back(new individual(runif(0,1),
                                                  runif(this->tolRiskSpace[0],this->tolRiskSpace[1])
                                                  ));
        }
    }
    else
    {
        for(int i=0; i<this->N; i++)
        {
            double trt=runif(0,1);
            this->popInd.push_back(new individual(trt,
                                                  trt*(this->tolRiskSpace[1]-this->tolRiskSpace[0])+this->tolRiskSpace[0]
                                                  ));
        }
    }
    
}

void academia::initColaborations(double domination)
{
    // if domination < 1 ...... else .......
    // if carrying capacity ..... else .......
    // Only domination=1 and without carrying capacity now
    double probInt;
    double sampleValue;
    double Tms;
    double IFS;
    for(int i=0; i<this->popInd.size();i++)
    {
        for(int j=0; j<this->popInd.size();j++)
        {
            if(i==j) continue;
            probInt=this->calcProbInterac(domination,this->popInd[i]->get_TRisk(),this->popInd[j]->get_TRisk(),this->popInd[i]->get_trait(),
                                          this->popInd[j]->get_trait());
            this->probIntMatrix[i][j]=probInt;
            sampleValue=runif(0,1);
            if(sampleValue<probInt)
            {
                this->colaborationMatrix[i][j]=1;
                this->popInd[i]->set_colaborators(j);
                this->popInd[i]->set_underColab(j);
                
                IFS=smpIF(i,j);
                this->popInd[i]->set_impactFactor(IFS);
                Tms=smpTimes(IFS);
                this->popInd[i]->set_times(Tms);
                this->popInd[i]->set_colabTime(Tms);
            
            }
            else {this->colaborationMatrix[i][j]=0;}
        }
        while(this->popInd[i]->get_outSize()<1)
        {
            for(int j=0; j<this->popInd.size();j++)
            {
                sampleValue=runif(0,1);
                if(sampleValue<probInt)
                {
                    this->colaborationMatrix[i][j]=1;
                    this->popInd[i]->set_colaborators(j);
                    this->popInd[i]->set_underColab(j);
                    
                    IFS=smpIF(i,j);
                    this->popInd[i]->set_impactFactor(IFS);
                    Tms=smpTimes(IFS);
                    this->popInd[i]->set_times(Tms);
                    this->popInd[i]->set_colabTime(Tms);
                    
                }
                else {this->colaborationMatrix[i][j]=0;}
            }
        }
        this->popInd[i]->set_simulColabNumb(); // this is the number of colaborations the individual i is involved in a specific time.
        
        int posit=0;
        double MiTi=this->popInd[i]->get_colabTime(posit);
        if(this->popInd[i]->get_simulColabNumb()>1)
        {
            for(int k=1;k<this->popInd[i]->get_simulColabNumb();k++)
            {
                if(this->popInd[i]->get_colabTime(k)<MiTi)
                {
                    MiTi=this->popInd[i]->get_colabTime(k);
                    posit=k;
                }
            }
        }
        this->minIndTimes.push_back(MiTi);
        this->positions.push_back(posit);
    }
}

double academia::calcProbInterac(double domination, double risk1, double risk2, double trait1, double trait2)
{
    double probInterac;
    probInterac=domination*(exp((-1/risk1)*(fabs(trait1-trait2))))+(1-domination)*(exp((-1/risk2)*(fabs(trait1-trait2))));
    return probInterac;
}

double academia::smpIF(int lider, int cols)
{
    double impact;
    double meanImp=this->intercIF+this->slopeIF*((this->popInd[lider]->get_TRisk()+this->popInd[cols]->get_TRisk())/2);
    if(this->var_IF==1)
    {
        impact=rnorm(meanImp,this->varianceIF*meanImp);
        while(impact<0){impact=rnorm(meanImp,this->varianceIF*meanImp);}
    }
    else
    {
        impact=rnorm(meanImp,this->varianceIF);
        while(impact<0){impact=rnorm(meanImp,this->varianceIF);}
    }
    return impact;
}

double academia::smpTimes(double impFac)
{
    double tm;
    double meanTm=this->intercTime+this->slopeTime*impFac;
    if(this->var_Time==1)
    {
        tm=rnorm(meanTm,this->varianceTime*meanTm);
        while(tm<this->minTime){tm=rnorm(meanTm,this->varianceTime*meanTm);}
    }
    else
    {
        tm=rnorm(meanTm,this->varianceTime);
        while(tm<this->minTime){tm=rnorm(meanTm,this->varianceTime);}
    }
    return tm;
}

void academia::updateAcademia()
{
    int aux1, aux2;
    int minPosition=0;
    double MT=this->minIndTimes[minPosition];
    for(int i=1; i<this->minIndTimes.size();i++)
    {
        if(this->minIndTimes[i]<MT)
        {
            MT=this->minIndTimes[i];
            minPosition=i;
        }
    }
    aux1=this->positions[minPosition]; // aux1 = position in the vector of colaborations and times of the individual[minPosition]
    aux2=this->popInd[minPosition]->get_colaboratorMin(aux1); // aux2 = position the colaborator in the popInd vector (i.e. the individual colaborating with the one who had the lowest colaboration time)
    this->colaborationMatrix[minPosition][aux2]=0;
    this->simTime=MT;
    
    /// erase the finished colaboration from underColab and colabTime of individual minPosition.
    this->popInd[minPosition]->erase_underColab(aux1);////////////////////////////////////////////
    this->popInd[minPosition]->erase_colabTime(aux1);//////////////////////////////////////////////////
    
    /// sample new colaboration for the chosen individual (minPosition)
    vector <double> intProbab;
    vector <int> potColab;
    int aux3=0;
    for(int i=0;i<this->popInd.size();i++)
    {
        if(i==minPosition) continue;
        if(this->colaborationMatrix[minPosition][i]==0)
        {
            intProbab.push_back(this->probIntMatrix[minPosition][i]);
            potColab.push_back(i);
            aux3=aux3+1;
        }
    }
    double sumProbs=sumVectorValues(intProbab);
    double probIni=0;
    double sampleColaborator=runif(0,sumProbs);
    int newColaborator;
    for(int i=0;i<intProbab.size();i++)
    {
        probIni=probIni+intProbab[i];
        if(sampleColaborator<probIni)
       {
           newColaborator=potColab[i];
           this->colaborationMatrix[minPosition][newColaborator]=1;
           break;
       }
    }
    this->popInd[minPosition]->set_colaborators(newColaborator);
    
    /// sample impact factor and colaboration time for the new colaboration.
    double ImF=smpIF(minPosition,newColaborator);
    this->popInd[minPosition]->set_impactFactor(ImF);
    double nTime=smpTimes(ImF)+this->simTime;
    this->popInd[minPosition]->set_times(nTime);
    
    /// update UnderColab and colabTime vectors of the individual minPosition.
    this->popInd[minPosition]->set_underColab(newColaborator);
    this->popInd[minPosition]->set_colabTime(nTime);
    
    /// Update vetor minIndTimes and positions from academia in position minPosition.
    int toPositions=this->popInd[minPosition]->checkMinTime();
    this->positions[minPosition]=toPositions;
    this->minIndTimes[minPosition]=this->popInd[minPosition]->get_colabTime(toPositions);
}

double academia::sumVectorValues(vector <double> vec1)
{
    double sumVVal=vec1[0];
    for(int z=1;z<vec1.size();z++)
    {
        sumVVal=sumVVal+vec1[z];
    }
    return sumVVal;
}
