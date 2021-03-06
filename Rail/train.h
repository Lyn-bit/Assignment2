//Ciccone Davide

#ifndef train_h
#define train_h

#include "declarations.h"

#include "station.h"
#include "read_file.h"

class Train
{
public:
	virtual int getMaxSpeed() const = 0;		 //metodo che ritorna la velocità massima
	virtual int getMaxWait() const = 0;			 //metodo che ritorna il tempo d'attesa massimo
	virtual int getId() const = 0;				 //metodo che ritorna l'id del treno
	virtual int getVerse() const = 0;			 //metodo che ritorna il verso del treno
	virtual int getType() const = 0;			 //metodo che ritorna il tipo di treno
	virtual std::list<int> getTimes() const = 0; //metodo che ritorna la lista degli orari
	virtual int getSpeed() const = 0;			 //metodo che ritorna la velocità del treno
	virtual int getPosition() const = 0;		 //metodo che ritorna la posizione del treno

	//metodo che ritorna il tempo globale
	virtual int getTime() = 0;

	//metodo che controlla se c'è un ritardo nel tempo
	virtual bool checkDelay() = 0;
	//metodo che ritorna un eventuale tempo di ritardo
	virtual int getDelay() const = 0;

	//metodo che ritorna la stazione in cui il treno si trova al momento
	virtual const Station *getCurrentStation() const = 0;

	//metodo che ritorna la prossima stazione in cui il treno dovrà andare
	virtual const Station *getNextStation() = 0;

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
	virtual std::pair<int, const Station *> SendArrivalRequest() = 0;

	//metodo che invia una richiesta di partenza alla stazione
	virtual std::pair<int, const Station *> SendDepartureRequest() = 0;

	//controllo se che un treno davanti
	virtual bool checkTrainAhead() const = 0;

	//treno che si trova davanti
	virtual const Train *getTrainAhead() const = 0;

	//metodo che aggiorna le variabili che cambiano con il tempo
	virtual std::pair<int, const Station *> update() = 0;
};

class Train_Regional : public Train
{
public:
	Train_Regional(int numero, int direzione, int tipo, std::list<int> orari, const ReadFile *temp);
	//~Train_Regional();

	int getMaxSpeed() const override;		  //metodo che ritorna la velocità massima del treno
	int getMaxWait() const override;		  //metodo che ritorna il tempo d'attesa massimo del treno
	int getId() const override;				  //metodo che ritorna l'id del treno
	int getVerse() const override;			  //metodo che ritorna il verso del treno
	int getType() const override;			  //metodo che ritorna il tipo di treno
	std::list<int> getTimes() const override; //metodo che ritorna la lista degli orari
	int getSpeed() const override;			  //metodo che ritorna la velocità del treno
	int getPosition() const override;		  //metodo che ritorna la posizione del treno

	//metodo che ritorna il tempo globale
	int getTime() override;
	//metodo che controlla se c'è un ritardo nel tempo
	bool checkDelay() override;
	//metodo che ritorna un eventuale tempo di ritardo
	int getDelay() const override;

	//metodo che ritorna la stazione in cui il treno si trova al momento
	const Station *getCurrentStation() const override;

	//metodo che ritorna la prossima stazione in cui il treno dovrà andare
	const Station *getNextStation() override;

	//metodo che imposta il tempo di attesa del treno
	void SetWaitTime(int waitTime) override;
	//metodo che ritorna il tempo di attesa del treno
	int GetTimeLeft() const override;
	//metodo che ritorna il tempo che il treno ha aspettato
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
	std::pair<int, const Station *> SendArrivalRequest() override;

	//metodo che manda una richiesta di partenza alla stazione
	std::pair<int, const Station *> SendDepartureRequest() override;

	//controllo se ci sono treni davanti
	bool checkTrainAhead() const override;

	//treno che si trova davanti
	const Train *getTrainAhead() const override;

	//metodo che aggiorna le viariabili che cambiano con il tempo
	std::pair<int, const Station *> update() override;

private:
	int id;
	int verse;
	int type;
	std::list<int> times;
	int speed;
	int position;
	int wait;						  //tempo d'attesa
	int track;						  //binario
	const Station *Current; //stazione in cui si trova al momento
	const ReadFile *file;			  //oggetto per controllare la lista delle stazioni
	std::string state;				  //stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	int GlobalTime;				  //ora globale
	int delay;					  //eventuale tempo di ritardo
	int TimePassed;				  //tempo che è rimasto fermo
	int firstTime; //primo orario
	const int MAX_SPEED = 160;
	const int MAX_WAIT = 20;
};

class Train_High_Speed_Super : public Train
{
public:
	Train_High_Speed_Super(int numero, int direzione, int tipo, std::list<int> orari, const ReadFile *temp);
	//~Train_High_Speed_Super();

	int getMaxSpeed() const override;		  //metodo che ritorna la velocità massima del treno
	int getMaxWait() const override;		  //metodo che ritorna il tempo d'attesa massimo del treno
	int getId() const override;				  //metodo che ritorna l'id del treno
	int getVerse() const override;			  //metodo che ritorna il verso del treno
	int getType() const override;			  //metodo che ritorna il tipo di treno
	std::list<int> getTimes() const override; //metodo che ritorna la lista degli orari
	int getSpeed() const override;			  //metodo che ritorna la velocità del treno
	int getPosition() const override;		  //metodo che ritorna la posizione del treno

	//metodo che ritorna il tempo globale
	int getTime() override;
	//metodo che controlla se c'è un ritardo nel tempo
	bool checkDelay() override;
	//metodo che ritorna un eventuale tempo di ritardo
	int getDelay() const override;

	//metodo che ritorna la stazione in cui il treno si trova al momento
	const Station *getCurrentStation() const override;

	//metodo che ritorna la prossima stazione in cui il treno dovrà andare
	const Station *getNextStation() override;

	//metodo che imposta il tempo di attesa del treno
	void SetWaitTime(int waitTime) override;
	//metodo che ritorna il tempo di attesa del treno
	int GetTimeLeft() const override;
	//metodo che ritorna il tempo che il treno ha aspettato
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
	std::pair<int, const Station *> SendArrivalRequest() override;

	//metodo che manda una richiesta di partenza alla stazione
	std::pair<int, const Station *> SendDepartureRequest() override;

	//controllo se ci sono treni davanti
	bool checkTrainAhead() const override;

	//treno che si trova davanti
	const Train *getTrainAhead() const override;

	//metodo che aggiorna le viariabili che cambiano con il tempo
	std::pair<int, const Station *> update() override;

private:
	int id;
	int verse;
	int type;
	std::list<int> times;
	int speed;
	int position;
	int wait;						  //tempo d'attesa
	int track;						  //binario
	const Station *Current; //stazione in cui si trova al momento
	const ReadFile *file;			  //oggetto per controllare la lista delle stazioni
	std::string state;				  //stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	int GlobalTime;				  //ora globale
	int delay;					  //eventuale tempo di ritardo
	int TimePassed;				  //tempo che è rimasto fermo
	int firstTime; //primo orario
	const int MAX_SPEED = 300;
	const int MAX_WAIT = 5;
};

class Train_High_Speed : public Train
{
public:
	Train_High_Speed(int numero, int direzione, int tipo, std::list<int> orari, const ReadFile *temp);
	//~Train_High_Speed();

	int getMaxSpeed() const override;		  //metodo che ritorna la velocità massima del treno
	int getMaxWait() const override;		  //metodo che ritorna il tempo d'attesa massimo del treno
	int getId() const override;				  //metodo che ritorna l'id del treno
	int getVerse() const override;			  //metodo che ritorna il verso del treno
	int getType() const override;			  //metodo che ritorna il tipo di treno
	std::list<int> getTimes() const override; //metodo che ritorna la lista degli orari
	int getSpeed() const override;			  //metodo che ritorna la velocità del treno
	int getPosition() const override;		  //metodo che ritorna la posizione del treno

	//metodo che ritorna il tempo globale
	int getTime() override;
	//metodo che controlla se c'è un ritardo nel tempo
	bool checkDelay() override;
	//metodo che ritorna un eventuale tempo di ritardo
	int getDelay() const override;

	//metodo che ritorna la stazione in cui il treno si trova al momento
	const Station *getCurrentStation() const override;

	//metodo che ritorna la prossima stazione in cui il treno dovrà andare
	const Station *getNextStation() override;

	//metodo che imposta il tempo di attesa del treno
	void SetWaitTime(int waitTime) override;
	//metodo che ritorna il tempo di attesa del treno
	int GetTimeLeft() const override;
	//metodo che ritorna il tempo che il treno ha aspettato
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
	std::pair<int, const Station *> SendArrivalRequest() override;

	//metodo che manda una richiesta di partenza alla stazione
	std::pair<int, const Station *> SendDepartureRequest() override;

	//controllo se ci sono treni davanti
	bool checkTrainAhead() const override;

	//treno che si trova davanti
	const Train *getTrainAhead() const override;

	//metodo che aggiorna le viariabili che cambiano con il tempo
	std::pair<int, const Station *> update() override;

private:
	int id;
	int verse;
	int type;
	std::list<int> times;
	int speed;
	int position;
	int wait;						  //tempo d'attesa
	int track;						  //binario
	const Station* Current; //stazione in cui si trova al momento
	const ReadFile *file;			  //oggetto per controllare la lista delle stazioni
	std::string state;				  //stato del treno, "s" in stazione, "p" nel parcheggio, "v" in viaggio
	int GlobalTime;				  //ora globale
	int delay;					  //eventuale tempo di ritardo
	int TimePassed;				  //tempo che è rimasto fermo
	int firstTime; //primo orario
	const int MAX_SPEED = 240;
	const int MAX_WAIT = 15;
};

#endif // !train_h
