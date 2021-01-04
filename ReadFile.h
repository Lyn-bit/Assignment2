//file ReadFile.h
#ifndef ReadFile_h
#define ReadFile_h

#include <iostream>
#include <list>
#include <string>
#include "station.h"
#include "main_station.h"
#include "local_station.h"
#include "Train.h"
#include "Train_Regional.h"
#include "Train_High_Speed.h"
#include "Train_High_Speed_Super.h"

class ReadFile{
   public:
        ReadFile();
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
        std::list<Station*> modified_station_list();
        std::list<Train*> modified_train_list();

        //funzione che ritorna la stazione di partenza
        MainStation get_first_Station() const;
        //funzione che ritorna l'ultima stazione
        MainStation get_last_Station() const;

        //funzioni che ritornano liste aggiustate
        std::list<Station*> get_station_list() const;
        std::list<Train*> get_train_list() const;
        //funzione che restituisce lista di stazioni principali
        std::list<Station*> get_main_station_list() const;

        //funzioni che restituisco la stazione dopo/prima ricevento come un parametro Station
        Station* nextStation(Station* s);
        Station* prevStation(Station* s);
      
    private:
        //liste originali, senza modifiche
        std::list<Station*> Station_list_orig;
        std::list<Train*> Train_list_orig;

        //liste modificate/aggiustate
        std::list<Station*> Station_list;
        std::list<Train*> Train_list;

        //string che memorizza la stazione di partenza
        string origin_Station;
        string last_Station;

};


#endif // ReadFile_h