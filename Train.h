#pragma once

#include <list>
#include <utility>
#include <string>
#include <vector>
#include <iostream>


class Train;
class ReadFile;
class Station;

bool operator==(const Station& s_one, const Station& s_two);

std::string FormatTime(int min);

//int TimeToFree(const list<const Train*>& trains_ahead, const Station& a);

class Train
{
public:

	virtual int getMaxSpeed() const = 0; //metodo che ritorna la velocità massima
	virtual int getMaxWait() const = 0; //metodo che ritorna il tempo d'attesa massimo
	virtual int getId() const = 0; //metodo che ritorna l'id del treno
	virtual int getVerse() const = 0; //metodo che ritorna il verso del treno
	virtual int getType() const = 0; //metodo che ritorna il tipo di treno
	virtual std::list<int> getTimes() const = 0; //metodo che ritorna la lista degli orari
	virtual int getSpeed() const = 0; //metodo che ritorna la velocità del treno
	virtual int getPosition() const = 0; //metodo che ritorna la posizione del treno

	//metodo che controlla se c'è un ritardo nel tempo
	virtual bool checkDelay() = 0;
	//metodo che ritorna un eventuale tempo di ritardo
	virtual int getDelay() const = 0;

	//metodo che ritorna la stazione in cui il treno si trova al momento
	virtual const Station* getCurrentStation() const = 0;

	//metodo che ritorna la prossima stazione in cui il treno dovrà andare
	virtual const Station* getNextStation() = 0;

	//metodo che imposta il tempo di attesa del treno
	virtual void SetWaitTime(int waitTime) = 0;
	//metodo che ritorna il tempo di attesa del treno
	virtual int GetTimeLeft() const = 0;
    //metodo che ritorna il tempo che il treno ha aspettato
    virtual int GetWaitTime() const = 0;

	//metodo che imposta lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	virtual void setState(std::string status) = 0;
	//metodo che ritorna lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	virtual std::string getState() const = 0;

	//metodo che imposta il binario in cui il treno dovrà andare
	virtual void SetTrack(int trackNumber) = 0;
	//metodo che ritorna il binario in cui il treno dovrà andare
	virtual int GetTrack() const = 0;

	//metodo che invia una richiesta d'arrivo alla stazione
	virtual std::pair<int, const Station*> SendArrivalRequest() = 0;

	//metodo che invia una richiesta di partenza alla stazione
	virtual std::pair<int, const Station*> SendDepartureRequest() = 0;

	//controllo se che un treno davanti
	virtual bool checkTrainAhead() const = 0;

	//treno che si trova davanti
	virtual const Train* getTrainAhead() const = 0;

	//metodo che aggiorna le variabili che cambiano con il tempo
	virtual void update() = 0;

};

class Station
{
public:
	virtual void ArrivalRequest(Train* t) = 0;
	virtual bool DepartureRequest(Train* t) = 0;
	virtual void Update() = 0;
	virtual int GetNextTrain(const Train* t) const = 0;
	virtual std::string GetName() const = 0;
	virtual int GetType() const = 0;
	virtual int GetDistance() const = 0;
	virtual std::list<const Train*> GetTrainsAhead(int verse) const = 0;

	virtual void PrintDepartureTime(const Train* t, int time) const = 0;
	virtual void PrintArrivalTime(const Train* t, int time, int delay) const = 0;
	// Da rivedere
	
};


class MainStation : public Station {
public:
	/*
	Differenziare stazione origine e capolinea!!!!!
	Variabile int che :
		vale 1 se è stazione d'origine
		2 se capolinea
		-1 per tutte le altre
	*/


	MainStation(std::string name, int type, int distance, const ReadFile& r);

	/*Forse*/
	MainStation(const MainStation& s) = default;
	MainStation(MainStation&& s) = default;

	const MainStation& operator=(const MainStation& s) = delete;
	const MainStation& operator=(const MainStation&& s) = delete;
	////////

	// Viene chiamata dal treno quando si trova a 20km dalla stazione
	// Imposta direttamente le infomrazioni sul binario e 
	// sul tempo d'attesa
	void ArrivalRequest(Train* t) override;
	// Richiesta fatta dal treno alla stazione per capire se può partire o meno
	// quando si trova nei parcheggi o in stazione
	// in caso il treno non possa ancora partire aggiunge il ritardo a se
	// se stesso e ai treni che devono arrivare sul suo binario
	bool DepartureRequest(Train* t) override;
	// viene chiamato ogni incremento del contatore. 
	// Aggiorna le variabili che dipendono dal tempo
	void Update() override;
	// Ritorna un intero che indica la distanza dal 
	// origine del treno che ha davanti.
	// Ritorna -1 se non ci sono treni
	int GetNextTrain(const Train* t) const override;


	inline std::string GetName() const override;
	inline int GetType() const override;
	inline int GetDistance() const override;


	// Ritorna una lista dei treni che si trovano tra questa stazione 
	// e la succesiva/precedente in base al verso
	inline std::list<const Train*> GetTrainsAhead(int verse) const override;

	/*da fare*/
	void PrintDepartureTime(const Train* t, int time) const override;
	void PrintArrivalTime(const Train* t, int time, int delay) const override;

private:
	void AddParkedTrain(Train* t);
	bool ComparePriority(const Train* t1, const Train* t2) const;
	int GetEstimatedArrivalTime(const Train* t) const;
	// Ritorna un array con le prime due posizioni i minuti d'attesa
	// (senza contare i 5 minuti che ogni treno deve stare fermo in stazione)
	// per poter usufurire del binario piu veloce e il numero di binario
	// le ultime 2 celle uguale ma di quello più lento
	std::vector<int> TrackStatus(const Train* t) const;

	/*
	 ...
	 */

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
	int tracks_state_[4];
	// Lista dei treni che sono in stazione
	std::list<Train*> trains_in_station_;
	// Lista dei treni che si trovano dalla stazione ai
	// 5km prima della stazione successiva/precedente
	// l'ultimo treno nella lista è quello più vicino alla stazione
	// east <-
	// weast ->
	std::list<const Train*> trains_ahead_east_;
	std::list<const Train*> trains_ahead_weast_;
	//Sono strutturate in modo da avere un treno con binario 0 e 1 alternati
	//
	// Lista dei treni parcheggiati verso 0 ->
	std::vector<Train*> parked_trains_east_;
	// Lista dei treni parcheggiati verso 1 <-
	std::vector<Train*> parked_trains_weast_;
};



class LocalStation : public Station {
public:
	//LocalStation() = delete; /*Forse*/
	LocalStation(std::string name, int type, int distance, const ReadFile&);

	/*Forse*/
	LocalStation(const LocalStation& s) = default;
	LocalStation(LocalStation&& s) = default;

	const LocalStation& operator=(const LocalStation& s) = delete;
	const LocalStation& operator=(const LocalStation&& s) = delete;
	////////

	// Possono esserci treni di solo transito
	// Viene chiamata dal treno quando si trova a 20km dalla stazione
	// Ritorna una stringa contenente le infomrazioni sul binario e 
	// sul tempo d'attesa tipo: "1, 5" -> binario uno e 5 minuti di attesa in parcheggio
	// Se l'attesa è 0 allora il treno non deve fermarsi in parcheggio
	void ArrivalRequest(Train* t) override;
	bool DepartureRequest(Train* t) override;

	void Update() override;

	int GetNextTrain(const Train* t) const override;


	inline std::string GetName() const override;
	inline int GetType() const override;
	inline int GetDistance() const override;

	/*Da aggiungere i treni di transito(appena riceve la richiesta a 20km in base all
	situazione la stazione sa quando il treno sarà arrivato)ccc*/
	inline std::list<const Train*> GetTrainsAhead(int verse) const override;

	void PrintDepartureTime(const Train* t, int time) const override;
	void PrintArrivalTime(const Train* t, int time, int delay) const override;

private:
	void AddParkedTrain(Train* t);
	void AddDelay(int delay, int verse);
	bool ComparePriority(const Train* t1, const Train* t2) const;
	int GetEstimatedArrivalTime(const Train* t) const;
	// Ritorna un array con le prime due posizioni i minuti d'attesa
	// (senza contare i 5 minuti che ogni treno deve stare fermo in stazione)
	// per poter usufurire del binario piu veloce e il numero di binario
	// e le ultime 2 di quello più lento
	// l'ultima cella contiene l'attesa in minuti per poter usare il 
	// binario di transito in base al verso del treno
	std::vector<int> TrackStatus(const Train* t) const;

	// calocla il tempo che il treno ci impiegha a superare la stazione di 10km
	// in base alla posizione
	int GetHighSpeedTrainTime(const Train* t) const;
	// In base al bonario trova il treno
	const Train* FindTrain(int track) const;

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
	// I binari di trainsito sarebbero il 5(0) e il 6(1) ma non servono nell'array
	int tracks_state_[4];
	// Lista dei treni che sono in stazione (non conta quelli di transito)
	std::list<Train*> trains_in_station_;

	// Lista dei treni che si trovano dalla stazione ai
	// 5km prima della stazione successiva/precedente
	// l'ultimo treno nella lista è quello più vicino alla stazione
	// east <-
	// weast ->
	std::list<const Train*> trains_ahead_east_;
	std::list<const Train*> trains_ahead_weast_;
	// Lista dei treni parcheggiati verso 0 ->
	std::vector<Train*> parked_trains_east_;
	// Lista dei treni parcheggiati verso 1 <-
	std::vector<Train*> parked_trains_weast_;

	/*
	Variabili private che forse servono:

	*/
};

class ReadFile {
public:
	ReadFile();
	~ReadFile();
	ReadFile(const ReadFile& m) = delete;
	ReadFile(ReadFile&& m) = delete;
	ReadFile& operator=(const ReadFile& rf) = delete;
	ReadFile& operator=(const ReadFile&& rf) = delete;


	//funzioni che leggono dati dai file.txt senza modificare
	//restituiscono liste con dati errati
	std::list<Station*> read_Station();
	std::list<Train*> read_Train();

	//funzione che restituisce quante main station ci sono
	int get_num_of_mainstation() const;

	//funzione che restituisce quante local station ci sono
	int get_num_of_localstation() const;

	//funzioni che modificano liste errate
	//tolgono eventuali orari/stazioni inappropriate
	std::list<Station*> modified_station_list() const;
	std::list<Train*> modified_train_list() const;

	//funzione che ritorna la stazione di partenza
	Station* get_First_Station() const;
	//funzione che ritorna l'ultima stazione
	Station* get_Last_Station() const;

	//funzioni che ritornano liste aggiustate
	std::list<Station*> get_station_list() const;
	std::list<Train*> get_train_list() const;
	//funzione che restituisce lista di stazioni principali
	std::list<Station*> get_main_station_list() const;

	//funzioni che restituisco la stazione dopo/prima ricevento come un parametro Station
	const Station* nextStation(const Station* s) const;
	const Station* prevStation(const Station* s) const;

private:
	//liste originali, senza modifiche
	std::list<Station*> Station_list_orig;
	std::list<Train*> Train_list_orig;

	//liste modificate/aggiustate
	std::list<Station*> Station_list;
	std::list<Train*> Train_list;

	//string che memorizza la stazione di partenza
	std::string origin_Station;
	std::string last_Station;

};


class Train_Regional : public Train
{
public:
	Train_Regional(int numero, int tipo, int direzione, std::list<int> orari, const ReadFile* temp);
	//~Train_Regional();

	int getMaxSpeed() const override; //metodo che ritorna la velocità massima del treno
	int getMaxWait() const override; //metodo che ritorna il tempo d'attesa massimo del treno
	int getId() const override; //metodo che ritorna l'id del treno
	int getVerse() const override; //metodo che ritorna il verso del treno
	int getType() const override; //metodo che ritorna il tipo di treno
	std::list<int> getTimes() const override; //metodo che ritorna la lista degli orari
	int getSpeed() const override; //metodo che ritorna la velocità del treno
	int getPosition() const override; //metodo che ritorna la posizione del treno

	/////
	int GetTimeLeft() override { return 0; }

	//metodo che controlla se c'è un ritardo nel tempo
	bool checkDelay() override;
	//metodo che ritorna un eventuale tempo di ritardo
	int getDelay() const override;

	//metodo che ritorna la stazione in cui il treno si trova al momento
	const Station* getCurrentStation() const override;

	//metodo che ritorna la prossima stazione in cui il treno dovrà andare
	const Station* getNextStation() override;

	//metodo che imposta il tempo di attesa del treno
	void SetWaitTime(int waitTime) override;
	//metodo che ritorna il tempo di attesa del treno
	int GetWaitTime() const override;

	//metodo che imposta il binario in cui il treno dovrà andare
	void SetTrack(int trackNumber) override;
	//metodo che ritorna il binario in cui il treno dovrà andare
	int GetTrack() const override;

	//metodo che imposta lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	void setState(std::string status) override;
	//metodo che ritorna lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	std::string getState() const override;

	//metodo che manda una richiesta d'arrivo alla stazione
	std::pair<int, const Station*> SendArrivalRequest() override;

	//metodo che manda una richiesta di partenza alla stazione
	std::pair<int, const Station*> SendDepartureRequest() override;

	//controllo se ci sono treni davanti
	bool checkTrainAhead() const override;

	//treno che si trova davanti
	const Train* getTrainAhead() const override;

	//metodo che aggiorna le viariabili che cambiano con il tempo
	void update() override;

private:
	int id;
	int verse;
	int type;
	std::list<int> times;
	int speed;
	int position;
	const int MAX_SPEED = 160;
	const int MAX_WAIT = 20;
	int wait; //tempo d'attesa
	int track; //binario
	const Station* Current; //stazione in cui si trova al momento
	const ReadFile* file; //oggetto per controllare la lista delle stazioni
	std::string state; //stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	int GlobalTime = 0; //ora globale
	int delay = 0; //eventuale tempo di ritardo
};

class Train_High_Speed_Super : public Train
{
public:
	Train_High_Speed_Super(int numero, int tipo, int direzione, std::list<int> orari, const ReadFile* temp);
	//~Train_High_Speed_Super();

	int getMaxSpeed() const override; //metodo che ritorna la velocità massima del treno
	int getMaxWait() const override; //metodo che ritorna il tempo d'attesa massimo del treno
	int getId() const override; //metodo che ritorna l'id del treno
	int getVerse() const override; //metodo che ritorna il verso del treno
	int getType() const override; //metodo che ritorna il tipo di treno
	std::list<int> getTimes() const override; //metodo che ritorna la lista degli orari
	int getSpeed() const override; //metodo che ritorna la velocità del treno
	int getPosition() const override; //metodo che ritorna la posizione del treno

	/////
	int GetTimeLeft() override { return 0; }

	//metodo che controlla se c'è un ritardo nel tempo
	bool checkDelay() override;
	//metodo che ritorna un eventuale tempo di ritardo
	int getDelay() const override;

	//metodo che ritorna la stazione in cui il treno si trova al momento
	const Station* getCurrentStation() const override;

	//metodo che ritorna la prossima stazione in cui il treno dovrà andare
	const Station* getNextStation() override;

	//metodo che imposta il tempo di attesa del treno
	void SetWaitTime(int waitTime) override;
	//metodo che ritorna il tempo di attesa del treno
	int GetWaitTime() const override;

	//metodo che imposta il binario in cui il treno dovrà andare
	void SetTrack(int trackNumber) override;
	//metodo che ritorna il binario in cui il treno dovrà andare
	int GetTrack() const override;

	//metodo che imposta lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	void setState(std::string status) override;
	//metodo che ritorna lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	std::string getState() const override;

	//metodo che manda una richiesta d'arrivo alla stazione
	std::pair<int, const Station*> SendArrivalRequest() override;

	//metodo che manda una richiesta di partenza alla stazione
	std::pair<int, const Station*> SendDepartureRequest() override;

	//controllo se ci sono treni davanti
	bool checkTrainAhead() const override;

	//treno che si trova davanti
	const Train* getTrainAhead() const override;

	//metodo che aggiorna le viariabili che cambiano con il tempo
	void update() override;

private:
	int id;
	int verse;
	int type;
	std::list<int> times;
	int speed;
	int position;
	const int MAX_SPEED = 300;
	const int MAX_WAIT = 5;
	int wait; //tempo d'attesa
	int track; //binario
	const Station* Current; //stazione in cui si trova al momento
	const ReadFile* file; //oggetto per controllare la lista delle stazioni
	std::string state; //stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	int GlobalTime = 0; //ora globale
	int delay = 0; //eventuale tempo di ritardo
};

class Train_High_Speed : public Train
{
public:
	Train_High_Speed(int numero, int tipo, int direzione, std::list<int> orari, const ReadFile* temp);
	//~Train_High_Speed();

	int getMaxSpeed() const override; //metodo che ritorna la velocità massima del treno
	int getMaxWait() const override; //metodo che ritorna il tempo d'attesa massimo del treno
	int getId() const override; //metodo che ritorna l'id del treno
	int getVerse() const override; //metodo che ritorna il verso del treno
	int getType() const override; //metodo che ritorna il tipo di treno
	std::list<int> getTimes() const override; //metodo che ritorna la lista degli orari
	int getSpeed() const override; //metodo che ritorna la velocità del treno
	int getPosition() const override; //metodo che ritorna la posizione del treno

	/////
	int GetTimeLeft() override { return 0; }

	//metodo che controlla se c'è un ritardo nel tempo
	bool checkDelay() override;
	//metodo che ritorna un eventuale tempo di ritardo
	int getDelay() const override;

	//metodo che ritorna la stazione in cui il treno si trova al momento
	const Station* getCurrentStation() const override;

	//metodo che ritorna la prossima stazione in cui il treno dovrà andare
	const Station* getNextStation() override;

	//metodo che imposta il tempo di attesa del treno
	void SetWaitTime(int waitTime) override;
	//metodo che ritorna il tempo di attesa del treno
	int GetWaitTime() const override;

	//metodo che imposta il binario in cui il treno dovrà andare
	void SetTrack(int trackNumber) override;
	//metodo che ritorna il binario in cui il treno dovrà andare
	int GetTrack() const override;

	//metodo che imposta lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	void setState(std::string status) override;
	//metodo che ritorna lo stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	std::string getState() const override;

	//metodo che manda una richiesta d'arrivo alla stazione
	std::pair<int, const Station*> SendArrivalRequest() override;

	//metodo che manda una richiesta di partenza alla stazione
	std::pair<int, const Station*> SendDepartureRequest() override;

	//controllo se ci sono treni davanti
	bool checkTrainAhead() const override;

	//treno che si trova davanti
	const Train* getTrainAhead() const override;

	//metodo che aggiorna le viariabili che cambiano con il tempo
	void update() override;

private:
	int id;
	int verse;
	int type;
	std::list<int> times;
	int speed;
	int position;
	const int MAX_SPEED = 240;
	const int MAX_WAIT = 15;
	int wait; //tempo d'attesa
	int track; //binario
	const Station* Current; //stazione in cui si trova al momento
	const ReadFile* file; //oggetto per controllare la lista delle stazioni
	std::string state; //stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	int GlobalTime = 0; //ora globale
	int delay = 0; //eventuale tempo di ritardo
};


//int TimeToFree(const list<const Train*>& trains_ahead, const LocalStation& s);