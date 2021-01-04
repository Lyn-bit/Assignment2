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

	/*Da aggiungere i treni di transito(appena riceve la richiesta a 20km in base all
	situazione la stazione sa quando il treno sarà arrivato)ccc*/
	std::list<Train> GetTrainsAhead(int verse) const override;
	int GetNextTrain(const Train& t) const override;

	void PrintDepartureTime(const Train& t, int time) const override;
	void PrintArrivalTime(const Train& t, int time) const override;

private:
	void AddParkedTrain(Train& t);
	void AddDelay(int delay, int verse);
	bool ComparePriority(const Train& t1, const Train& t2) const;
	int GetEstimatedArrivalTime(const Train& t) const;
	// Ritorna un array con le prime due posizioni i minuti d'attesa
	// (senza contare i 5 minuti che ogni treno deve stare fermo in stazione)
	// per poter usufurire del binario piu veloce e il numero di binario
	// e le ultime 2 di quello più lento
	// l'ultima cella contiene l'attesa in minuti per poter usare il 
	// binario di transito in base al verso del treno
	std::vector<int> TrackStatus(const Train& t) const;

	// calocla il tempo che il treno ci impiegha a superare la stazione di 10km
	// in base alla posizione
	int GetHighSpeedTrainTime(const Train& t) const;
	// In base al bonario trova il treno
	const Train& FindTrain(int track) const;

private:
	std::string name_;
	int type_;
	int distance_;
	// 1 se originie, 2 se capolinea, -1 per le altre
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
	// Lista dei treni che sono in stazione (non conta quelli di transito)
	std::list<Train&> trains_in_station_;

	// Lista dei treni che si trovano dalla stazione ai
	// 5km prima della stazione successiva/precedente
	// l'ultimo treno nella lista è quello più vicino alla stazione
	// east <-
	// weast ->
	std::list<const Train&> trains_ahead_east_;
	std::list<const Train&> trains_ahead_weast_;
	// Lista dei treni parcheggiati verso 0 ->
	std::vector<Train&> parked_trains_east_;
	// Lista dei treni parcheggiati verso 1 <-
	std::vector<Train&> parked_trains_weast_;

	/*
	Variabili private che forse servono:

	*/
};

///////////////
int TimeToFree(const std::list<Train>& t, const MainStation& s);
std::string FormatTime(int n);
///////////////
#endif // !local_station_h
