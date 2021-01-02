#ifndef main_station_h
#define main_staion_h

#include "station.h"

#include <list>
#include <utility>
#include <algorithm>

#include "train.h"

class MainStation : public Station {
public:
	//MainStation() = delete; /*Forse*/
	MainStation(std::string name, int type, int distance);

	// Viene chiamata dal treno quando si trova a 20km dalla stazione
	// Ritorna una stringa contenente le infomrazioni sul binario e 
	// sul tempo d'attesa tipo: "1, 5" -> binario uno e 5 minuti di attesa in parcheggio
	// Se l'attesa è 0 allora il treno non deve fermarsi in parcheggio
	void SendMsg(Train& t) override;
	
	std::string GetName() const override;
	int GetType() const override;
	int GetDistnace() const override;
	std::vector<Train> GetTrainsAhead() const override;

	void PrintDepartureTime(const Train& t, int time) const override;
	void PrintArrivalTime(const Train& t, int time) const override;

private:
	void AddParkedTrain(Train& t);
	bool ComparePriority(const Train& t1, const Train& t2) const;
	int GetEstimatedArrivalTime(const Train& t) const;
	std::vector<int> TrackStatus(const Train& t) const;
	/*
	Manca la funzione che gestisce la cosa della stazione !!!!!!!!

	 ...
	 */

private:
	std::string name_;
	int type_;
	int distance_;

	// Array di interi di dimensione fissa con:
	// PRIMA e SECONDA cella contengono il tempo in minuti che mancano alla 
	// partenza per i treni che si trovano ai binari 1 e 2. 
	// Queste due celle indicano che i banari sono BINARI STANDARD con 
	// VERSO "0"
	//
	// SECONDA e TERZA cella uguale a prime due ma per il verso opposto
	int tracks_state_ [4];
	// Lista dei treni che sono in stazione
	std::list<Train> trains_in_station_;
	// Lista dei treni che si trovano dalla stazione ai
	// 20km prima della stazione successiva
	std::list<Train> trains_ahead_;

	//Sono strutturate in modo da aere un treno con binario 0 e 1 alternati
	//
	// Lista dei treni parcheggiati verso 0
	std::vector<Train> parked_trains_east_;
	// Lista dei treni parcheggiati verso 0
	std::vector<Train> parked_trains_weast_;
};

#endif // !main_station_h

