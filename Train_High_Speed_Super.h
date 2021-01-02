//Ciccone Davide

#ifndef TRAIN_HIGH_SPEED_SUPER_H
#define TRAIN_HIGH_SPEED_SUPER_H

#include "Train.h"

#include <list>

#include "Station.h"

class Train_High_Speed_Super
{
public:
    Train_High_Speed_Super(int numero, int verso, int direzione, std::list<int> orari);
    ~Train_High_Speed_Super();
    
    int getMaxSpeed() override;
    int getMaxWait() override;
    int getId();
    int getVerse();
    int getType();
    std::list<int> getTimes();
    int getSpeed();
    int getPosition();
    Station getCurrentStation() override;
    Station getNextStation() override;
    void setWaitTime(int waitTime) override;
    int getWaitTime() override;
    void setTrack(int trackNumber) override;
    int getTrack() override;
    
private:
    int id;
    int verse;
    int type;
    std::list<int> times;
    int speed;
    int position;
    const int MAX_SPEED = 300;
    const int MAX_WAIT = 5;
};

#endif // TRAIN_HIGH_SPEED_SUPER_H
