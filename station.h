#ifndef station_h
#define station_h

#include <string>
#include <vector>

#include "train.h"

class Station {
	virtual std::string SendMsg(const Train& t, int time) = 0;
	virtual std::string GetName() const = 0;
	virtual int GetType() const = 0;
	virtual int GetDistnace() const = 0;
	virtual std::vector<Train> GetTrainsAhead() const = 0;

	virtual void PrintDepartureTime(const Train& t, int time) const = 0;
	virtual void PrintArrivalTime(const Train& t, int time) const = 0;
};

#endif // !station_h
