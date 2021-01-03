//Ciccone Davide

#include "Train_Regional.h"
#include <list>
#include "station.h"

using namespace std;

Train_Regional::Train_Regional(int numero, int tipo, int direzione, std::list<int> orari)
{
    id = numero;
    verse = direzione;
    type = tipo;
    times = orari;
}

//metodo che ritorna la velocità massima del treno
int Train_Regional::getMaxSpeed()
{
    return MAX_SPEED;
}

//metodo che ritorna il tempo d'attesa massimo del treno
int Train_Regional::getMaxWait()
{
    return MAX_WAIT;
}

//metodo che ritorna l'id del treno
int Train_Regional::getId()
{
    return id;
}

//metodo che ritorna il verso del treno
int Train_Regional::getVerse()
{
    return verse;
}

//metodo che ritorna il tipo di treno
int Train_Regional::getType()
{
    return type;
}

//metodo che ritorna la lista degli orari del treno
list<int> Train_Regional::getTimes()
{
    return times;
}

//metodo che ritorna la velocità del treno
int Train_Regional::getSpeed()
{
    return speed;
}

//metodo che ritorna la posizione del treno
int Train_Regional::getPosition()
{
    return position;
}

//metodo che ritorna la stazione in cui il treno si trova al momento
string Train_Regional::getCurrentStation() //DA FARE
{
    return "";
}
    
//metodo che ritorna la prossima stazione in cui il treno dovrà andare
string Train_Regional::getNextStation() //DA FARE
{
    return "";
}

//metodo che imposta il tempo di attesa del treno
void Train_Regional::SetWaitTime(int waitTime)
{
    wait = waitTime;
}

//metodo che ritorna il tempo di attesa del treno
int Train_Regional::GetWaitTime()
{
    return wait;
}

//metodo che imposta il binario in cui il treno dovrà andare
void Train_Regional::SetTrack(int trackNumber)
{
    track = trackNumber;
}

//metodo che ritorna il binario in cui il treno dovrà andare
int Train_Regional::GetTrack()
{
    return track;
}

//metodo che manda una richiesta d'arrivo alla stazione
void Train_Regional::SendArrivalRequest() //DA FARE
{
    
}

//metodo che manda una richiesta di partenza alla stazione
void Train_Regional::SendDepartureRequest() //DA FARE
{
    
}

void Train_Regional::resize_timeList() //DA FARE
{
    
}