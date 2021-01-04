// Stojna Lorei

#ifndef local_station_h
#define local_station_h

#include "station.h"

class LocalStation : public Station{
public:
	//LocalStation() = delete; /*Forse*/
	LocalStation(std::string name, int type, int distance, const ReadFile&);

	/*Forse*/
	LocalStation(const LocalStation& s) = default;
	LocalStation(LocalStation && s) = default;

	const LocalStation& operator=(const LocalStation & s) = delete;
	const LocalStation& operator=(const LocalStation && s) = delete;
	////////

	// Possono esserci treni di solo transito
	// Viene chiamata dal treno quando si trova a 20km dalla stazione
	// Ritorna una stringa contenente le infomrazioni sul binario e 
	// sul tempo d'attesa tipo: "1, 5" -> binario uno e 5 minuti di attesa in parcheggio
	// Se l'attesa è 0 allora il treno non deve fermarsi in parcheggio
	void ArrivalRequest(Train& t) override;
	bool DepartureRequest(Train& t) override;

	int GetNextTrain(const Train& t) const override;

	void Update() override;

	std::string GetName() const override;
	int GetType() const override;
	int GetDistance() const override;
	std::list<Train> GetTrainsAhead(int verse) const override;

	void PrintDepartureTime(const Train& t, int time) const override;
	void PrintArrivalTime(const Train& t, int time) const override;

private:
	void AddParkedTrain(Train& t);
	bool ComparePriority(const Train& t1, const Train& t2) const;
	int GetEstimatedArrivalTime(const Train& t) const;
	std::vector<int> TrackStatus(const Train& t) const;

private:
	std::string name_;
	int type_;
	int distance_;

	int position_;
	const ReadFile& read_file_;
	// Array di interi di dimensione fissa con:
	// PRIMA e SECONDA cella contengono il tempo in minuti che mancano alla 
	// partenza per i treni che si trovano ai binari 1 e 2. 
	// Queste due celle indicano che i banari sono BINARI STANDARD con 
	// VERSO "0"
	//
	// SECONDA e TERZA cella uguale a prime due ma per il verso opposto
	// 
	// QUINTA cella (stato del binario di transito), VERSO 0
	//
	// SESTA cella, come la quinta ma per il verso opposto
	int tracks_state_ [6];
	// Lista dei treni che sono in stazione
	std::list<Train> trains_in_station_;

	std::list<Train> trains_ahead_east_;
	std::list<Train> trains_ahead_weast_;
	// Lista dei treni parcheggiati verso 0 ->
	std::vector<Train> parked_trains_east_;
	// Lista dei treni parcheggiati verso 1 <-
	std::vector<Train> parked_trains_weast_;

	/*
	Variabili private che forse servono:

	*/
};

bool operator==(const LocalStation& s_one, const LocalStation& s_two);

///////////////
int TimeToFree(const std::list<Train>& t, const MainStation& s);
std::string FormatTime(int n);
///////////////
#endif // !local_station_h
