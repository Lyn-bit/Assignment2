//Ciccone Davide

#ifndef TRAIN_H
#define TRAIN_H

#include "station.h"

#include <list>

class Train
{
public:
    
    virtual int getMaxSpeed() = 0; //metodo che ritorna la velocità massima
    virtual int getMaxWait() = 0; //metodo che ritorna il tempo d'attesa massimo
    virtual int getId() = 0; //metodo che ritorna l'id del treno
    virtual int getVerse() = 0; //metodo che ritorna il verso del treno
    virtual int getType() = 0; //metodo che ritorna il tipo di treno
    virtual std::list<int> getTimes() = 0; //metodo che ritorna la lista degli orari
    virtual int getSpeed() = 0; //metodo che ritorna la velocità del treno
    virtual int getPosition() = 0; //metodo che ritorna la posizione del treno
    
    //metodo che ritorna la stazione in cui il treno si trova al momento
    virtual std::string getCurrentStation() = 0;
    
    //metodo che ritorna la prossima stazione in cui il treno dovrà andare
    virtual std::string getNextStation() = 0;
    
    //metodo che imposta il tempo di attesa del treno
    virtual void SetWaitTime(int waitTime) = 0;
    //metodo che ritorna il tempo di attesa del treno
    virtual int GetWaitTime() = 0;
    
    //metodo che imposta il binario in cui il treno dovrà andare
    virtual void SetTrack(int trackNumber) = 0;
    //metodo che ritorna il binario in cui il treno dovrà andare
    virtual int GetTrack() = 0;
    
    //metodo che invia una richiesta d'arrivo alla stazione
    virtual void SendArrivalRequest() = 0;
    
    //metodo che invia una richiesta di partenza alla stazine
    virtual void SendDepartureRequest() = 0;
    
    
    virtual void resize_timeList() = 0;
    
    
};

#endif // TRAIN_H
