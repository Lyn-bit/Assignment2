// Stojna Lorei

#include "main_station.h"

#include "Controll.h" //DEBUG

#define PARK_TO_STATION_TIME 4

using namespace std;

MainStation::MainStation(string name, int type, int distance) :
	name_{name}, type_{type}, distance_{distance}, trains_in_station_{}, tracks_state_{0, 0, 0, 0},
	trains_ahead_east_{}, trains_ahead_weast_{}, parked_trains_east_{}, parked_trains_weast_{}
{
}

bool operator==(const MainStation& s_one, const MainStation& s_two)
{
	if (s_one.GetName() == s_two.GetName()) { return true; }
}

// I binari devono essere occupati appena il treno lascia il parcheggio
// (e forse appena fa la riechiesta se gia disponibile)
void MainStation::ArrivalRequest(Train& t)
{ 
	vector<Train>& trains = parked_trains_east_;
	trains = (t.GetVerse() == 0) ? trains = parked_trains_east_ : trains = parked_trains_weast_;

	int estimated_arrival_time = GetEstimatedArrivalTime(t);
	vector<int> track_info = TrackStatus(t);
	int odd_tracks = (trains.size() / 2) + 1;
	int evene_tracks = trains.size() - odd_tracks;
	// se all arrivo trova libero il binario pi� veloce lo metto la
	if (estimated_arrival_time > (track_info[0] + (odd_tracks * 5)))
	{
		t.SetTrack(track_info[1]); t.SetWaitTime(0);
	}
	// se all arrivo trova libero il binario pi� "lento" lo metto la
	else if (estimated_arrival_time > (track_info[2] + (evene_tracks * 5)))
	{
		t.SetTrack(track_info[3]); t.SetWaitTime(0);
	}
	// altri casi
	else
		AddParkedTrain(t);
}

bool MainStation::DepartureRequest(Train& t)
{
	/*devo gestire i casi che la richiesta venga col trenp
	che si ttrova in stazione o in parcheggio*/
	vector<Train>& parked_trains = parked_trains_east_;
	if (t.GetVerse() == 0) { parked_trains = parked_trains_weast_; }

	list<Train>& trains_ahead = trains_ahead_east_;
	if (t.GetVerse() == 0) { trains_ahead = trains_ahead_weast_; }

	// Se si trova in parcheggio devo vedere se il suo binario � stato liberato
	if (t.GetState() == "p")
	{
		// devo fare un pop, aggiungere il treno in stazione e aggioranre lo stato del ritardo
		// a tempo di arrivo dal parcheggio piu 5 minuti di attesa e aggiungerolo alla lista
		// di treni che si trovano tra questa stazione e la succ./prec.
		if (tracks_state_[t.GetTrack()] == 0)
		{
			tracks_state_[t.GetTrack()] = PARK_TO_STATION_TIME + 5;
			trains_in_station_.push_back(parked_trains[1]);
			parked_trains.erase(parked_trains.begin());
			trains_ahead.push_back(t);
			return true;
		}
		// se no tutti gli altri treni devono aumentare l'attesa e anche lui
		else
		{
			int delay = t.GetTimeLeft() + tracks_state_[t.GetTrack()];
			t.SetWaitTime(delay);
			for (int i = t.GetVerse(); i < parked_trains.size(); i += 2)
				parked_trains[i].SetWaitTime(parked_trains[i].GetTimeLeft() + delay);
			return false;
		}
	}

	// Se si trova in stazione constrollo che non ci siano treni a meno di 10km di fronte
	// e se il treno nel altro binario ha priorit� maggiore
	if (t.GetState() == "s")
	{
		// Controllo se laltro treno h auna priorita piu alta
		for (auto i = trains_in_station_.begin(); i != trains_in_station_.end(); i++)
			if (i->GetTrack() == t.GetTrack() and ComparePriority(*i, t))
			{
				int delay = i->GetTimeLeft();
				for (int i = t.GetVerse(); i < parked_trains.size(); i += 2)
					parked_trains[i].SetWaitTime(parked_trains[i].GetTimeLeft() + delay);
				return false;
			}
		// Se non pu� partire perche ci sono treni davanti aumnetare il ritardo del treno e di quelli
		// che lo precedono nel parcheggio
		if (abs(distance_ - trains_ahead.back().GetPos()) < 10)
		{
			// Controllo se laltro treno h auna priorita piu alta
			
			int delay = TimeToFree(trains_ahead, *this); ////////////
			// aggiorno il tempo di attesa in stazione
			t.SetWaitTime(t.GetTimeLeft() + delay);
			tracks_state_[t.GetTrack()] += delay;
			// aggiungo il delay ai treni che hanno lo stesso binario
			for (int i = t.GetVerse(); i < parked_trains.size(); i+=2)
				parked_trains[i].SetWaitTime(parked_trains[i].GetTimeLeft() + delay);
			return false;
		}
		// Se pu� partire
		else
		{
			tracks_state_[t.GetTrack()] = 0;
			trains_in_station_.remove(t);
			trains_ahead.push_back(t);
			return true;
		}
	}
}

void MainStation::Update()
{
	// Decrementa il contatore del tempo passato nei binari
	for (int i = 0; i < 4; i++)
		if (tracks_state_[i] > 0)
			tracks_state_[i] -= 1;
	
	// Rimuovono i treni che si trovano nella stazione successiva
	for (auto i = trains_ahead_east_.begin(); i != trains_ahead_east_.end(); i++)
		if (i->GetPos() <= (f::GetNext(*this).GetDistance() + 5))
			trains_ahead_east_.erase(i);

	for (auto i = trains_ahead_weast_.begin(); i != trains_ahead_weast_.end(); i++)
		if (i->GetPos() <= (f::GetPrev(*this).GetDistance() - 5))
			trains_ahead_weast_.erase(i);
}

void MainStation::PrintDepartureTime(const Train& t, int time) const {}
void MainStation::PrintArrivalTime(const Train& t, int time) const {}


vector<int> MainStation::TrackStatus(const Train& t) const
{
	int verse = t.GetVerse();
	const vector<Train> *trains = &parked_trains_east_;
	trains = (verse == 0) ? trains = &parked_trains_east_ : trains = &parked_trains_weast_;
	
	int wait_time_track_one{ 0 };
	int wait_time_track_two{ 0 };
	
	wait_time_track_one = tracks_state_[(2 * verse)];
	wait_time_track_two = tracks_state_[1 + (2 * verse)];
	
	for (int i = 0; i < (trains->size()); i++)
	{
		if (trains->at(i).GetTrack() == (2 * verse))
			wait_time_track_one += trains->at(i).GetTimeLeft();
		else
			wait_time_track_two += trains->at(i).GetTimeLeft();
	}
	
	// prime due posizioni attesa in muinuti del binario pi� veloce
	// e il numero di binario
	//
	// Ultime due il tempo binario pi� lento e numero
	vector<int> status(4);
	if (wait_time_track_one <= wait_time_track_two)
	{
		status[0] = wait_time_track_one; status[1] = (2 * verse);
		status[2] = wait_time_track_two; status[3] = (1 + (2 * verse));
	}
	else
	{
		status[0] = wait_time_track_two; status[1] = (1 + (2 * verse));
		status[2] = wait_time_track_one; status[3] = (2 * verse);
	}

	// max minuti attesa, binario
	return status;
}

int MainStation::GetEstimatedArrivalTime(const Train& t) const
{
	int position = t.GetPos();
	int speed = t.GetMaxSpeed();

	// Il tempo minimo d'arrivo
	// (se per esempio ha un treno davanti che va pi� lento
	// allora arriver� pi� tardi ma comunque non prima del treno che o precede
	int d_to_park = abs(distance_ - position) - 5;
	int time_to_station = (d_to_park / speed) + PARK_TO_STATION_TIME;

	return time_to_station;
}

// l'array parked_trains cresce ogni volta che si fa insert
// Inserisce il treno nella lista e aggiorna direttamente il tempo d�attesa del treno
// return il binario del treno
void MainStation::AddParkedTrain(Train& t)
{
	vector<Train>& trains = parked_trains_east_;
	trains = (t.GetVerse() == 0) ? trains = parked_trains_east_ : trains = parked_trains_weast_;

	int estimated_arrival = GetEstimatedArrivalTime(t);
	// Controlla solo il binario pi� veloce
	int track = TrackStatus(t)[1];
	// Imposta subito il binario del treno
	t.SetTrack(track);

	for (; track < trains.size(); track+=2)
	{
		// devo vedere se il tempo d'arrivo � < o > dei treni che lo confronto
		if (ComparePriority(t, trains[track]) and estimated_arrival < trains[track].GetTimeLeft())
		{
			trains.insert(trains.begin() + track, t);
			break;
		}
	}
	trains.push_back(t);

	/*
	impostare il tempo d'attesa del treno appena aggiunto
	*/
	int wait_time{ 0 };
	int i = TrackStatus(t)[1] % 2;
	for (; i < track or i < trains.size(); i += 2)
		wait_time += trains[track].GetWaitTime() + 5;
	t.SetWaitTime(wait_time);

	// Aggiorno l'attesa per gli eventuali treni che sono stati spostati 
	// a priorit� pi� bassa
	/*devo sapere l'orario d'arrivo in parcheggio del treno*/
	int time_to_add = wait_time + 5;
	if (track < trains.size())
		for (; track < trains.size(); track += 2)
		{
			int prev_time = trains[track].GetWaitTime();
			trains[track].SetWaitTime(prev_time + time_to_add);
		}
	else
	{
		trains.push_back(t);
	}
}

// il primo treno � quello a 20km o l'altro treno che si trova in stazione,
// il secondo o in parcheggio o stazione
// True se t1 ha priorit� pi� alta
// False altrimenti
bool MainStation::ComparePriority(const Train& t1, const Train& t2) const
{
	if (t2.GetWaitTime() >= t2.GetMaxWait())
		return false;

	if (t1.GetType() > t2.GetType())
		return true;

	return false;

}


int TimeToFree(const list<Train>& trains_ahead, const MainStation& s)
{
	int pos = abs(s.GetDistance() - trains_ahead.back().GetPos());

	int s_km{ 0 };
	int f_km{ 5 };
	if (pos < 5) { s_km = pos - 5; }
	else { f_km = (pos - 5) + 5; }

	// nei 5km "veloci" utilizza la veloci� del treno regionale
	// cosi nel caso pessimo il treno aspettera del tempo in pi�
	// ma i 10km saranno sempre risettati
	int time = (s_km / 80) + (f_km / 160);

	return time;
}

