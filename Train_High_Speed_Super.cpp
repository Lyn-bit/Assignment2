//Ciccone Davide

#include "Train_High_Speed_Super.h"
#include <list>
#include "station.h"
#include "ReadFile.h"
#include <utility>

using namespace std;

Train_High_Speed_Super::Train_High_Speed_Super(int numero, int tipo, int direzione, list<int> orari, const ReadFile& temp)
{
    this->id = numero;
    this->verse = direzione;
    this->type = tipo;
    this->times = orari;
    this->file = temp;
    
    if(verse == 0)
    {
        this->Current = file->get_First_Station();
        this->position = Current->GetDistance();
    }
    else if(verse == 1)
    {
        this->Current = file->get_Last_Station();
        this->position = Current->GetDistance();
    }
}

//metodo che ritorna la velocità massima del treno
int Train_High_Speed_Super::getMaxSpeed() const
{
    return MAX_SPEED;
}

//metodo che ritorna il tempo d'attesa massimo del treno
int Train_High_Speed_Super::getMaxWait() const
{
    return MAX_WAIT;
}

//metodo che ritorna l'id del treno
int Train_High_Speed_Super::getId() const
{
    return id;
}

//metodo che ritorna il verso del treno
int Train_High_Speed_Super::getVerse() const
{
    return verse;
}

//metodo che ritorna il tipo di treno
int Train_High_Speed_Super::getType() const
{
    return type;
}

//metodo che ritorna la lista degli orari del treno
list<int> Train_High_Speed_Super::getTimes() const
{
    return times;
}

//metodo che controlla se c'è un ritardo nel tempo
bool Train_High_Speed_Super::checkDelay()
{
    if(delay == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//metodo che ritorna un eventuale tempo di ritardo
int Train_High_Speed_Super::getDelay() const
{
    return delay;
}

//metodo che ritorna la velocità del treno
int Train_High_Speed_Super::getSpeed() const
{
    return speed;
}

//metodo che ritorna la posizione del treno
int Train_High_Speed_Super::getPosition() const
{
    return position;
}

//metodo che ritorna la stazione in cui il treno si trova al momento
Station* Train_High_Speed_Super::getCurrentStation() const
{
    return Current;
}
    
//metodo che ritorna la prossima stazione in cui il treno dovrà andare
//ritorna nullptr se non c'è una prossima stazione
Station* Train_High_Speed_Super::getNextStation()
{
    if(verse = 0)
    {
        if(File->nextStation(Current) == nullptr)
        {
            return nullptr;
            
        }
        else
        {
            return File->nextStation(Current);
        }
    }
    else
    {
        if(File->prevStation(Current) == nullptr)
        {
            return nullptr;
        }
        else
        {
            return File->prevStation(Current);
        }
    }
}

//metodo che imposta il tempo di attesa del treno
void Train_High_Speed_Super::SetWaitTime(int waitTime)
{
    this->wait = waitTime;
}

//metodo che ritorna il tempo di attesa del treno
int Train_High_Speed_Super::GetWaitTime() const
{
    return wait;
}

//metodo che imposta lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
void Train_High_Speed_Super::setState(string status){
    this->state = status;
}

//metodo che ritorna lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
string Train_High_Speed_Super::getState() const
{
    return state;
}

//metodo che imposta il binario in cui il treno dovrà andare
void Train_High_Speed_Super::SetTrack(int trackNumber)
{
    this->track = trackNumber;
}

//metodo che ritorna il binario in cui il treno dovrà andare
int Train_High_Speed_Super::GetTrack() const
{
    return track;
}

//metodo che manda una richiesta d'arrivo alla stazione
pair<int, Station*> Train_High_Speed_Super::SendArrivalRequest()
{
    Station* temp = getNextStation();
    pair<int, Station*> request {0, temp};
    return request;
}

//metodo che manda una richiesta di partenza alla stazione
pair<int, Station*> Train_High_Speed_Super::SendDepartureRequest()
{
    Station* temp = getNextStation();
    pair<int, Station*> request {1, temp};
    return request;
}

//controllo se ci sono treni davanti
bool Train_High_Speed_Super::checkTrainAhead()
{
    if(Current->getNextTrain(const this) == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//treno che si trova davanti
Train* Train_High_Speed_Super::getTrainAhead() const
{
    if(checkTrainAhead()) //sistema di controllo per capire se ha un treno davanti
    {
        list<Train*> temp = Current->GetTrainsAhead(verse);
        Train* NextTrain = temp.back();
        return NextTrain;
    }
    else
    {
        return nullptr;
    }
}

//metodo che aggiorna le variabili che cambiano con il tempo
void Train_High_Speed_Super::update()
{
    Train* TrainTemp = getTrainAhead();
    Station* StationTemp = getNextStation();
    
    //aggiorno il tempo globale
    GlobalTime++;
    
    //aggiorno posizione
    if(verse == 0)
    {
        position += round(speed * 60);
    }
    else
    {
        position -= round(speed * 60);
    }
    
    //aggiorno velocità
    if( ( (StationTemp->GetDistance() - position) <= 20 && verse == 0) || ( (position - StationTemp->GetDistance()) <= 20 && verse == 1) ) //controllo se il treno si trova a 20km dalla stazione
    {
        if((track == nullptr) && (state != "p")) //se il treno non ha un binario e non è nel parcheggio allora mando una richiesta di arrivo
        {
            SendArrivalRequest(); //manda la richiesta di arrivo alla stazione
            if(!checkTrainAhead())//se non ha un treno davanti
            {
                if( ( (StationTemp->GetDistance() - position) <= 5 && verse == 0) || ( (position - StationTemp->GetDistance()) <= 5 && verse == 1)  ) //controllo se il treno si trova a 5km dalla stazione
                {
                    if(state == "p") //se deve andare nel parcheggio, il treno si ferma perchè il parcheggio si trova a 5km dalla stazione
                    {
                        speed = 0;
                    }
                    else //altrimenti può andare ad 80km, velocità massima a 5 km dalla stazione
                    {
                        speed = 80;
                    }
                }
                else
                {
                    speed = MAX_SPEED; //altrimenti può andare alla velocità massima
                }
            }
            else //se ha un treno davanti
            {
                if(TrainTemp->getState() == "s")//controlla lo stato del treno davanti
                {
                    if(state == "s")
                    {
                        TrainTemp->getSpeed();//se ha un treno davanti vanno alla stessa velocità
                    }
                    else //se lo stato del treno è p
                    {
                        if(!( ( (StationTemp->GetDistance() - position) <= 5 && verse == 0) || ( (position - StationTemp->GetDistance()) <= 5 && verse == 1) ))//se non si trova a 5 km dalla stazione
                        {
                            TrainTemp->getSpeed();//se ha un treno davanti vanno alla stessa velocità
                        }
                        else //se si trova a 5km dalla stazione fermo e parcheggio il treno
                        {
                            speed = 0; 
                        }
                    }
                }
                else if(TrainTemp->getState() == "p") //se il treno davanti sta andando al parcheggio, controllo lo stato del treno attuale
                {
                    if(state = "p") //se sta andando al parcheggio
                    {
                        if(TrainTemp->getSpeed() == 0) //controllo se la velocità del treno davanti è zero
                        {
                            speed = MAX_SPEED; //se la velocità del treno davanti è zero, vuol dire che è nel parcheggio
                        }
                        else
                        {
                            TrainTemp->getSpeed();//altrimenti possono andare alla stessa velocità
                        }
                    }
                }
            }
        }
        else if(state == "s") //se il treno ha già mandato una richiesta di arrivo controllo se sta andando in stazione
        {
            if( ( (StationTemp->GetDistance() - position) <= 5 && verse == 0) || ( (position - StationTemp->GetDistance()) <= 5 && verse == 1) )//se si trova a 5 km dalla stazione
            {
                speed = 80;
            }
            else if(checkTrainAhead()) //se ha un treno davanti controlla lo stato
            {
                if(TrainTemp->getState() == "s") //se sta andando in stazione
                {
                    speed = TrainTemp->getSpeed();
                }
                else //se il treno davanti sta andando nel parcheggio
                {
                    if(TrainTemp->getSpeed() == 0) //se il treno davanti è nel parcheggio allora questo può andare alla velocità massima
                    {
                        speed = MAX_SPEED;
                    }
                    else
                    {
                        speed = TrainTemp->getSpeed();
                    }
                }
            }
        }
    }
    else if(checkTrainAhead())//se non si trova a 20km dalla stazione controlla se ha un treno davanti
    {
        speed = TrainTemp->getSpeed(); //altrimenti viaggia alla stessa velocità del treno
    }
    else if(!checkTrainAhead())//se non ha un treno davanti allora può andare alla velocità massima
    {
        speed = MAX_SPEED;
    }
    else if(StationTemp->GetDistance() == position) //se il treno arriva alla stazione, allora si ferma
    {
        speed = 0;
    }
    
    //aggiorno tempo
    if(wait > 0)//controllo se c'è del tempo da aggiornare
    {
        wait -= 1;
        if(wait == 0) //se il tempo d'attesa è finito
        {
            if(state == "s") //controlla lo stato
            {
                SendDepartureRequest();//se è in stazione manda una richiesta di partenza
            }
            else
            {
                SendArrivalRequest();//se è nel parcheggio manda una richiesta di arrivo
            }
        }
    }
    
    //aggiorno stazione e orari
    if( ( (StationTemp->GetDistance() < position) && verse == 0) || ( (position < StationTemp->GetDistance()) && verse == 1) ) //se il treno ha superato la stazione allora aggiorno la stazione
    {
        Current = getNextStation(); //aggiorno stazione
        int PlannedTime = times.front();
        if(PlannedTime == GlobalTime) //se il tempo di arrivo effettivo non corrisponde a quello previsto allora aggiorno il ritardo
        {
            int temp = GlobalTime - PlannedTime;
            delay += temp;
        }
        times.pop_front(); //rimuovo l'orario della stazione appena superata
        times.front() += delay; //aggiorno l'orario di arrivo stimato alla prossima stazione aggiungendo il ritardo
    }
    
}


