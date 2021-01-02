//file ReadFile.h
#ifndef ReadFile_h
#define ReadFile_h

#include <iostream>
#include <list>
#include <string>
#include "station.h"
#include "Train.h"

class ReadFile{
   public:
        ReadFile();
        //funzioni che leggono dati dai file.txt senza modificare
        //restituiscono liste con dati errati
        std::list<Station> read_Station(); 
        std::list<Train> read_Train();

        //funzione che restituisce quante main station ci sono
        int get_num_of_mainstation();

        //funzione che restituisce quante local station ci sono
        int get_num_of_localstation();

        //funzioni che modificano liste errate
        //tolgono eventuali orari/stazioni inappropriate
        std::list<Station> modify_station_list();
        std::list<Train> modify_train_list();

        //funzioni che ritornano liste esatte
        std::list<Station> get_list_Station(); 
        std::list<Train> get_list_Train();

        //funzione che ritorna la stazione di partenza
        std::string get_origin_Station();
      
    private:
        //liste originali, senza modifiche
        std::list<Station> Station_list_orig;
        std::list<Train> Train_list_orig;

        //liste modificate
        std::list<Station> Station_list;
        std::list<Train> Train_list;

        //string che memorizza la stazione di partenza
        std::string origin_Station;

};

#endif // ReadFile_h