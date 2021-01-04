// Stojna Lorei

#ifndef station_h
#define station_h

#include <string>
#include <vector>
#include <list>
#include <math.h>
#include <iostream>

#include "Train.h"
#include "ReadFile.h"

class Station 
{
public:
	virtual void ArrivalRequest(Train& t) = 0;
	virtual bool DepartureRequest(Train& t) = 0;
	virtual void Update() = 0;
	virtual int GetNextTrain(const Train& t) const = 0;
	virtual std::string GetName() const = 0;
	virtual int GetType() const = 0;
	virtual int GetDistance() const = 0;
	virtual std::list<const Train&> GetTrainsAhead(int verse) const = 0;

	virtual void PrintDepartureTime(const Train& t, int time) const = 0;
	virtual void PrintArrivalTime(const Train& t, int time) const = 0;
	// Da rivedere
	bool operator==(const Station& s) { return s.GetName() == this->GetName(); }
};

#endif // !station_h
