#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <R.h>
#include <Rmath.h>
#include "HV-Academia.cpp"
#include "HV-Individual.cpp"
#include <sstream>
#include <string>

using namespace std;

extern "C" void HVmodel (int * N, double * TRspace, double * varianceIF, double * intercImp, double * slopeImp,
                         double * varianceTime, double * intercTime, double * slopeTime, double * minTime, int * domination, int * var_IF, int * var_Time,
                         int * correlationTrR, double * maxTime, int * outCode)
{
	// This sequence creates an attribute containing the output file name. The template is output-00000.txt.
	string fileNAME = "outCOLAB-00000.txt";
	stringstream tmps;
	tmps<<outCode[0];
	string addToName = tmps.str();
	int fnSize = fileNAME.size();
    int tmpsSize = addToName.size();
	fileNAME.erase(fileNAME.begin()+fnSize-4-tmpsSize,fileNAME.begin()+fnSize-4);
	fileNAME.insert(fnSize-4-tmpsSize,addToName);
    
    string fileNAME1 = "outTIMES-00000.txt";
    stringstream tmps1;
    tmps1<<outCode[0];
    string addToName1 = tmps1.str();
    int fnSize1 = fileNAME1.size();
    int tmpsSize1 = addToName1.size();
    fileNAME1.erase(fileNAME1.begin()+fnSize1-4-tmpsSize1,fileNAME1.begin()+fnSize1-4);
    fileNAME1.insert(fnSize1-4-tmpsSize1,addToName1);
    
    string fileNAME2 = "outIFac-00000.txt";
    stringstream tmps2;
    tmps2<<outCode[0];
    string addToName2 = tmps2.str();
    int fnSize2 = fileNAME2.size();
    int tmpsSize2 = addToName2.size();
    fileNAME2.erase(fileNAME2.begin()+fnSize2-4-tmpsSize2,fileNAME2.begin()+fnSize2-4);
    fileNAME2.insert(fnSize2-4-tmpsSize2,addToName2);
    
    string fileNAME3 = "IndProp-00000.txt";
    stringstream tmps3;
    tmps3<<outCode[0];
    string addToName3 = tmps3.str();
    int fnSize3 = fileNAME3.size();
    int tmpsSize3 = addToName3.size();
    fileNAME3.erase(fileNAME3.begin()+fnSize3-4-tmpsSize3,fileNAME3.begin()+fnSize3-4);
    fileNAME3.insert(fnSize3-4-tmpsSize3,addToName3);
	
	GetRNGstate(); /* (mudar para doxygen):  este comando chama o engine de numeros aleatorios do R
					Por causa dela nossa biblioteca nao eh standalone */
	
	academia* academy = new academia(N[0], TRspace, varianceIF[0], intercImp[0], slopeImp[0], varianceTime[0], intercTime[0],
                                     slopeTime[0], minTime[0], domination[0],var_IF[0], var_Time[0], correlationTrR[0]);
    
    while (academy->simTime < maxTime[0]){academy->updateAcademia();}
	
    ofstream outputSIM;
    ofstream outputSIM2;
    ofstream outputSIM3;
    ofstream outputSIM4;
    outputSIM.open(fileNAME.c_str());
    outputSIM2.open(fileNAME1.c_str());
    outputSIM3.open(fileNAME2.c_str());
    outputSIM4.open(fileNAME3.c_str());
    
    outputSIM4 << "id" << " " << "trait" << " " << "tolerance_risk" << " " << "Simult_colaborations"<< " " << "Tot_colaborations"<< endl;
    for(int i=0; i<academy->popSize();i++)
    {
        outputSIM4 << i+1 << " " << academy->get_ind(i)->get_trait() << " " << academy->get_ind(i)->get_TRisk() << " " << academy->get_ind(i)->get_simulColabNumb()<< " " << academy->get_ind(i)->get_outSize() << endl;
    }
    outputSIM4.close();
    
    for(unsigned int i=0; i<academy->popSize();i++)
    {
        int vecSize=academy->get_ind(i)->get_outSize();
        outputSIM << academy->get_ind(i)->get_colaborators(0);
        outputSIM2 << academy->get_ind(i)->get_times(0);
        outputSIM3 << academy->get_ind(i)->get_impactFactor(0);
        if(vecSize>1)
        {
            for(int j=1;j<vecSize;j++)
            {
                outputSIM << " " << academy->get_ind(i)->get_colaborators(j);
                outputSIM2 << " " << academy->get_ind(i)->get_times(j);
                outputSIM3 << " " << academy->get_ind(i)->get_impactFactor(j);
            }
        }
        
        outputSIM << endl;
        outputSIM2 << endl;
        outputSIM3 << endl;
    }
	
    outputSIM.close();
    outputSIM2.close();
    outputSIM3.close();
	
	delete academy;
	PutRNGstate();
}


