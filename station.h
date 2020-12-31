#ifndef station_h
#define station_h

#include <string>

#include "train.h"

class Station {
	Station() = delete; //Forse
	virtual std::string SendMsg(Train t, int time) = 0;
	virtual std::string GetName() const = 0;
	virtual std::string GetType() const = 0;
	virtual std::string GetDistnace() const = 0;
	virtual std::string GetTrainsAhead() const = 0;

	virtual void PrintDepartureTime(Train t, int time) const = 0;
	virtual void PrintArrivalTime(Train t, int time) const = 0;
};

#endif // !station_h
