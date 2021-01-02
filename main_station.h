// Stojna Lorei

#ifndef main_station_h
#define main_staion_h

#include "station.h"

#include <utility>
#include <algorithm>

#include "train.h"

class MainStation : public Station {
public:
	MainStation(std::string name, int type, int distance);

	// Viene chiamata dal treno quando si trova a 20km dalla stazione
	// Imposta direttamente le infomrazioni sul binario e 
	// sul tempo d'attesa
	void ArrivalRequest(Train& t) override;
	// Richiesta fatta dal treno alla stazione per capire se può partire o meno
	// quando si trova nei parcheggi o in stazione
	// in caso il treno non possa ancora partire aggiunge il ritardo a se
	// se stesso e ai treni che devono arrivare sul suo binario
	bool DepartureRequest(Train& t) override;
	
	std::string GetName() const override { return name_; }
	int GetType() const override { return type_; }
	int GetDistnace() const override { return distance_; }
	
	// Ritorna una lista dei treni che si trovano tra questa stazione 
	// e la succesiva/precedente in base al verso
	std::list<Train> GetTrainsAhead(int verse) const override
	{
		if (verse == 0) { return trains_ahead_weast_; }
		else { return trains_ahead_east_; }
	}

	/*da fare*/
	void PrintDepartureTime(const Train& t, int time) const override;
	void PrintArrivalTime(const Train& t, int time) const override;

private:
	void AddParkedTrain(Train& t);
	bool ComparePriority(const Train& t1, const Train& t2) const;
	int GetEstimatedArrivalTime(const Train& t) const;
	// Ritorna un array con le prime due posizioni i minuti d'attesa
	// (senza contare i 5 minuti che ogni treno deve stare fermo in stazione)
	// per poter usufurire del binario piu veloce e il numero di binario
	// e le ultime 2 di quello più lento
	std::vector<int> TrackStatus(const Train& t) const;

	
	/*
	Funzione che riceve il contatore e aggiorna ogni minuti le variabili
	da aggiornare (tipo tracks_state_, trains_ahead_east/weast_)
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
	// 20km prima della stazione successiva/precedente
	// l'ultimo treno nella lista è quello più vicino alla stazione
	// east <-
	// weast ->
	std::list<Train> trains_ahead_east_;
	std::list<Train> trains_ahead_weast_;


	//Sono strutturate in modo da avere un treno con binario 0 e 1 alternati
	//
	// Lista dei treni parcheggiati verso 0 ->
	std::vector<Train> parked_trains_east_;
	// Lista dei treni parcheggiati verso 1 <-
	std::vector<Train> parked_trains_weast_;
};

// Operatore di uguaglianza
bool operator==(const MainStation& s_one, const MainStation& s_two);

// Ritrona i minuti, del binario con verso in base alla lista "t",
// che mancano che il treno che c'è davanti sia a più di 10km
int TimeToFree(const std::list<Train>& t, const MainStation& s);

#endif // !main_station_h

