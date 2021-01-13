#ifndef station_h
#define station_h

#include "declarations.h"

#include "train.h"
#include "read_file.h"

bool operator==(const Station &s_one, const Station &s_two);
int TimeToFree(const std::list<const Train *> &trains_ahead, const Station &a);

class Station
{
public:
	virtual void ArrivalRequest(Train *t, const ReadFile &) = 0;
	virtual bool DepartureRequest(Train *t, const ReadFile &) = 0;
	virtual void Update(const ReadFile &) = 0;
	virtual int GetNextTrain(const Train *t) const = 0;
	virtual std::string GetName() const = 0;
	virtual int GetType() const = 0;
	virtual int GetDistance() const = 0;
	virtual std::list<const Train *> GetTrainsAhead(int verse) const = 0;
	virtual int Position(const ReadFile &) const = 0;
	virtual void SetTypeVoid() = 0;

	virtual void PrintDepartureTime(const Train *t, int time, const ReadFile &file) const = 0;
	virtual void PrintArrivalTime(const Train *t, int time, int delay, const ReadFile &file) const = 0;
};

class MainStation : public Station
{
public:
	MainStation(std::string name, int type, int distance, const ReadFile &r);

	MainStation(const MainStation &s) = default;
	MainStation(MainStation &&s) = default;

	const MainStation &operator=(const MainStation &s) = delete;
	const MainStation &operator=(const MainStation &&s) = delete;

	inline void SetTypeVoid() override;

	// Viene chiamata dal treno quando si trova a 20km dalla stazione
	// Imposta direttamente le infomrazioni sul binario e
	// sul tempo d'attesa
	void ArrivalRequest(Train *t, const ReadFile &) override;
	// Richiesta fatta dal treno alla stazione per capire se pu� partire o meno
	// quando si trova nei parcheggi o in stazione
	// in caso il treno non possa ancora partire aggiunge il ritardo a se
	// se stesso e ai treni che devono arrivare sul suo binario
	bool DepartureRequest(Train *t, const ReadFile &) override;
	// viene chiamato ogni incremento del contatore.
	// Aggiorna le variabili che dipendono dal tempo
	void Update(const ReadFile &) override;

	int Position(const ReadFile &) const override;
	// Ritorna un intero che indica la distanza dal
	// origine del treno che ha davanti.
	// Ritorna -1 se non ci sono treni
	int GetNextTrain(const Train *t) const override;

	inline std::string GetName() const override;
	inline int GetType() const override;
	inline int GetDistance() const override;

	// Ritorna una lista dei treni che si trovano tra questa stazione
	// e la succesiva/precedente in base al verso
	inline std::list<const Train *> GetTrainsAhead(int verse) const override;

	void PrintDepartureTime(const Train *t, int time, const ReadFile &file) const override;
	void PrintArrivalTime(const Train *t, int time, int delay, const ReadFile &file) const override;

private:
	void AddParkedTrain(Train *t);
	bool ComparePriority(const Train *t1, const Train *t2) const;
	int GetEstimatedArrivalTime(const Train *t) const;
	// Ritorna un array con le prime due posizioni i minuti d'attesa
	// (senza contare i 5 minuti che ogni treno deve stare fermo in stazione)
	// per poter usufurire del binario piu veloce e il numero di binario
	// le ultime 2 celle uguale ma di quello pi� lento
	std::vector<int> TrackStatus(const Train *t) const;

private:
	std::string name_;
	int type_;
	int distance_;
	// 1 se originie, 2 se capolinea, -1 per le altre
	const ReadFile &read_file_;
	// Array di interi di dimensione fissa con:
	// Cella 0 vuota
	// SECONDA e TERZA cella contengono il tempo in minuti che mancano alla
	// partenza per i treni che si trovano ai binari 1 e 2.
	// Queste due celle indicano che i banari sono BINARI STANDARD con
	// VERSO "0"
	//
	// TERZA e QUARTA cella uguale a prime due ma per il verso opposto
	int tracks_state_[5];
	// Lista dei treni che sono in stazione
	std::list<Train *> trains_in_station_;
	// Lista dei treni che si trovano dalla stazione ai
	// 5km prima della stazione successiva/precedente
	// l'ultimo treno nella lista e' quello piu' vicino alla stazione
	// east <-
	// weast ->
	std::list<const Train *> trains_ahead_east_;
	std::list<const Train *> trains_ahead_weast_;
	//Sono strutturate in modo da avere un treno con binario 0 e 1 alternati
	//
	// Lista dei treni parcheggiati verso 0 ->
	std::vector<Train *> parked_trains_east_;
	// Lista dei treni parcheggiati verso 1 <-
	std::vector<Train *> parked_trains_weast_;
};

class LocalStation : public Station
{
public:
	LocalStation(std::string name, int type, int distance, const ReadFile &);
	LocalStation(const LocalStation &s) = default;
	LocalStation(LocalStation &&s) = default;

	const LocalStation &operator=(const LocalStation &s) = delete;
	const LocalStation &operator=(const LocalStation &&s) = delete;

	// Possono esserci treni di solo transito
	// Viene chiamata dal treno quando si trova a 20km dalla stazione
	// Ritorna una stringa contenente le infomrazioni sul binario e
	// sul tempo d'attesa tipo: "1, 5" -> binario uno e 5 minuti di attesa in parcheggio
	// Se l'attesa e' 0 allora il treno non deve fermarsi in parcheggio
	void ArrivalRequest(Train *t, const ReadFile &) override;
	bool DepartureRequest(Train *t, const ReadFile &) override;
	void Update(const ReadFile &) override;

	int Position(const ReadFile &) const override;
	inline void SetTypeVoid() override;

	int GetNextTrain(const Train *t) const override;
	inline std::string GetName() const override;
	inline int GetType() const override;
	inline int GetDistance() const override;

	inline std::list<const Train *> GetTrainsAhead(int verse) const override;

	void PrintDepartureTime(const Train *t, int time, const ReadFile &file) const override;
	void PrintArrivalTime(const Train *t, int time, int delay, const ReadFile &file) const override;

private:
	void AddParkedTrain(Train *t);
	void AddDelay(int delay, int verse);
	bool ComparePriority(const Train *t1, const Train *t2) const;
	int GetEstimatedArrivalTime(const Train *t) const;
	// Ritorna un array con le prime due posizioni i minuti d'attesa
	// (senza contare i 5 minuti che ogni treno deve stare fermo in stazione)
	// per poter usufurire del binario piu veloce e il numero di binario
	// e le ultime 2 di quello piu' lento
	// l'ultima cella contiene l'attesa in minuti per poter usare il
	// binario di transito in base al verso del treno
	std::vector<int> TrackStatus(const Train *t) const;

	// calocla il tempo che il treno ci impiegha a superare la stazione di 10km
	// in base alla posizione
	int GetHighSpeedTrainTime(const Train *t) const;
	// In base al bonario trova il treno
	const Train *FindTrain(int track, const std::vector<Train *>) const;

private:
	std::string name_;
	int type_;
	int distance_;
	// 1 se originie, 2 se capolinea, -1 per le altre
	const ReadFile &read_file_;
	// Array di interi di dimensione fissa con:
	// Cella 0 vuota
	// SECONDA e TERZA cella contengono il tempo in minuti che mancano alla
	// partenza per i treni che si trovano ai binari 1 e 2.
	// Queste due celle indicano che i banari sono BINARI STANDARD con
	// VERSO "0"
	//
	// TERZA e QUARTA cella uguale a prime due ma per il verso opposto
	// I binari di trainsito sarebbero nelle celle 5(verso 0) e 6( verso 1) ma non servono nell'array
	int tracks_state_[7];
	// Lista dei treni che sono in stazione (non conta quelli di transito)
	std::list<Train *> trains_in_station_;

	// Lista dei treni che si trovano dalla stazione ai
	// 5km prima della stazione successiva/precedente
	// l'ultimo treno nella lista e' quello piu' vicino alla stazione
	// east <-
	// weast ->
	std::list<const Train *> trains_ahead_east_;
	std::list<const Train *> trains_ahead_weast_;
	// Lista dei treni parcheggiati verso 0 ->
	std::vector<Train *> parked_trains_east_;
	// Lista dei treni parcheggiati verso 1 <-
	std::vector<Train *> parked_trains_weast_;
};

#endif // !station_h
