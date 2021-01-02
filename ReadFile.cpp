//file ReadFile.cpp

#include "ReadFile.h"
#include <fstream>

using namespace std;

//construttore
ReadFile::ReadFile()
{
    origin_Station = "";
    Station_list = modify_station_list();
    Train_list = modify_train_list();
}

list<Station> ReadFile::read_Station()
{
    list<Station> temp;
    fstream myfile("line_description.txt,");
    if(myfile.is_open())
    {
        myfile >> origin_Station;

        string name;
        int type;
        int distance;
        for(myfile >> name >> type >> distance)
        {
            Station s (name,type,distance);
            Station_list_orig.push_back(s);
        }
        return temp;
    }
    else
    {
        cout << "Failed to open" << endl;
    }
}

list<Train> ReadFile::read_Train()
{
    list<Train> temp;
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
            Train t (id,verse,type,times);
            temp.push_back(t);
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
        if ((*p).GetType()==0){
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
        if ((*p).GetType()==1)
        {
            count++;
        }
    }
    return count;
}

//rimuovo tutte le stazioni che distano meno di 20 km da quella precedente 
//rimuovo anche 
list<Station> ReadFile::modify_station_list()
{
    list<Station> temp {Station_list_orig.begin(),Station_list_orig.end()};
    auto p = temp.begin();
    if ((*p).GetDistance()<20){
        temp.erase(p);
    }
    for(prev(p);p!=temp.end();p++)
    {
        if(((*p).GetDistance()-(*prev(p)).GetDistance())<20)
        {
            temp.erase(p);
        }
    }
    return temp;
}

//rimuovo orari inappropriati: tempi impossibili per realizzare il percorso con la max velocità del treno
//rimuovo anche orari in più. Gli ultimi sono quelli in più
list<Train> ReadFile::modify_train_list()
{
    list<Train> temp {Train_list_orig.begin(),Train_list_orig.end()};
    //primo ciclo for per rimuovere orari in più
    //se ci sono meno orari, lancia un messaggio
    for(auto p=temp.begin();p!=temp.end();p++)
    {
        if((*p).getType()==1)
        {
            if(((*p).getTimes()).size()>Station_list.size())
            {
                int i=((*p).getTimes()).size()-Station_list.size();
                //funzione della classe Train: rimuove gli ultimi i elementi del list times
                //(*p).reset_timelist(i);
            }
            else if (((*p).getTimes()).size()<Station_list.size())
            {
                //ci sono meno orari di arrivo rispetto al numero di stazioni
                cout << "Mancano degli orari di arrivo" << endl;
            }
        }
        else if((*p).getType()==2||(*p).getType()==3)
        {
            int count=0;
            for (auto p=Station_list.begin();p!=Station_list.end();p++)
            {
                if ((*p).GetType()==0){
                    count++;
                }
            }
            if(((*p).getTimes()).size()>count)
            {
                int i=((*p).getTimes()).size()-Station_list.size();
                //funzione della classe Train: rimuove gli ultimi i elementi del list times
                //(*p).reset_timelist(i);
            }
            else if (((*p).getTimes()).size()<count)
            {
                //ci sono meno orari di arrivo rispetto al numero di stazioni
                cout << "Mancano degli orari di arrivo" << endl;
            }
        }
    }
}

