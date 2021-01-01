//Ciccone Davide

#ifndef TRAIN_H
#define TRAIN_H

#include "station.h"

class Train
{
public:
    
    virtual int getMaxSpeed() = 0;
    virtual Station getCurrentStation() = 0;
    virtual Station getNextStation() = 0;
    
    
};

#endif // TRAIN_H
