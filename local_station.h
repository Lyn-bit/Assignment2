#ifndef local_station_h
#define local_station_h

#include "station.h"

#include <list>

#include "train.h"

class LocalStation : public Station{
public:
	//LocalStation() = delete; /*Forse*/
	LocalStation(std::string name, int type, int distance);

	// Possono esserci treni di solo transito
	// Viene chiamata dal treno quando si trova a 20km dalla stazione
	// Ritorna una stringa contenente le infomrazioni sul binario e 
	// sul tempo d'attesa tipo: "1, 5" -> binario uno e 5 minuti di attesa in parcheggio
	// Se l'attesa è 0 allora il treno non deve fermarsi in parcheggio
	std::string SendMsg(const Train& t, int time) override; 

	std::string GetName() const override;
	int GetType() const override;
	int GetDistnace() const override;
	std::vector<Train> GetTrainsAhead() const override;

	void PrintDepartureTime(const Train& t, int time) const override;
	void PrintArrivalTime(const Train& t, int time) const override;

	 /*
	 Funzioni private mancanti:
	 Funzione che in base al tempo di arrivo del treno 
	 avvisa se ci sarà un binario libero.

	 Funzione che calcola il binario alla richiesta

	 Funzioni che gestiscono le due code prioritarie (parcheggio e stazione)

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
	// 
	// QUINTA cella (stato del binario di transito), VERSO 0
	//
	// SESTA cella, come la quinta ma per il verso opposto
	int tracks_state [6];
	// Lista dei treni che sono in stazione
	std::list<Train> trains_in_station;
	// Lista dei treni che si trovano dalla stazione ai
	// 20km prima della stazione successiva
	std::list<Train> trains_ahead;
	// Lista dei treni parcheggiati
	std::vector<Train> paked_trains;

	/*
	Variabili private che forse servono:

	*/
};

#endif // !local_station_h
