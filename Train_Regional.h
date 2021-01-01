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
    const int MAX_SPEED = 160;
    
    
};

#endif // TRAIN_REGIONAL_H
