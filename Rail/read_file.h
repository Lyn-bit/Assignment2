//Nome e cognome: Yin Lin
//Matricola: 1188226
//Corso: ingegneria biomedica

//file header della classe ReadFile

#ifndef read_file_h
#define read_file_h

#include "declarations.h"

#include "train.h"
#include "station.h"

class ReadFile
{
public:
	//classe eccezione
	class Invalid
	{
	};

	ReadFile();
	~ReadFile();

	//costruttori copia e move sono impediti
	ReadFile(const ReadFile &m) = delete;
	ReadFile(ReadFile &&m) = delete;
	//anche gli operatori= sono impediti
	ReadFile &operator=(const ReadFile &rf) = delete;
	ReadFile &operator=(const ReadFile &&rf) = delete;

	//funzioni che leggono dati dai file.txt senza modificare
	//restituiscono liste con dati errati
	std::list<Station *> read_Station();
	std::list<Train *> read_Train();

	//funzione che restituisce quante main station ci sono
	int get_num_of_mainstation() const;

	//funzione che restituisce quante local station ci sono
	int get_num_of_localstation() const;

	//funzioni che modificano liste errate
	//tolgono eventuali orari/stazioni inappropriate
	std::list<Station *> modified_station_list();
	std::list<Train *> modified_train_list();

	//funzione che ritorna la stazione di partenza
	Station *get_First_Station() const;
	//funzione che ritorna l'ultima stazione
	Station *get_Last_Station() const;

	//funzioni che ritornano liste aggiustate
	std::list<Station *> get_station_list() const;
	std::list<Train *> get_train_list() const;
	//funzione che restituisce lista di stazioni principali
	std::list<Station *> get_main_station_list();

	//funzioni che restituisco la stazione dopo/prima ricevento come un parametro Station
	const Station *nextStation(const Station *s) const;
	const Station *prevStation(const Station *s) const;

private:
	//liste originali, senza modifiche
	std::list<Station *> Station_list_orig;
	std::list<Train *> Train_list_orig;

	//liste modificate/aggiustate
	std::list<Station *> Station_list;
	std::list<Train *> Train_list;

	//string che memorizza la stazione di partenza
	std::string origin_Station;
	std::string last_Station;

	std::list<Station *> main_station_list;
};

#endif // !read_file_h
