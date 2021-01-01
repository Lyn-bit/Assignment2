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
    
    int getMaxSpeed() override
    {
        return MAX_SPEED;
    }
    Station getCurrentStation() override;
    Station getNextStation() override;
    
private:
    int id;
    int verse;
    int type;
    std::list<int> times;
    int speed;
    int position;
    const int MAX_SPEED = 240;

};

#endif // TRAIN_HIGH_SPEED_H
