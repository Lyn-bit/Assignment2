// Stojna Lorei

#ifndef station_h
#define station_h

#include <string>
#include <vector>
#include <list>
#include <math.h>

#include "train.h"

//#include "Controll.h"  //debug

class Station 
{
protected:
	Station(std::string name, int type, int distance) :
		name_{ name }, type_{ type }, distance_{ distance }
	{
	}
private:
	virtual void ArrivalRequest(Train& t) = 0;
	virtual bool DepartureRequest(Train& t) = 0;
	virtual std::string GetName() const = 0;
	virtual int GetType() const = 0;
	virtual int GetDistance() const = 0;
	virtual void Update() = 0;
	virtual std::list<Train> GetTrainsAhead(int verse) const = 0;

	virtual void PrintDepartureTime(const Train& t, int time) const = 0;
	virtual void PrintArrivalTime(const Train& t, int time) const = 0;
private:
	std::string name_;
	int type_;
	int distance_;
};
/*
public:

	/*costruttori di copia e operatore assegnamento da mettere delete
	virtual void ArrivalRequest(Train& t) = 0;
	virtual bool DepartureRequest(Train& t) = 0;
	virtual void Update() = 0;
	virtual std::string GetName() const = 0;
	virtual int GetType() const = 0;
	virtual int GetDistance() const = 0;
	virtual std::list<Train> GetTrainsAhead(int verse) const = 0;

	virtual void PrintDepartureTime(const Train& t, int time) const = 0;
	virtual void PrintArrivalTime(const Train& t, int time) const = 0;
};
*/

#endif // !station_h
