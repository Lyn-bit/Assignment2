//Ciccone Davide

#ifndef TRAIN_REGIONAL_H
#define TRAIN_REGIONAL_HclTabCtrl

#include "Train.h"

#include <list>
#include <utility>

#include "station.h"
#include "ReadFile.h"

class Train_Regional : public Train
{
public:
    Train_Regional(int numero, int tipo, int direzione, std::list<int> orari, const ReadFile& temp);
    ~Train_Regional();
    
    int getMaxSpeed() const override; //metodo che ritorna la velocità massima del treno
    int getMaxWait() const override; //metodo che ritorna il tempo d'attesa massimo del treno
    int getId() const override; //metodo che ritorna l'id del treno
    int getVerse() const override; //metodo che ritorna il verso del treno
    int getType() const override; //metodo che ritorna il tipo di treno
    std::list<int> getTimes() const override; //metodo che ritorna la lista degli orari
    int getSpeed() const override; //metodo che ritorna la velocità del treno
    int getPosition() const override; //metodo che ritorna la posizione del treno
    
    //metodo che controlla se c'è un ritardo nel tempo
    bool checkDelay() override;
    //metodo che ritorna un eventuale tempo di ritardo
    int getDelay() const override;
    
    //metodo che ritorna la stazione in cui il treno si trova al momento
    Station* getCurrentStation() const override;
    
    //metodo che ritorna la prossima stazione in cui il treno dovrà andare
    Station* getNextStation() override;
    
    //metodo che imposta il tempo di attesa del treno
    void SetWaitTime(int waitTime) override;
    //metodo che ritorna il tempo di attesa del treno
    int GetWaitTime() const override;
    
    //metodo che imposta il binario in cui il treno dovrà andare
    void SetTrack(int trackNumber) override;
    //metodo che ritorna il binario in cui il treno dovrà andare
    int GetTrack() const override;
    
    //metodo che imposta lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
    void setState(std::string status) override;
    //metodo che ritorna lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
    std::string getState() const override;
    
    //metodo che manda una richiesta d'arrivo alla stazione
    std::pair<int, Station*> SendArrivalRequest() override;
    
    //metodo che manda una richiesta di partenza alla stazione
    std::pair<int, Station*> SendDepartureRequest() override;
    
    //controllo se ci sono treni davanti
    bool checkTrainAhead() override;
    
    //treno che si trova davanti
    Train* getTrainAhead() const override;
    
    //metodo che aggiorna le viariabili che cambiano con il tempo
    void update() override;
    
private:
    int id; 
    int verse;
    int type;
    std::list<int> times;
    int speed;
    int position;
    const int MAX_SPEED = 160;
    const int MAX_WAIT = 20;
    int wait;
    int track;
    Station* Current; //stazione in cui si trova al momento
    ReadFile* file; //oggetto per controllare la lista delle stazioni
    std::string state; //stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
    int GlobalTime = 0; //ora globale
    int delay = 0; //eventuale tempo di ritardo
};

#endif // TRAIN_REGIONAL_H
