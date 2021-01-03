//file ReadFile.cpp

#include "ReadFile.h"
#include <fstream>
#include <math.h>

using namespace std;

//construttore
ReadFile::ReadFile()
{
    auto p=modified_station_list().begin();
    origin_Station = (*p)->GetName();
    auto q=prev(modified_station_list().end(),1);
    last_Station = (*q)->GetName();
    Station_list_orig = read_Station();
    Train_list_orig = read_Train();
    Station_list = modified_station_list();
    Train_list = modified_train_list();
}

list<Station*> ReadFile::read_Station()
{
    list<Station*> temp;
    fstream myfile("line_description.txt,");
    if(myfile.is_open())
    {
        myfile >> origin_Station;
        MainStation *ms = new MainStation{origin_Station,0,0};
        temp.push_back(ms);

        string name;
        int type;
        int distance;
        while(myfile >> name >> type >> distance)
        {
            if(type==0)
            {
                MainStation *ms = new MainStation{name,type,distance/*,*this*/};
                temp.push_back(ms);
            }
            else if(type==1)
            {
                LocalStation *ls = new LocalStation{name,type,distance};
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
    if(myfile.is_open())
    {
        while(getline(myfile,line))
        {
            vector<int> temp_vect;
            int i=0;
            while(i<line.size())
            {
                string str="";
                while(isdigit(line[i]))
                {
                char c = line[i];
                str += c;
                i++;
                }
                if (str!="")
                {
                    i++;
                }
                temp_vect.push_back(stoi(str));
            }
            int id = temp_vect[0];
            int verse = temp_vect[1];
            int type = temp_vect[2];
            list<int> times {next(temp_vect.begin(),3),temp_vect.end()};
            if (type==1)
            {
                Train_Regional *tr = new Train_Regional{id,verse,type,times};
                temp.push_back(tr);
            }
            else if (type==2)
            {
                Train_High_Speed *ths = new Train_High_Speed{id,verse,type,times};
                temp.push_back(ths);
            }
            else if (type==3)
            {
                Train_High_Speed_Super *thss = new Train_High_Speed_Super{id,verse,type,times};
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

int ReadFile::get_num_of_mainstation()
{
    int count=0;
    for (auto p=Station_list.begin();p!=Station_list.end();p++)
    {
        if ((*p)->GetType()==0){
            count++;
        }
    }
    return count;
}

int ReadFile::get_num_of_localstation()
{
    int count=0;
    for (auto p=Station_list.begin();p!=Station_list.end();p++)
    {
        if ((*p)->GetType()==1)
        {
            count++;
        }
    }
    return count;
}

//rimuovo tutte le stazioni che distano meno di 20 km da quella precedente 
list<Station*> ReadFile::modified_station_list()
{
    list<Station*> temp;
    copy(Station_list_orig.begin(),Station_list_orig.end(),temp);
    for(auto p=temp.begin();p!=temp.end();p++)
    {
        if((*p)->GetDistance()-(*prev(p))->GetDistance()<20)
        {
            delete *p;
            temp.erase(p);
        }
    }
    return temp;
}

//aggiustare orari inappropriati: tempi impossibili per realizzare il percorso con la max velocità del treno
//rimuovo anche orari in più. considero gli ultimi come quelli in più
//se ci sono piu stazioni, verrano aggiunti orari
list<Train*> ReadFile::modified_train_list()
{
    list<Train*> temp;
    string line;
    fstream myfile("timetables.txt");
    if(myfile.is_open())
    {
        while(getline(myfile,line))
        {
            vector<int> temp_vect;
            int i=0;
            while(i<line.size())
            {
                string str="";
                while(isdigit(line[i]))
                {
                char c = line[i];
                str += c;
                i++;
                }
                if (str!="")
                {
                    i++;
                }
                temp_vect.push_back(stoi(str));
            }
            int id = temp_vect[0];
            int verse = temp_vect[1];
            int type = temp_vect[2];
            vector<int> temp_times {next(temp_vect.begin(),3),temp_vect.end()};            
            if(verse==0)
            {
                if (type==1)
                {
                //elimino orari in piu
                    if(temp_times.size()>=Station_list.size())
                    {
                        temp_times.resize(Station_list.size());
                        list<Station*> temp_s = get_station_list();
                        //contatore per vector
                        int i=1;
                        auto p=next(temp_s.begin(),1);
                        for (p;p!=temp_s.end();p++)
                        {
                            //approssimo il tempo per eccesso
                            int t_teorico = floor((*p)->GetDistance()/160);
                            if (temp_times[i]!=t_teorico)
                            {
                                temp_times[i]=t_teorico;
                            }
                            i++;
                        }
                    }
                     //aggiungo orari mancanti
                    else if (temp_times.size()>=Station_list.size())
                    {
                        //gli elementi in più sono rappresentati da -1
                        temp_times.resize(Station_list.size(),-1);
                        list<Station*> temp_s = get_station_list();
                        //contatore per vector
                        int i=1;
                        auto p=next(temp_s.begin(),1);
                        for (p;p!=temp_s.end();p++)
                        {
                            if(temp_times[i]!=-1)
                            {
                                int t_teorico = floor((*p)->GetDistance()/160);
                                if (temp_times[i]!=t_teorico)
                                {
                                    temp_times[i]=t_teorico;
                                }
                            }
                            else
                            {
                                int t_teorico = floor((*p)->GetDistance()/160);
                                temp_times[i]=t_teorico;
                            }
                        }
                    }
                    list<int> times{temp_times.begin(),temp_times.end()};
                    Train_Regional *tr = new Train_Regional{id,verse,type,times};
                    temp.push_back(tr);
                }
                else if (type==2)
                {
                    if(temp_times.size()>=Station_list.size())
                    {
                        temp_times.resize(Station_list.size());
                        list<Station*> temp_s = get_main_station_list();
                        //contatore per vector
                        int i=1;
                        auto p=next(temp_s.begin(),1);
                        for (p;p!=temp_s.end();p++)
                        {
                            //approssimo il tempo per eccesso
                            int t_teorico = floor((*p)->GetDistance()/240);
                            if (temp_times[i]!=t_teorico)
                            {
                                temp_times[i]=t_teorico;
                            }
                            i++;
                        }
                    }
                    else if (temp_times.size()>=Station_list.size())
                    {
                        //gli elementi in più sono rappresentati da -1
                        temp_times.resize(Station_list.size(),-1);
                        list<Station*> temp_s = get_main_station_list();
                        //contatore per vector
                        int i=1;
                        auto p=next(temp_s.begin(),1);
                        for (p;p!=temp_s.end();p++)
                        {
                            if(temp_times[i]!=-1)
                            {
                                int t_teorico = floor((*p)->GetDistance()/240);
                                if (temp_times[i]!=t_teorico)
                                {
                                    temp_times[i]=t_teorico;
                                }
                            }
                            else
                            {
                                int t_teorico = floor((*p)->GetDistance()/240);
                                temp_times[i]=t_teorico;
                            }
                        }
                    }
                    list<int> times{temp_times.begin(),temp_times.end()};
                    Train_High_Speed *ths = new Train_High_Speed{id,verse,type,times};
                    temp.push_back(ths);
                }
                else if (type==3)
                {
                    if(temp_times.size()>=Station_list.size())
                    {
                        temp_times.resize(Station_list.size());
                        list<Station*> temp_s = get_main_station_list();
                        //contatore per vector
                        int i=1;
                        auto p=next(temp_s.begin(),1);
                        for (p;p!=temp_s.end();p++)
                        {
                            //approssimo il tempo per eccesso
                            int t_teorico = floor((*p)->GetDistance()/300);
                            if (temp_times[i]!=t_teorico)
                            {
                                temp_times[i]=t_teorico;
                            }
                            i++;
                        }
                    }
                    else if (temp_times.size()>=Station_list.size())
                    {
                        //gli elementi in più sono rappresentati da -1
                        temp_times.resize(Station_list.size(),-1);
                        list<Station*> temp_s = get_main_station_list();
                        //contatore per vector
                        int i=1;
                        auto p=next(temp_s.begin(),1);
                        for (p;p!=temp_s.end();p++)
                        {
                            if(temp_times[i]!=-1)
                            {
                                int t_teorico = floor((*p)->GetDistance()/300);
                                if (temp_times[i]!=t_teorico)
                                {
                                    temp_times[i]=t_teorico;
                                }
                            }
                            else
                            {
                                int t_teorico = floor((*p)->GetDistance()/300);
                                temp_times[i]=t_teorico;
                            }
                        }
                    }
                    list<int> times{temp_times.begin(),temp_times.end()};
                    Train_High_Speed_Super *thss = new Train_High_Speed_Super{id,verse,type,times};
                    temp.push_back(thss);
                }
                else
                {
                    cout << "Type error(Train)! " << endl;
                }
            }
            if(verse==1)
            {    
                if (type==1)
                {
                    //elimino orari in piu
                    if(temp_times.size()>=Station_list.size())
                    {
                        temp_times.resize(Station_list.size());
                        list<Station*> temp_s = get_station_list();
                        reverse(temp_s.begin(),temp_s.end());
                        //contatore per vector
                        int i=1;
                        int max_distance = get_last_Station().GetDistance();
                        auto p=next(temp_s.begin(),1);
                        for (p;p!=temp_s.end();p++)
                        {
                            //approssimo il tempo per eccesso
                            int t_teorico = floor((max_distance-(*p)->GetDistance())/160);
                            if (temp_times[i]!=t_teorico)
                            {
                                temp_times[i]=t_teorico;
                            }
                            i++;
                        }
                    }
                    //aggiungo orari mancanti
                    else if (temp_times.size()>=Station_list.size())
                    {
                        //gli elementi in più sono rappresentati da -1
                        temp_times.resize(Station_list.size(),-1);
                        list<Station*> temp_s = get_station_list();
                        reverse(temp_s.begin(),temp_s.end());
                        //contatore per vector
                        int i=1;
                        int max_distance = get_last_Station().GetDistance();
                        auto p=next(temp_s.begin(),1);
                        for (p;p!=temp_s.end();p++)
                        {
                            if(temp_times[i]!=-1)
                            {
                                int t_teorico = floor((max_distance-(*p)->GetDistance())/160);
                                if (temp_times[i]!=t_teorico)
                                {
                                    temp_times[i]=t_teorico;
                                }
                            }
                            else
                            {
                                int t_teorico = floor((*p)->GetDistance()/160);
                                temp_times[i]=t_teorico;
                            }
                        }
                    }
                    list<int> times{temp_times.begin(),temp_times.end()};
                    Train_Regional *tr = new Train_Regional{id,verse,type,times};
                    temp.push_back(tr);
                }
                else if (type==2)
                {
                    if(temp_times.size()>=Station_list.size())
                    {
                        temp_times.resize(Station_list.size());
                        list<Station*> temp_s = get_main_station_list();
                        reverse(temp_s.begin(),temp_s.end());
                        //contatore per vector
                        int i=1;
                        int max_distance = get_last_Station().GetDistance();
                        auto p=next(temp_s.begin(),1);
                        for (p;p!=temp_s.end();p++)
                        {
                            //approssimo il tempo per eccesso
                            int t_teorico = floor((max_distance-(*p)->GetDistance())/240);
                            if (temp_times[i]!=t_teorico)
                            {
                                temp_times[i]=t_teorico;
                            }
                            i++;
                        }
                    }
                    else if (temp_times.size()>=Station_list.size())
                    {
                        //gli elementi in più sono rappresentati da -1
                        temp_times.resize(Station_list.size(),-1);
                        list<Station*> temp_s = get_main_station_list();
                        reverse(temp_s.begin(),temp_s.end());
                        //contatore per vector
                        int i=1;
                        int max_distance = get_last_Station().GetDistance();
                        auto p=next(temp_s.begin(),1);
                        for (p;p!=temp_s.end();p++)
                        {
                            if(temp_times[i]!=-1)
                            {
                                int t_teorico = floor((max_distance-(*p)->GetDistance())/240);
                                if (temp_times[i]!=t_teorico)
                                {
                                    temp_times[i]=t_teorico;
                                }
                            }
                            else
                            {
                                int t_teorico = floor((*p)->GetDistance()/240);
                                temp_times[i]=t_teorico;
                            }
                        }
                    }
                    list<int> times{temp_times.begin(),temp_times.end()};
                    Train_High_Speed *ths = new Train_High_Speed{id,verse,type,times};
                    temp.push_back(ths);
            }
                else if (type==3)
                {
                    if(temp_times.size()>=Station_list.size())
                    {
                        temp_times.resize(Station_list.size());
                        list<Station*> temp_s = get_main_station_list();
                        reverse(temp_s.begin(),temp_s.end());
                        //contatore per vector
                        int i=1;
                        int max_distance = get_last_Station().GetDistance();
                        auto p=next(temp_s.begin(),1);
                        for (p;p!=temp_s.end();p++)
                        {
                            //approssimo il tempo per eccesso
                            int t_teorico = floor((max_distance-(*p)->GetDistance())/300);
                            if (temp_times[i]!=t_teorico)
                            {
                                temp_times[i]=t_teorico;
                            }
                            i++;
                        }
                    }
                    else if (temp_times.size()>=Station_list.size())
                    {
                        //gli elementi in più sono rappresentati da -1
                        temp_times.resize(Station_list.size(),-1);
                        list<Station*> temp_s = get_main_station_list();
                        reverse(temp_s.begin(),temp_s.end());
                        //contatore per vector
                        int i=1;
                        int max_distance = get_last_Station().GetDistance();
                        auto p=next(temp_s.begin(),1);
                        for (p;p!=temp_s.end();p++)
                        {
                            if(temp_times[i]!=-1)
                            {
                                int t_teorico = floor((max_distance-(*p)->GetDistance())/300);
                                if (temp_times[i]!=t_teorico)
                                {
                                    temp_times[i]=t_teorico;
                                }
                            }
                            else
                            {
                                int t_teorico = floor((*p)->GetDistance()/300);
                                temp_times[i]=t_teorico;
                            }
                        }
                    }
                    list<int> times{temp_times.begin(),temp_times.end()};
                    Train_High_Speed_Super *thss = new Train_High_Speed_Super{id,verse,type,times};
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
MainStation ReadFile::get_first_Station()
{
    list<Station*> temp = modified_station_list();
    auto p=temp.begin();
    MainStation ms {(*p)->GetName(),(*p)->GetType(),(*p)->GetDistance()};
    return ms;
}
//funzione che restituisce l'ultima stazione
MainStation ReadFile::get_last_Station()
{
    list<Station*> temp = modified_station_list();
    auto p=prev(temp.begin(),1);
    MainStation ms {(*p)->GetName(),(*p)->GetType(),(*p)->GetDistance()};
    return ms;
}

//funzioni che ritornano liste aggiustate
list<Station*> ReadFile::get_station_list()
{
    return Station_list;
}
list<Train*> ReadFile::get_train_list()
{
    return Train_list;
}

//funzione che restituisce la lista delle stazioni principali
list<Station*> ReadFile::get_main_station_list()
{
    list<Station*> temp;
    copy(Station_list_orig.begin(),Station_list_orig.end(),temp);
    for(auto p=Station_list.begin();p!=Station_list.end();p++)
    {
        if((*p)->GetType()!=0)
        {
            delete *p;
            temp.erase(p);
        }
    }
    return temp;
}

