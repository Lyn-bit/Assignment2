//Ciccone Davide

#ifndef TRAIN_REGIONAL_H
#define TRAIN_REGIONAL_H

#include "Train.h"

#include <list>

#include "Station.h"

class Train_Regional : public Train
{
public:
    Train_Regional(int numero, int verso, int direzione, std::list<int> orari);
    ~Train_Regional();
    
    int getMaxSpeed() override;
    int getMaxWait() override;
    int getId() override;
    int getVerse() override;
    int getType() override;
    std::list<int> getTimes() override;
    int getSpeed() override;
    int getPosition() override;
    
    Station getCurrentStation() override;
    Station getNextStation() override;
    void setWaitTime(int waitTime) override;
    int getWaitTime() override;
    void setTrack(int trackNumber) override;
    int getTrack() override;
    void sendArrivalRequest() override;
    void sendDepartureRequest() override;
    
private:
    int id;
    int verse;
    int type;
    std::list<int> times;
    int speed;
    int position;
    const int MAX_SPEED = 160;
    const int MAX_WAIT = 20;
    
};

#endif // TRAIN_REGIONAL_H
