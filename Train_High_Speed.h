//Ciccone Davide

#ifndef TRAIN_HIGH_SPEED_H
#define TRAIN_HIGH_SPEED_H

#include "Train.h"

#include <list>

#include "Station.h"

class Train_High_Speed
{
public:
    Train_High_Speed(int numero, int verso, int direzione, std::list<int> orari);
    ~Train_High_Speed();
    
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
    
private:
    int id;
    int verse;
    int type;
    std::list<int> times;
    int speed;
    int position;
    const int MAX_SPEED = 240;
    const int MAX_WAIT = 15;
};

#endif // TRAIN_HIGH_SPEED_H
