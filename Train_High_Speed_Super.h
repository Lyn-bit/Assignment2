//Ciccone Davide

#ifndef TRAIN_HIGH_SPEED_SUPER_H
#define TRAIN_HIGH_SPEED_SUPER_H

#include "Train.h"

#include <list>

//#include "Station.h"

class Train_High_Speed_Super
{
public:
    Train_High_Speed_Super(int numero, int verso, int direzione, std::list<int> orari);
    ~Train_High_Speed_Super();
    
    int getMaxSpeed() override; //metodo che ritorna la velocità massima del treno
    int getMaxWait() override; //metodo che ritorna il tempo d'attesa massimo del treno
    int getId() override; //metodo che ritorna l'id del treno
    int getVerse() override; //metodo che ritorna il verso del treno
    int getType() override; //metodo che ritorna il tipo di treno
    std::list<int> getTimes() override; //metodo che ritorna la lista degli orari
    int getSpeed() override; //metodo che ritorna la velocità del treno
    int getPosition() override; //metodo che ritorna la posizione del treno
    
    //metodo che ritorna la stazione in cui il treno si trova al momento
    std::string getCurrentStation() override;
    
    //metodo che ritorna la prossima stazione in cui il treno dovrà andare
    std::string getNextStation() override;
    
    //metodo che imposta il tempo di attesa del treno
    void SetWaitTime(int waitTime) override;
    //metodo che ritorna il tempo di attesa del treno
    int GetWaitTime() override;
    
    //metodo che imposta il binario in cui il treno dovrà andare
    void SetTrack(int trackNumber) override;
    //metodo che ritorna il binario in cui il treno dovrà andare
    int GetTrack() override;
    
    //metodo che manda una richiesta d'arrivo alla stazione
    void SendArrivalRequest() override;
    
    //metodo che manda una richiesta di partenza alla stazione
    void SendDepartureRequest() override;
    
    
    void resize_timeList() override;
    
private:
    int id;
    int verse;
    int type;
    std::list<int> times;
    int speed;
    int position;
    const int MAX_SPEED = 300;
    const int MAX_WAIT = 5;
    int wait;
    int track;
};

#endif // TRAIN_HIGH_SPEED_SUPER_H
