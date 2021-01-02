//Ciccone Davide

#ifndef TRAIN_H
#define TRAIN_H

#include "station.h"

#include <list>

class Train
{
public:
    
    virtual int getMaxSpeed() = 0;
    virtual int getMaxWait() = 0;
    virtual int getId() = 0;
    virtual int getVerse() = 0;
    virtual int getType() = 0;
    virtual std::list<int> getTimes() = 0;
    virtual int getSpeed() = 0;
    virtual int getPosition() = 0;
    virtual Station getCurrentStation() = 0;
    virtual Station getNextStation() = 0;
    virtual void setWaitTime(int waitTime) = 0;
    virtual int getWaitTime() = 0;
    virtual void setTrack(int trackNumber) = 0;
    virtual int getTrack() = 0;
    
    
};

#endif // TRAIN_H
