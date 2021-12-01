#include <list>
#include<iostream>
#include <map>

using namespace std;


struct IEvento{};

class Evento{
    private:
   Evento* evento ;
    public:
    Evento(){};
    Evento(Evento* evento_):evento(evento_) {};
    ~Evento(){};

};

class State;
class Accion;

struct IProcess{
    virtual bool procesarEvento(Evento*)= 0;
    virtual~IProcess(){};
};

struct ICorrespondenciaEstado{
    virtual Accion* buscaAccion(State*)=0;
    virtual ~ICorrespondenciaEstado(){};
};

class CorrespondenciaEstado: public ICorrespondenciaEstado{
    public:
    CorrespondenciaEstado(){};
    ~CorrespondenciaEstado(){};
    Accion* buscaAccion(State*);
    private:
    map<State,Accion> correspondenciaEstado;
};

Accion* CorrespondenciaEstado::buscaAccion(State*){
    Accion* una;// busca del map la accion correspondiente a ese estado
  correspondenciaEstado; //
      cout<<"Accion que se ejecuta en ese estado"<<endl;
    return una;
}



struct IAccion{
    virtual Accion* getAccion(State*)= 0;
    virtual ~IAccion(){};
};

class Accion: public IAccion{
    public:
    Accion(     ICorrespondenciaEstado* corresponde_ = nullptr) {
                this->corresponde = corresponde_ ?: new CorrespondenciaEstado();}
    ~Accion(){};
    Accion* getAccion(State*);

    private:

    ICorrespondenciaEstado* corresponde;
};

Accion* Accion::getAccion(State* estado){
   // obtengo la accion para ese estado
return  corresponde->buscaAccion(estado);
}

class Guarda;


struct IFSM: public IProcess{
    virtual State* cambioEstado(Evento*evento,State*estado)=0;
    virtual~IFSM(){};
};

struct IGuarda{
    virtual bool evaluarGuarda(Evento* ,State* )= 0;
    virtual~IGuarda(){};
};

class Guarda: public IGuarda{
    public:
    Guarda(){};
    ~Guarda(){};
    bool evaluarGuarda(Evento* evento,State* estado);

    private:
     IGuarda* guarda;
};

bool Guarda::evaluarGuarda(Evento* evento,State* estado){
    cout<<"Valida Guarda----> guarda valida"<<endl;
    //validar la guarda de acuerdo al evento entrante y el estado actual

return true;
}

struct IState : public IProcess {
    virtual  void manifestarEfecto(State*)= 0;
    virtual ~IState(){};
};


struct IPersistencia{
    virtual void persisteEstado(IState*)= 0;
    virtual ~IPersistencia(){};
};

class Persistencia:public IPersistencia{
public:
    Persistencia(){};
    ~Persistencia(){};
    void persisteEstado(IState*);

private:
    IPersistencia* memoria;
};

void Persistencia::persisteEstado(IState*){
    cout<< "Persiste en memoria ultimo estado"<<endl;
    // carga en un archivo  el ultimo estado
}


class State : public IState{
    private:
        //IState *estadoAnterior;
        IState* estadoActual;
        //IAccion* actionEntry;
       // IAccion* actionExit;
        IAccion* doing;
        list<Evento>*esperados;


    public:
        State(
              list<Evento>*esperados_ = nullptr,
              IAccion* doing_ = nullptr) {
               this->esperados = esperados_ ?: new list<Evento>();
               this->doing = doing_ ?: new Accion();}

        virtual~State(){delete doing;delete esperados;};
        void setAccion(Accion*);
        IState* getState(){return estadoActual;}
        void setAnterior(State& estado);
        void manifestarEfecto(State* );
        bool procesarEvento(Evento* evento);

};

bool State::procesarEvento(Evento* evento){
    cout<<"procesa evento---> evento valido"<<endl;
    //buscar en Eventos esperados si el evento entrado corresponde al estado
    return true;

}

void State::manifestarEfecto(State* estado){
doing->getAccion(estado);

}

struct IRules{
    virtual State* getValue ( Evento* value,State* key)=0;
    virtual~IRules(){};
};

template<class State, class Evento>
class Rules: public IRules{
    public:
        Rules(){};
        ~Rules(){};
        State* getValue ( Evento* value, State* key);
    private:
        map< pair<State&,Evento&>,State&> rules {  };

};

template<class State, class Evento>
State* Rules<State, Evento>::getValue( Evento* value,State* key){
        State* nuevoEstado;
        cout<<"busca reglas de transicion---> retorna nuevo estado"<<endl;
        return nuevoEstado;
}

struct ITransicion{
    virtual State* dispararTransicion(Evento*,State*)= 0;
    virtual ~ITransicion(){};
};

class Transicion: public ITransicion{
    public:
    Transicion(
                IRules* trans_ = nullptr)
                {this->rulesTrans = trans_ ?: new Rules<State,Evento>();
                };
    ~Transicion(){delete rulesTrans;};
    State* dispararTransicion(Evento* evento,State* estado);
    State* consultaRegla(Evento*,State*);

    private:
    IRules* rulesTrans;


};

State* Transicion:: dispararTransicion(Evento* evento,State* estado){
    State* nuevoEstado= estado;
    nuevoEstado=consultaRegla(evento,estado);
    return nuevoEstado;

}

State* Transicion:: consultaRegla(Evento*evento,State*estado){

    return rulesTrans->getValue( evento, estado);

}


class FSM: public IFSM {
    public:
    FSM(
            IState* estadoActual_ = nullptr,
            ITransicion* transicion_ = nullptr,
            IGuarda* guarda_ = nullptr,
            IPersistencia* memoria_ = nullptr ) {
                this->estadoActual = estadoActual_ ?: new State();
                this->transicion = transicion_ ?: new Transicion();
                this->guarda = guarda_ ?: new Guarda();
                this->memoria = memoria_ ?:new Persistencia();
                }
    ~FSM() { memoria->persisteEstado(estadoActual);
            delete estadoActual;
            delete transicion;
            delete guarda;
            delete memoria;
    }

    bool procesarEvento(Evento* evento);
    State* cambioEstado(Evento* evento,State* estado);
    void ejecutaAccion(State* estado);
    private:
    IState* estadoActual;
    ITransicion *transicion;
    IGuarda* guarda;
    IPersistencia* memoria;
};
void FSM::ejecutaAccion(State* estado){
   estadoActual->manifestarEfecto(estado);
}

bool FSM::procesarEvento(Evento* evento){
    return this->estadoActual->procesarEvento(evento);
}

State* FSM:: cambioEstado(Evento*evento,State*estado) {
    State* result;
    if(guarda->evaluarGuarda(evento,estado))
    result=this->transicion->dispararTransicion(evento,estado);
    this->ejecutaAccion(result);
    return result;
}

void Objeto( IFSM*maquina, Evento* evento, State* estado){
  if( maquina->procesarEvento( evento))
   maquina->cambioEstado(evento, estado);

}

int main(){
    State* estado = new State();
    Evento* evento = new Evento();
    IFSM* maquina = new FSM( );

    Objeto(maquina,evento,estado);

    delete maquina;
    delete evento;
    delete estado;

    return 0;
}
