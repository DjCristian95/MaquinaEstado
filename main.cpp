#pragma once
#include <stdio.h>
#include <iostream>

using namespace std;
class Estado;

enum estado{ON=0,OFF};

class Switch {
    private:
        Estado* estadoOff;
        Estado* estadoOn;
        Estado* estadoActual;
    public:
        ~Switch();
        Switch();
        void setEstado(int st);
        void on();
        void off();
};
class Estado{
    public:
        Estado(){}
        virtual void on(Switch* op){}
        virtual void off(Switch* op){}
};
class EstadoOn : public Estado{
    public:
    EstadoOn(){
        cout << "Estado EstadoOn inicializado" << endl;
    }
    void on(Switch* op);
    void off(Switch* op);
};
class EstadoOff : public Estado{
    public:
    EstadoOff(){
        cout << "Estado EstadoOff inicializado" << endl;
    }
    void on(Switch* op);
    void off(Switch* op);
};
Switch::Switch(){
    estadoOff = new EstadoOff();
    estadoOn = new EstadoOn();
    estadoActual=estadoOff;
}
Switch::~Switch(){
    if(estadoOff != NULL)
        delete estadoOff;
    if(estadoOn != NULL)
        delete estadoOn;
}
void Switch::setEstado(int nuevoEstado){
    if(nuevoEstado == ON)
    {
        estadoActual = estadoOn;
    }
    else if(nuevoEstado == OFF)
    {
        estadoActual = estadoOff;
    }
}
void Switch::on(){
    estadoActual->on(this);
}
void Switch::off(){
    estadoActual->off(this);
}
void EstadoOff::on(Switch* op){
    cout << "Transicion de estado de OFF a ON" << endl;
    op->setEstado(ON);
}
void EstadoOff::off(Switch* op){
    cout << "Ya en estado OFF" << endl;
}
void EstadoOn::on(Switch* op){
    cout << "Ya en estado ON" << endl;
}
void EstadoOn::off(Switch* op){
    cout << "Transicion de estado de ON a OFF" << endl;
    op->setEstado(OFF);
}
int main(){
    Switch* swObj = new Switch();
    int ch;
    do{
        switch(ch){
            case 1:
                swObj->on();
                break;
            case 0:
                swObj->off();
                break;
            case 2:
                cout << "Salida programa"<<endl;
                delete swObj;
                return 0;
                break;
            default :
                cout << "Eleccion inválida"<<endl;
                break;
        }
        cout << "Ingrese 0/1 y 2 para salir: ";
        cin >> ch;
    }while(true);
    delete swObj;
    return 0;
}
