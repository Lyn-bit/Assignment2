//Ciccone Davide

#include "train.h"

using namespace std;

Train_High_Speed::Train_High_Speed(int numero, int direzione, int tipo, std::list<int> orari, const ReadFile *temp) : id{numero}, verse{direzione}, type{tipo}, times{orari}, speed{0}, position{0}, wait{5}, track{-1}, Current{nullptr}, file{temp}, state{"s"}, GlobalTime{0}, delay{0}, TimePassed{0}, firstTime{times.front()}
{
}

//metodo che ritorna la velocità massima del treno
int Train_High_Speed::getMaxSpeed() const
{
    return MAX_SPEED;
}

//metodo che ritorna il tempo d'attesa massimo del treno
int Train_High_Speed::getMaxWait() const
{
    return MAX_WAIT;
}

//metodo che ritorna l'id del treno
int Train_High_Speed::getId() const
{
    return id;
}

//metodo che ritorna il verso del treno
int Train_High_Speed::getVerse() const
{
    return verse;
}

//metodo che ritorna il tipo di treno
int Train_High_Speed::getType() const
{
    return type;
}

//metodo che ritorna la lista degli orari del treno
list<int> Train_High_Speed::getTimes() const
{
    return times;
}

//metodo che controlla se c'è un ritardo nel tempo
bool Train_High_Speed::checkDelay()
{
    if (delay == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//metodo che ritorna un eventuale tempo di ritardo
int Train_High_Speed::getDelay() const
{
    return delay;
}

//metodo che ritorna la velocità del treno
int Train_High_Speed::getSpeed() const
{
    return speed;
}

//metodo che ritorna la posizione del treno
int Train_High_Speed::getPosition() const
{
    return position;
}

//metodo che ritorna la stazione in cui il treno si trova al momento
const Station *Train_High_Speed::getCurrentStation() const
{
    return Current;
}

//metodo che ritorna la prossima stazione in cui il treno dovrà andare
//ritorna nullptr se non c'è una prossima stazione
const Station *Train_High_Speed::getNextStation()
{
    const Station *temp = file->get_Last_Station();
    if (verse == 1 && position == 0 && Current == nullptr) //se il treno parte dal capolinea allora do alla posizione come valore la distanza del capolinea dalla prima stazione
    {
        position = temp->GetDistance();
    }
    if (Current == nullptr && ((verse == 0 && position <= 0) || (verse == 1 && position >= temp->GetDistance()))) //se il puntatore current è nullptr e il treno deve ancora partire allora inizializzo current e restituisco la prossima stazione
    {
        if (verse == 0)
        {
            Current = file->get_First_Station();
            return file->nextStation(Current);
        }
        else
        {
            Current = file->get_Last_Station();
            return file->prevStation(Current);
        }
    }

    else if (verse == 0) // altrimenti in base al verso restituisco la prossima stazione
    {
        /*
        if (file->nextStation(Current) == nullptr)
        {
            return nullptr;
        }
        */

        return file->nextStation(Current);
    }

    else
    {
        /*
        if (file->prevStation(Current) == nullptr)
        {
            return nullptr;
        }
        */

        return file->prevStation(Current);
    }
    return nullptr;
}

//metodo che imposta il tempo di attesa del treno
void Train_High_Speed::SetWaitTime(int waitTime)
{
    this->wait = waitTime;
}

//metodo che imposta lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
void Train_High_Speed_Super::setState(string status)
{
    this->state = status;
}

//metodo che ritorna il tempo di attesa del treno
int Train_High_Speed::GetTimeLeft() const
{
    return wait;
}

//metodo che ritorna il tempo che il treno ha aspettato
int Train_High_Speed::GetWaitTime() const
{
    return TimePassed;
}

//metodo che ritorna lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
string Train_High_Speed::getState() const
{
    return state;
}

//metodo che imposta il binario in cui il treno dovrà andare
void Train_High_Speed::SetTrack(int trackNumber)
{
    this->track = trackNumber;
}

//metodo che ritorna il binario in cui il treno dovrà andare
int Train_High_Speed::GetTrack() const
{
    return track;
}

//metodo che manda una richiesta d'arrivo alla stazione
pair<int, const Station *> Train_High_Speed::SendArrivalRequest()
{
    const Station *temp = getNextStation();
    pair<int, const Station *> request{0, temp};
    return request;
}

//metodo che manda una richiesta di partenza alla stazione
pair<int, const Station *> Train_High_Speed::SendDepartureRequest()
{
    const Station *temp = getNextStation();
    pair<int, const Station *> request{1, temp};
    return request;
}

//controllo se ci sono treni davanti
bool Train_High_Speed::checkTrainAhead() const
{
    if (Current->GetNextTrain(this) == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//treno che si trova davanti
const Train *Train_High_Speed::getTrainAhead() const
{
    if (checkTrainAhead()) //sistema di controllo per capire se ha un treno davanti
    {
        list<const Train *> temp = Current->GetTrainsAhead(verse);
        const Train *NextTrain = temp.back();
        return NextTrain;
    }
    else
    {
        return nullptr;
    }
}

//metodo che ritorna il tempo globale
int Train_High_Speed::getTime()
{
    return GlobalTime;
}

//metodo che aggiorna le variabili che cambiano con il tempo
pair<int, const Station *> Train_High_Speed::update()
{
    const Station *StationTemp = getNextStation();
    const Train *TrainTemp = getTrainAhead();
    //aggiorno il tempo globale
    GlobalTime++;

    //aggiorno posizione
    if (verse == 0)
    {
        position += round(speed / 60);
    }
    else if (verse == 1 && position == 0)
    {
        position = Current->GetDistance();
    }
    else
    {
        position -= round(speed / 60);
    }

    //aggiorno stazione e orari
    if (((StationTemp->GetDistance() < position) && verse == 0) || ((position < StationTemp->GetDistance()) && verse == 1)) //se il treno ha superato la stazione allora aggiorno la stazione
    {
        Current = getNextStation(); //aggiorno stazione
        int PlannedTime = times.front();
        if (PlannedTime == GlobalTime) //se il tempo di arrivo effettivo non corrisponde a quello previsto allora aggiorno il ritardo
        {
            int temp = GlobalTime - PlannedTime;
            delay += temp;
        }
        times.pop_front();      //rimuovo l'orario della stazione appena superata
        times.front() += delay; //aggiorno l'orario di arrivo stimato alla prossima stazione aggiungendo il ritardo
    }

    //aggiorno velocità
    if (GlobalTime >= firstTime)
    {
        if ((((StationTemp->GetDistance() - position) <= 20 && verse == 0) || ((position - StationTemp->GetDistance()) <= 20 && verse == 1)) && position != StationTemp->GetDistance()) //controllo se il treno si trova a 20km dalla stazione
        {
            if ((state != "p") && state != "s") //se il treno non ha un binario e non è nel parcheggio allora mando una richiesta di arrivo
            {

                if (!checkTrainAhead()) //se non ha un treno davanti
                {
                    if (((StationTemp->GetDistance() - position) <= 5 && verse == 0) || ((position - StationTemp->GetDistance()) <= 5 && verse == 1)) //controllo se il treno si trova a 5km dalla stazione
                    {
                        speed = 80;
                    }
                    else
                    {
                        speed = MAX_SPEED; //altrimenti può andare alla velocità massima
                    }
                }
                else //se ha un treno davanti
                {
                    if (TrainTemp->getState() == "s") //controlla lo stato del treno davanti
                    {
                        if ((!((StationTemp->GetDistance() - position) <= 5) && verse == 0) || !((position - StationTemp->GetDistance()) <= 5) && verse == 1) //se non si trova a 5 km dalla stazione
                        {
                            if (TrainTemp->getSpeed() >= MAX_SPEED)

                            {
                                speed = MAX_SPEED; //se vanno più veloci della velocità massima
                            }
                            else
                            {
                                TrainTemp->getSpeed(); //altrimenti possono andare alla stessa velocità
                            }
                        }
                        else //se si trova a 5km dalla stazione fermo e parcheggio il treno
                        {
                            speed = 0;
                        }
                    }
                    else if (TrainTemp->getState() == "p") //se il treno davanti sta andando al parcheggio, controllo lo stato del treno attuale
                    {
                        if (state == "p") //se sta andando al parcheggio
                        {
                            if (TrainTemp->getSpeed() == 0) //controllo se la velocità del treno davanti è zero
                            {
                                speed = MAX_SPEED; //se la velocità del treno davanti è zero, vuol dire che è nel parcheggio
                            }
                            else
                            {
                                if (TrainTemp->getSpeed() >= MAX_SPEED)
                                {
                                    speed = MAX_SPEED; //se vanno più veloci della velocità massima
                                }
                                else
                                {
                                    TrainTemp->getSpeed(); //altrimenti possono andare alla stessa velocità
                                }
                            }
                        }
                    }
                }
                return SendArrivalRequest(); //manda la richiesta di arrivo alla stazione
            }
            else if (state == "s") //se il treno ha già mandato una richiesta di arrivo controllo se sta andando in stazione
            {
                if (((StationTemp->GetDistance() - position) <= 5 && verse == 0) || ((position - StationTemp->GetDistance()) <= 5 && verse == 1)) //se si trova a 5 km dalla stazione
                {
                    speed = 80;
                }
                else if (checkTrainAhead()) //se ha un treno davanti controlla lo stato
                {
                    if (TrainTemp->getState() == "s") //se sta andando in stazione
                    {
                        if (TrainTemp->getSpeed() > MAX_SPEED) //se la velocità del treno davanti è maggiore della velocità massima di questo treno
                        {
                            speed = MAX_SPEED;
                        }
                        else
                        {
                            speed = TrainTemp->getSpeed();
                        }
                    }
                    else //se il treno davanti sta andando nel parcheggio
                    {
                        if (TrainTemp->getSpeed() == 0) //se il treno davanti è nel parcheggio allora questo può andare alla velocità massima
                        {
                            speed = MAX_SPEED;
                        }
                        else if (TrainTemp->getSpeed() > MAX_SPEED)
                        {
                            speed = TrainTemp->getSpeed();
                        }
                        else
                        {
                            speed = MAX_SPEED;
                        }
                    }
                }
            }
            else if (state == "p" && abs(position - StationTemp->GetDistance()) <= 5)
            {
                speed = 0;
            }
        }
        else if (checkTrainAhead() && state == "v") //se non si trova a 20km dalla stazione controlla se ha un treno davanti
        {
            if (TrainTemp->getSpeed() > MAX_SPEED) //se il treno davanti è più veloce della velocità massima allora questo treno viaggia alla sua velocità massima
            {
                speed = MAX_SPEED;
            }
            else
            {
                speed = TrainTemp->getSpeed(); //altrimenti viaggia alla stessa velocità del treno
            }
        }
        else if (!checkTrainAhead() && state == "v") //se non ha un treno davanti allora può andare alla velocità massima
        {
            speed = MAX_SPEED;
        }
        else if (StationTemp->GetDistance() == position) //se il treno arriva alla stazione, allora si ferma
        {
            if (wait > 0)
                speed = 0;
            //wait += 5;
        }
    }
    //aggiorno tempo
    if (GlobalTime >= firstTime)
    {
        if (wait > 0) //controllo se c'è del tempo da aggiornare
        {
            wait -= 1;
            TimePassed++;
            if (wait == 0) //se il tempo d'attesa è finito
            {
                return SendDepartureRequest();
            }
        }
        if (wait == 0 && TimePassed > 0) //se il tempo d'attesa è zero azzero TimePassed
        {
            TimePassed = 0;
        }
    }
    pair<int, const Station *> p = {-1, nullptr};
    return p;
}
