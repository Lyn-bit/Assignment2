//Ciccone Davide

#include "Train_Regional.h"
#include <list>
#include "station.h"
#include "ReadFile.h"

using namespace std;

Train_Regional::Train_Regional(int numero, int tipo, int direzione, std::list<int> orari)
{
    id = numero;
    verse = direzione;
    type = tipo;
    times = orari;
    
    if(verse = 0)
    {
        Current = File.get_First_Station();
        position = Current.GetDistance();
    }
    else if(verse = 1)
    {
        Current = File.get_Last_Station();
        position = Current.GetDistance();
    }
}

//metodo che ritorna la velocità massima del treno
int Train_Regional::getMaxSpeed() const
{
    return MAX_SPEED;
}

//metodo che ritorna il tempo d'attesa massimo del treno
int Train_Regional::getMaxWait() const
{
    return MAX_WAIT;
}

//metodo che ritorna l'id del treno
int Train_Regional::getId() const
{
    return id;
}

//metodo che ritorna il verso del treno
int Train_Regional::getVerse() const
{
    return verse;
}

//metodo che ritorna il tipo di treno
int Train_Regional::getType() const
{
    return type;
}

//metodo che ritorna la lista degli orari del treno
list<int> Train_Regional::getTimes() const
{
    return times;
}

//metodo che ritorna la velocità del treno
int Train_Regional::getSpeed() const
{
    return speed;
}

//metodo che ritorna la posizione del treno
int Train_Regional::getPosition() const
{
    return position;
}

//metodo che ritorna la stazione in cui il treno si trova al momento
Station Train_Regional::getCurrentStation() const
{
    return Current;
}
    
//metodo che ritorna la prossima stazione in cui il treno dovrà andare
//ritorna nullptr se non c'è una prossima stazione
Station Train_Regional::getNextStation()
{
    if(verse = 0)
    {
        if(File.nextStation(Current) == nullptr)
        {
            return nullptr;
            
        }
        else
        {
            return File.nextStation(Current);
        }
    }
    else
    {
        if(File.prevStation(Current) == nullptr)
        {
            return nullptr;
        }
        else
        {
            return File.prevStation(Current);
        }
    }
}

//metodo che imposta il tempo di attesa del treno
void Train_Regional::SetWaitTime(int waitTime)
{
    wait = waitTime;
}

//metodo che ritorna il tempo di attesa del treno
int Train_Regional::GetTimeLeft() const
{
    return wait;
}

//metodo che ritorna lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
string getState() const
{
    return state;
}

//metodo che imposta il binario in cui il treno dovrà andare
void Train_Regional::SetTrack(int trackNumber)
{
    track = trackNumber;
}

//metodo che ritorna il binario in cui il treno dovrà andare
int Train_Regional::GetTrack() const
{
    return track;
}

//metodo che manda una richiesta d'arrivo alla stazione
void Train_Regional::SendArrivalRequest() //DA FARE
{
    Station temp = getNextStation(); //NON COMPLETO
    temp.ArrivalRequest();
}

//metodo che manda una richiesta di partenza alla stazione
void Train_Regional::SendDepartureRequest() //DA FARE
{
    
}

void Train_Regional::resize_timeList() //DA FARE
{
    
}

//metodo che aggiorna le viariabili che cambiano con il tempo
void update()
{
    Station temp = getNextStation();
    //aggiorno posizione
    if(verse = 0)
    {
        position += speed;
    }
    else
    {
        position -= speed;
    }
    
    //aggiorno velocità
    if(verse = 0)
    {
        if((temp.GetDistance() - position) > MAX_SPEED)
        {
            speed = MAX_SPEED;
        }
        else if(((temp.GetDistance() - position) <= MAX_SPEED) && (temp.GetDistance() - position) > 20)
        {
            speed = (temp.GetDistance() - position) - 20;
        }
        else if((temp.GetDistance() - postion ) <= 20)
        {
            speed = 10;
            sendArrivalRequest();
        }
    }
    else if(verse = 1)
    {
        if((temp.GetDistance() - position) < -MAX_SPEED)
        {
            speed = MAX_SPEED;
        }
        else if(((temp.GetDistance() - position) >= -MAX_SPEED) && (temp.GetDistance() - position) < -20)
        {
            speed = -(temp.GetDistance() - position) - 20;
        }
        else if((temp.GetDistance() - postion ) >= -20)
        {
            speed = 10;
            sendArrivalRequest();
        }
    }
    
    //aggiorno stato
    if(speed = 0)
    {
        if(track = nullptr) //se il treno è fermo e non ha un binario assegnato, è in un parcheggio
        {
            state = "p";
        }
        else    //altrimenti è nella stazione
        {
            state = "s";
        }
    }
    else //se la velocità non è zero il treno è chiaramente in movimento
    {
        state = "v";
    }
}