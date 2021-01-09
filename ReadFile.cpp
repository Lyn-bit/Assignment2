//file ReadFile.cpp

#include "read_file.h"

using namespace std;

//construttore
ReadFile::ReadFile()
{
    

    Station_list_orig = read_Station();
    
    Train_list_orig = read_Train();

    Station_list = modified_station_list();
    Train_list = modified_train_list();

    main_station_list = get_main_station_list();

    origin_Station = Station_list.front()->GetName();
    last_Station = Station_list.back()->GetName();
}

//distruttore
ReadFile::~ReadFile()
{
    /*
    for (auto p = Station_list_orig.begin(); p != Station_list_orig.end(); p++)
    {
        delete* p;
    }
    for (auto p = Train_list_orig.begin(); p != Train_list_orig.end(); p++)
    {
        delete* p;
    }
    for (auto p = Station_list.begin(); p != Station_list.end(); p++)
    {
        delete* p;
    }
    for (auto p = Train_list.begin(); p != Train_list.end(); p++)
    {
        delete* p;
    }
    */
}

list<Station*> ReadFile::read_Station()
{
    list<Station*> temp;
    fstream myfile("line_description.txt");
    if (myfile.is_open())
    {
        myfile >> origin_Station;
        MainStation* ms = new MainStation{ origin_Station,0,0,*this };
        temp.push_back(ms);

        string name;
        int type;
        int distance;
        while (myfile >> name >> type >> distance)
        {
            if (type == 0)
            {
                MainStation* ms = new MainStation{ name,type,distance,*this };
                temp.push_back(ms);
            }
            else if (type == 1)
            {
                LocalStation* ls = new LocalStation{ name,type,distance,*this };
                temp.push_back(ls);
            }
            else
            {
                cout << "Type error(Station)! " << endl;
            }

        }
        return temp;
    }
    else
    {
        cout << "Failed to open" << endl;
    }
}

list<Train*> ReadFile::read_Train()
{
    list<Train*> temp;
    string line;
    fstream myfile("timetables.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            vector<int> temp_vect;
            int i = 0;
            while (i < line.size())
            {
                string str = "";
                while (isdigit(line[i]))
                {
                    char c = line[i];
                    str += c;
                    i++;
                }
                if (str != "")
                {
                    i++;
                }
                temp_vect.push_back(stoi(str));
            }
            int id = temp_vect[0];
            int verse = temp_vect[1];
            int type = temp_vect[2];
            list<int> times{ next(temp_vect.begin(),3),temp_vect.end() };
            if (type == 1)
            {
                Train_Regional* tr = new Train_Regional{ id,verse,type,times, this };
                temp.push_back(tr);
            }
            else if (type == 2)
            {
                Train_High_Speed* ths = new Train_High_Speed{ id,verse,type,times, this };
                temp.push_back(ths);
            }
            else if (type == 3)
            {
                Train_High_Speed_Super* thss = new Train_High_Speed_Super{ id,verse,type,times, this };
                temp.push_back(thss);
            }
            else
            {
                cout << "Type error(Train)! " << endl;
            }
        }
        return temp;
    }
    else
    {
        cout << "Failed to open" << endl;
    }

}

int ReadFile::get_num_of_mainstation() const
{
    int count = 0;
    for (auto p = Station_list.begin(); p != Station_list.end(); p++)
    {
        if ((*p)->GetType() == 0) {
            count++;
        }
    }
    return count;
}

int ReadFile::get_num_of_localstation() const
{
    int count = 0;
    for (auto p = Station_list.begin(); p != Station_list.end(); p++)
    {
        if ((*p)->GetType() == 1)
        {
            count++;
        }
    }
    return count;
}

//rimuovo tutte le stazioni che distano meno di 20 km da quella precedente 
list<Station*> ReadFile::modified_station_list() const
{
    list<Station*> temp = Station_list_orig;
    //copy(Station_list_orig.begin(), Station_list_orig.end(), temp);
    for (auto p = next(Station_list_orig.begin(), 1); p != Station_list_orig.end(); p++)
    {
        if ((*p)->GetDistance() - (*prev(p))->GetDistance() < 20)
        {
            //delete* p;
            p++;
            temp.erase(prev(p, 1));
        }
    }
    return temp;
}

//aggiustare orari inappropriati: tempi impossibili per realizzare il percorso con la max velocità del treno
//rimuovo anche orari in più. considero gli ultimi come quelli in più
//se ci sono piu stazioni, verrano aggiunti orari
list<Train*> ReadFile::modified_train_list() const
{
    list<Train*> temp;
    string line;
    fstream myfile("timetables.txt");
    //
    
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            vector<int> temp_vect;
            int i = 0;
            while (i < line.size())
            {
                string str = "";
                while (isdigit(line[i]))
                {
                    char c = line[i];
                    str += c;
                    i++;
                }
                if (str != "")
                {
                    i++;
                }
                temp_vect.push_back(stoi(str));
            }
            int id = temp_vect[0];
            int verse = temp_vect[1];
            int type = temp_vect[2];
            vector<int> temp_times{ next(temp_vect.begin(),3),temp_vect.end() };
            if (verse == 0)
            {
                if (type == 1)
                {
                    //elimino orari in piu
                    if (temp_times.size() >= Station_list.size())
                    {
                        temp_times.resize(Station_list.size());
                        list<Station*> temp_s = get_station_list();
                        //contatore per vector
                        int i = 1;
                        auto p = next(temp_s.begin(), 1);
                        for (p; p != temp_s.end(); p++)
                        {
                            //approssimo il tempo per eccesso
                            int t_teorico = floor((*p)->GetDistance() / 160 * 60);
                            if (temp_times[i] != t_teorico)
                            {
                                temp_times[i] = t_teorico;
                            }
                            i++;
                        }
                    }
                    //aggiungo orari mancanti
                    else if (temp_times.size() >= Station_list.size())
                    {
                        //gli elementi in più sono rappresentati da -1
                        temp_times.resize(Station_list.size(), -1);
                        list<Station*> temp_s = get_station_list();
                        //contatore per vector
                        int i = 1;
                        auto p = next(temp_s.begin(), 1);
                        for (p; p != temp_s.end(); p++)
                        {
                            if (temp_times[i] != -1)
                            {
                                int t_teorico = floor((*p)->GetDistance() / 160 * 60);
                                if (temp_times[i] != t_teorico)
                                {
                                    temp_times[i] = t_teorico;
                                }
                            }
                            else
                            {
                                int t_teorico = floor((*p)->GetDistance() / 160 * 60);
                                temp_times[i] = t_teorico;
                            }
                        }
                    }
                    list<int> times{ temp_times.begin(),temp_times.end() };
                    Train_Regional* tr = new Train_Regional{ id,verse,type,times, this };
                    temp.push_back(tr);
                }
                else if (type == 2)
                {
                    if (temp_times.size() >= Station_list.size())
                    {
                        temp_times.resize(Station_list.size());
                        list<Station*> temp_s = main_station_list;
                        //contatore per vector
                        int i = 1;
                        auto p = next(temp_s.begin(), 1);
                        for (p; p != temp_s.end(); p++)
                        {
                            //approssimo il tempo per eccesso
                            int t_teorico = floor((*p)->GetDistance() / 240 * 60);
                            if (temp_times[i] != t_teorico)
                            {
                                temp_times[i] = t_teorico;
                            }
                            i++;
                        }
                    }
                    else if (temp_times.size() >= Station_list.size())
                    {
                        //gli elementi in più sono rappresentati da -1
                        temp_times.resize(Station_list.size(), -1);
                        list<Station*> temp_s = main_station_list;
                        //contatore per vector
                        int i = 1;
                        auto p = next(temp_s.begin(), 1);
                        for (p; p != temp_s.end(); p++)
                        {
                            if (temp_times[i] != -1)
                            {
                                int t_teorico = floor((*p)->GetDistance() / 240 * 60);
                                if (temp_times[i] != t_teorico)
                                {
                                    temp_times[i] = t_teorico;
                                }
                            }
                            else
                            {
                                int t_teorico = floor((*p)->GetDistance() / 240 * 60);
                                temp_times[i] = t_teorico;
                            }
                        }
                    }
                    list<int> times{ temp_times.begin(),temp_times.end() };
                    Train_High_Speed* ths = new Train_High_Speed{ id,verse,type,times, this };
                    temp.push_back(ths);
                }
                else if (type == 3)
                {
                    if (temp_times.size() >= Station_list.size())
                    {
                        temp_times.resize(Station_list.size());
                        list<Station*> temp_s = main_station_list;
                        //contatore per vector
                        int i = 1;
                        auto p = next(temp_s.begin(), 1);
                        for (p; p != temp_s.end(); p++)
                        {
                            //approssimo il tempo per eccesso
                            int t_teorico = floor((*p)->GetDistance() / 300 * 60);
                            if (temp_times[i] != t_teorico)
                            {
                                temp_times[i] = t_teorico;
                            }
                            i++;
                        }
                    }
                    else if (temp_times.size() >= Station_list.size())
                    {
                        //gli elementi in più sono rappresentati da -1
                        temp_times.resize(Station_list.size(), -1);
                        list<Station*> temp_s = main_station_list;
                        //contatore per vector
                        int i = 1;
                        auto p = next(temp_s.begin(), 1);
                        for (p; p != temp_s.end(); p++)
                        {
                            if (temp_times[i] != -1)
                            {
                                int t_teorico = floor((*p)->GetDistance() / 300 * 60);
                                if (temp_times[i] != t_teorico)
                                {
                                    temp_times[i] = t_teorico;
                                }
                            }
                            else
                            {
                                int t_teorico = floor((*p)->GetDistance() / 300 * 60);
                                temp_times[i] = t_teorico;
                            }
                        }
                    }
                    list<int> times{ temp_times.begin(),temp_times.end() };
                    Train_High_Speed_Super* thss = new Train_High_Speed_Super{ id,verse,type,times, this };
                    temp.push_back(thss);
                }
                else
                {
                    cout << "Type error(Train)! " << endl;
                }
            }
            if (verse == 1)
            {
                if (type == 1)
                {
                    //elimino orari in piu
                    if (temp_times.size() >= Station_list.size())
                    {
                        temp_times.resize(Station_list.size());
                        list<Station*> temp_s = get_station_list();
                        reverse(temp_s.begin(), temp_s.end());
                        //contatore per vector
                        int i = 1;
                        int max_distance = get_Last_Station()->GetDistance();
                        auto p = next(temp_s.begin(), 1);
                        for (p; p != temp_s.end(); p++)
                        {
                            //approssimo il tempo per eccesso
                            int t_teorico = floor((max_distance - (*p)->GetDistance()) / 160 * 60);
                            if (temp_times[i] != t_teorico)
                            {
                                temp_times[i] = t_teorico;
                            }
                            i++;
                        }
                    }
                    //aggiungo orari mancanti
                    else if (temp_times.size() >= Station_list.size())
                    {
                        //gli elementi in più sono rappresentati da -1
                        temp_times.resize(Station_list.size(), -1);
                        list<Station*> temp_s = get_station_list();
                        reverse(temp_s.begin(), temp_s.end());
                        //contatore per vector
                        int i = 1;
                        int max_distance = get_Last_Station()->GetDistance();
                        auto p = next(temp_s.begin(), 1);
                        for (p; p != temp_s.end(); p++)
                        {
                            if (temp_times[i] != -1)
                            {
                                int t_teorico = floor((max_distance - (*p)->GetDistance()) / 160 * 60);
                                if (temp_times[i] != t_teorico)
                                {
                                    temp_times[i] = t_teorico;
                                }
                            }
                            else
                            {
                                int t_teorico = floor((*p)->GetDistance() / 160 * 60);
                                temp_times[i] = t_teorico;
                            }
                        }
                    }
                    list<int> times{ temp_times.begin(),temp_times.end() };
                    Train_Regional* tr = new Train_Regional{ id,verse,type,times, this };
                    temp.push_back(tr);
                }
                else if (type == 2)
                {
                    if (temp_times.size() >= Station_list.size())
                    {
                        temp_times.resize(Station_list.size());
                        list<Station*> temp_s = main_station_list;
                        reverse(temp_s.begin(), temp_s.end());
                        //contatore per vector
                        int i = 1;
                        int max_distance = get_Last_Station()->GetDistance();
                        auto p = next(temp_s.begin(), 1);
                        for (p; p != temp_s.end(); p++)
                        {
                            //approssimo il tempo per eccesso
                            int t_teorico = floor((max_distance - (*p)->GetDistance()) / 240 * 60);
                            if (temp_times[i] != t_teorico)
                            {
                                temp_times[i] = t_teorico;
                            }
                            i++;
                        }
                    }
                    else if (temp_times.size() >= Station_list.size())
                    {
                        //gli elementi in più sono rappresentati da -1
                        temp_times.resize(Station_list.size(), -1);
                        list<Station*> temp_s = main_station_list;
                        reverse(temp_s.begin(), temp_s.end());
                        //contatore per vector
                        int i = 1;
                        int max_distance = get_Last_Station()->GetDistance();
                        auto p = next(temp_s.begin(), 1);
                        for (p; p != temp_s.end(); p++)
                        {
                            if (temp_times[i] != -1)
                            {
                                int t_teorico = floor((max_distance - (*p)->GetDistance()) / 240 * 60);
                                if (temp_times[i] != t_teorico)
                                {
                                    temp_times[i] = t_teorico;
                                }
                            }
                            else
                            {
                                int t_teorico = floor((*p)->GetDistance() / 240 * 60);
                                temp_times[i] = t_teorico;
                            }
                        }
                    }
                    list<int> times{ temp_times.begin(),temp_times.end() };
                    Train_High_Speed* ths = new Train_High_Speed{ id,verse,type,times, this };
                    temp.push_back(ths);
                }
                else if (type == 3)
                {
                    if (temp_times.size() >= Station_list.size())
                    {
                        temp_times.resize(Station_list.size());
                        list<Station*> temp_s = get_main_station_list();
                        reverse(temp_s.begin(), temp_s.end());
                        //contatore per vector
                        int i = 1;
                        int max_distance = get_Last_Station()->GetDistance();
                        auto p = next(temp_s.begin(), 1);
                        for (p; p != temp_s.end(); p++)
                        {
                            //approssimo il tempo per eccesso
                            int t_teorico = floor((max_distance - (*p)->GetDistance()) / 300 * 60);
                            if (temp_times[i] != t_teorico)
                            {
                                temp_times[i] = t_teorico;
                            }
                            i++;
                        }
                    }
                    else if (temp_times.size() >= Station_list.size())
                    {
                        //gli elementi in più sono rappresentati da -1
                        temp_times.resize(Station_list.size(), -1);
                        list<Station*> temp_s = main_station_list;
                        reverse(temp_s.begin(), temp_s.end());
                        //contatore per vector
                        int i = 1;
                        int max_distance = get_Last_Station()->GetDistance();
                        auto p = next(temp_s.begin(), 1);
                        for (p; p != temp_s.end(); p++)
                        {
                            if (temp_times[i] != -1)
                            {
                                int t_teorico = floor((max_distance - (*p)->GetDistance()) / 300 * 60);
                                if (temp_times[i] != t_teorico)
                                {
                                    temp_times[i] = t_teorico;
                                }
                            }
                            else
                            {
                                int t_teorico = floor((*p)->GetDistance() / 300 * 60);
                                temp_times[i] = t_teorico;
                            }
                        }
                    }
                    list<int> times{ temp_times.begin(),temp_times.end() };
                    Train_High_Speed_Super* thss = new Train_High_Speed_Super{ id,verse,type,times, this };
                    temp.push_back(thss);
                }
                else
                {
                    cout << "Type error(Train)! " << endl;
                }
            }
        }
        return temp;
    }
    else
    {
        cout << "Failed to open" << endl;
    }
}

//funzione che restituisce la prima stazione(origine)
Station* ReadFile::get_First_Station() const
{
    //list<Station*> temp = modified_station_list();
    auto p = Station_list.begin();
    //Station* ms = new MainStation{ (*p)->GetName(), (*p)->GetType(), (*p)->GetDistance(), *this };
    return *p;
}
//funzione che restituisce l'ultima stazione
Station* ReadFile::get_Last_Station() const
{
    //list<Station*> temp = modified_station_list();
    auto p = prev(Station_list.end(), 1);
    //Station* ms = new MainStation{(*p)->GetName(), (*p)->GetType(), (*p)->GetDistance(), *this};
    return *p;
}

//funzioni che ritornano liste aggiustate
list<Station*> ReadFile::get_station_list() const
{
    return Station_list;
}
list<Train*> ReadFile::get_train_list() const
{
    return Train_list;
}

//funzione che restituisce la lista delle stazioni principali
list<Station*> ReadFile::get_main_station_list() const
{
    list<Station*> temp = Station_list_orig;
    //copy(Station_list_orig.begin(), Station_list_orig.end(), temp);
    auto end = temp.end();
    for (auto p = temp.begin(); p != end; p++)
    {
        if ((*p)->GetType() != 0)
        {
            //delete* p;
            p++;
            temp.erase(prev((p), 1));
            end = temp.end();
        }
    }
    
    return temp;
}

//funzioni che restituisco la stazione dopo/prima ricevento come un parametro Station
//ritorna nullptr nei casi estremi
const Station* ReadFile::nextStation(const Station* s) const
{
    if (s->GetName() == get_Last_Station()->GetName())
    {
        cout << "It the last Station" << endl;
        return nullptr;
    }
    for (auto p = Station_list.begin(); p != Station_list.end(); p++)
    {
        if ((*p)->GetName() == s->GetName())
        {
            return *(next(p, 1));
        }
    }
}
const Station* ReadFile::prevStation(const Station* s) const
{
    if (s->GetName() == get_First_Station()->GetName())
    {
        cout << "It the first Station" << endl;
        return nullptr;
    }
    for (auto p = Station_list.begin(); p != Station_list.end(); p++)
    {
        if ((*p)->GetName() == s->GetName())
        {
            return *(prev(p, 1));
        }
    }
}
