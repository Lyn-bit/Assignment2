//Ciccone Davide

#ifndef TRAIN_H
#define TRAIN_H

#include "station.h"
#include "ReadFile.h"
#include <list>

class Train
{
public:
    
    virtual int getMaxSpeed() const = 0; //metodo che ritorna la velocità massima
    virtual int getMaxWait() const = 0; //metodo che ritorna il tempo d'attesa massimo
    virtual int getId() const = 0; //metodo che ritorna l'id del treno
    virtual int getVerse() const = 0; //metodo che ritorna il verso del treno
    virtual int getType() const = 0; //metodo che ritorna il tipo di treno
    virtual std::list<int> getTimes() const = 0; //metodo che ritorna la lista degli orari
    virtual int getSpeed() const = 0; //metodo che ritorna la velocità del treno
    virtual int getPosition() const = 0; //metodo che ritorna la posizione del treno
    
    //metodo che ritorna la stazione in cui il treno si trova al momento
    virtual Station getCurrentStation() const = 0;
    
    //metodo che ritorna la prossima stazione in cui il treno dovrà andare
    virtual Station getNextStation() = 0;
    
    //metodo che imposta il tempo di attesa del treno
    virtual void SetWaitTime(int waitTime) = 0;
    //metodo che ritorna il tempo di attesa del treno
    virtual int GetTimeLeft() const = 0;
    
    //metodo che ritorna lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
    virtual std::string getState() const = 0;
    
    //metodo che imposta il binario in cui il treno dovrà andare
    virtual void SetTrack(int trackNumber) = 0;
    //metodo che ritorna il binario in cui il treno dovrà andare
    virtual int GetTrack() const = 0;
    
    //metodo che invia una richiesta d'arrivo alla stazione
    virtual void SendArrivalRequest() = 0;
    
    //metodo che invia una richiesta di partenza alla stazione
    virtual void SendDepartureRequest() = 0;
    
    
    virtual void resize_timeList() = 0;
    
    //metodo che aggiorna le variabili che cambiano con il tempo
    virtual void update() = 0;
    
};

#endif // TRAIN_H
