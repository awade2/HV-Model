#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include <vector>

using namespace std;

/** \brief A classe individuo representa um agente da simulação.
 *
 *  Esta classe contém informações referentes a cada indivíduo, incluindo sua localização e estado, uma lista contendo ponteiros para os vizinhos próximos, tamanho do passo de movimentação, etc. Esta classe NÃO contém métodos que sejam de responsabilidade do ambiente, como o método de atualizar os vizinhos */
class individual
{
private:
    //propriedades
	
    /** Identificador único de cada indivíduo */
    const unsigned long id;
	/** Identificador máximo já usado */
    static unsigned long MAXID;
    
    const double trait;
    const double toleranceRisk;
    
    vector <double> colabTime;
    vector <int> underColab;
    int simulColabNumb;
    //double minTime;
    //int colabMinTime;
    
    vector <double> times;
    vector <double> colaborators;
    vector <double> impactFactor;
    
    /*
    double pubScoreLider;
    double pubScoreTotal;
    double pubRateLider;
    double pubRateTotal;
    */
	
public:
    /** Construtor da classe individuo. Deve ser chamado pela paisagem para posicionar os
     * individuos que já estão na paisagem no início da simulação. */
    individual(
              const double trait,
              /**  */
              const double toleranceRisk
              );
    
    /** Construtor de copia, usado para gerar novos individuos */
    //individual(/** Individuo pai */ const individual& rhs);

    /** Reinicia o contador de indivíduos **/
    static void reset_id() {MAXID = 0;}
    
    //// GETs
    double get_TRisk(){return this->toleranceRisk;}
    double get_trait(){return this->trait;}
    double get_simulColabNumb(){return this->simulColabNumb;}
    double get_colabTime(int row){return this->colabTime[row];}
    double get_underColab(int pos){return this->underColab[pos];}
    double get_underColabSize(){return this->underColab.size();}
    int get_colaboratorMin(int row){return this->underColab[row];}
    double get_times(int ps){return this->times[ps];}
    int get_colaborators(int ps){return this->colaborators[ps];}
    double get_impactFactor(int ps){return this->impactFactor[ps];}
    int get_outSize(){return this->times.size();}
    
    //// SETs
    /* Time */
    void set_times(double tm){this->times.push_back(tm);}
    void set_colabTime(double tm){this->colabTime.push_back(tm);};
    /* Colaborators */
    void set_colaborators(int colabRow){this->colaborators.push_back(colabRow);}
    void set_underColab(int colabRow){this->underColab.push_back(colabRow);}
    /* Impact Factor */
    void set_impactFactor(double IFac){this->impactFactor.push_back(IFac);}
    void set_simulColabNumb(){this->simulColabNumb=this->underColab.size();}
    
    //// ERASEs
    void erase_underColab(int position){this->underColab.erase(underColab.begin()+position);}
    void erase_colabTime(int position){this->colabTime.erase(colabTime.begin()+position);}
    
    //// ...
    int checkMinTime();
};

#endif // INDIVIDUAL_H
