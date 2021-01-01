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
    const int MAX_SPEED = 300;

};

#endif // TRAIN_HIGH_SPEED_SUPER_H
