//Ciccone Davide

#ifndef TRAIN_HIGH_SPEED_H
#define TRAIN_HIGH_SPEED_H

#include "Train.h"

#include <list>

#include "station.h"
#include "ReadFile.h"

class Train_High_Speed
{
public:
    Train_High_Speed(int numero, int verso, int direzione, std::list<int> orari);
    ~Train_High_Speed();
    
    int getMaxSpeed() const override; //metodo che ritorna la velocità massima del treno
    int getMaxWait() const override; //metodo che ritorna il tempo d'attesa massimo del treno
    int getId() const override; //metodo che ritorna l'id del treno
    int getVerse() const override; //metodo che ritorna il verso del treno
    int getType() const override; //metodo che ritorna il tipo di treno
    std::list<int> getTimes() const override; //metodo che ritorna la lista degli orari
    int getSpeed() const override; //metodo che ritorna la velocità del treno
    int getPosition() const override; //metodo che ritorna la posizione del treno
    
    //metodo che ritorna la stazione in cui il treno si trova al momento
    Station getCurrentStation() const override;
    
    //metodo che ritorna la prossima stazione in cui il treno dovrà andare
    Station getNextStation() override;
    
    //metodo che imposta il tempo di attesa del treno
    void SetWaitTime(int waitTime) override;
    //metodo che ritorna il tempo di attesa del treno
    int GetTimeLeft() const override;
    
    //metodo che imposta il binario in cui il treno dovrà andare
    void SetTrack(int trackNumber) override;
    //metodo che ritorna il binario in cui il treno dovrà andare
    int GetTrack() const override;
    
    //metodo che ritorna lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
    std::string getState() const override;
    
    //metodo che manda una richiesta d'arrivo alla stazione
    void SendArrivalRequest() override;
    
    //metodo che manda una richiesta di partenza alla stazione
    void SendDepartureRequest() override;
    
    
    void resize_timeList() override;
    
    //metodo che aggiorna le viariabili che cambiano con il tempo
    void update() override;
    
private:
    int id;
    int verse;
    int type;
    std::list<int> times;
    int speed;
    int position;
    const int MAX_SPEED = 240;
    const int MAX_WAIT = 15;
    int wait;
    int track;
};

#endif // TRAIN_HIGH_SPEED_H
