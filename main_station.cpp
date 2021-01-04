// Stojna Lorei

#include "main_station.h"

//#include "Controll.h" //DEBUG

#define PARK_TO_STATION_TIME 4

using namespace std;

MainStation::MainStation(string name, int type, int distance, const ReadFile &read_file) :
	name_{name}, type_{type}, distance_{distance}, position_{-1}, read_file_{read_file},
	trains_in_station_{}, tracks_state_{0, 0, 0, 0},
	trains_ahead_east_{}, trains_ahead_weast_{}, parked_trains_east_{}, parked_trains_weast_{}
{
	if (read_file.get_first_Station() == *this) { position_ = 1; }
	else if (read_file.get_last_Station() == *this) { position_= 2; }
}



// I binari devono essere occupati appena il treno lascia il parcheggio
// (e forse appena fa la riechiesta se gia disponibile)
void MainStation::ArrivalRequest(Train& t)
{ 
	/*
	* Se stazione origine o capolinea ( in base al verso del treno)
	* devono comunque stare i 5 min per far scendere i passeggieri
	* La cosa diversa è che la lista trains_aheadeast/east e train_p_east/weast (se orig. o capol.)
	* non ci saranno
	*/

	vector<Train&>& trains = parked_trains_east_;
	trains = (t.getVerse() == 0) ? trains = parked_trains_east_ : trains = parked_trains_weast_;

	int estimated_arrival_time = GetEstimatedArrivalTime(t);
	vector<int> track_info = TrackStatus(t);
	int odd_tracks = (trains.size() / 2) + 1;
	int evene_tracks = trains.size() - odd_tracks;
	// se all arrivo trova libero il binario più veloce lo metto la
	if (estimated_arrival_time > (track_info[0] + (odd_tracks * 5)))
	{
		t.SetTrack(track_info[1]); t.SetWaitTime(0);
	}
	// se all arrivo trova libero il binario più "lento" lo metto la
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
	/*
	* 
	* Una Volta arrivati al capolinea/origine (in base al verso) non devono più partire ma scompaiono e basta
	* 
	*/


	/*devo gestire i casi che la richiesta venga col trenp
	che si ttrova in stazione o in parcheggio*/
	vector<Train&>& parked_trains = parked_trains_east_;
	if (t.getVerse() == 0) { parked_trains = parked_trains_weast_; }

	list<const Train&>& trains_ahead = trains_ahead_east_;
	if (t.getVerse() == 0) { trains_ahead = trains_ahead_weast_; }

	// Se si trova in parcheggio devo vedere se il suo binario è stato liberato
	if (t.GetState() == "p")
	{
		// devo fare un pop, aggiungere il treno in stazione e aggioranre lo stato del ritardo
		// a tempo di arrivo dal parcheggio piu 5 minuti di attesa e aggiungerolo alla lista
		// di treni che si trovano tra questa stazione e la succ./prec.
		if (tracks_state_[t.GetTrack()] == 0)
		{
			tracks_state_[t.GetTrack()] = PARK_TO_STATION_TIME + 5;
			trains_in_station_.push_back(parked_trains[0]);
			parked_trains.erase(parked_trains.begin());
			// Controlla se è stazione origine o capolinea 
			if (t.getVerse() == 0 && position_ != 2 || t.getVerse() == 1 && position_ != 1)
				trains_ahead.push_back(t);
			return true;
		}
		// se no tutti gli altri treni devono aumentare l'attesa e anche lui
		else
		{
			int delay = t.GetTimeLeft() + tracks_state_[t.GetTrack()];
			t.SetWaitTime(delay);
			for (int i = t.getVerse(); i < parked_trains.size(); i += 2)
				parked_trains[i].SetWaitTime(parked_trains[i].GetTimeLeft() + delay);
			return false;
		}
	}

	// Se si trova in stazione constrollo che non ci siano treni a meno di 10km di fronte
	// e se il treno nel altro binario ha priorità maggiore
	if (t.GetState() == "s")
	{
		// Controllo se laltro treno h auna priorita piu alta
		for (auto i = trains_in_station_.begin(); i != trains_in_station_.end(); i++)
			if (i->GetTrack() == t.GetTrack() && ComparePriority(*i, t))
			{
				int delay = i->GetTimeLeft();
				for (int i = t.getVerse(); i < parked_trains.size(); i += 2)
					parked_trains[i].SetWaitTime(parked_trains[i].GetTimeLeft() + delay);
				return false;
			}
		// Se non può partire perche ci sono treni davanti aumnetare il ritardo del treno e di quelli
		// che lo precedono nel parcheggio
		if (abs(distance_ - trains_ahead.back().getPosition()) < 5)
		{
			// Controllo se laltro treno h auna priorita piu alta
			
			int delay = TimeToFree(trains_ahead, *this); ////////////
			// aggiorno il tempo di attesa in stazione
			t.SetWaitTime(t.GetTimeLeft() + delay);
			tracks_state_[t.GetTrack()] += delay;
			// aggiungo il delay ai treni che hanno lo stesso binario
			for (int i = t.getVerse(); i < parked_trains.size(); i+=2)
				parked_trains[i].SetWaitTime(parked_trains[i].GetTimeLeft() + delay);
			return false;
		}
		// Se può partire
		else
		{
			tracks_state_[t.GetTrack()] = 0;
			trains_in_station_.remove(t);
			// Controlla se è stazione origine o capolinea 
			if (t.getVerse() == 0 && position_ != 2 || t.getVerse() == 1 && position_ != 1)
				trains_ahead.push_back(t);
			return true;
		}
	}
}

void MainStation::Update()
{
	/*
	*
	* Una Volta arrivati al capolinea/origine (in base al verso) non devono più partire ma scompaiono e basta
	*
	*/

	// Decrementa il contatore del tempo passato nei binari
	for (int i = 0; i < 4; i++)
		if (tracks_state_[i] > 0)
			tracks_state_[i] -= 1;
	// Se stazione origine o capolinea devo rimuovere i treni dalla stazione
	if (position_ == 1) // Se stazione origine
	{
		for (auto i = trains_in_station_.begin(); i != trains_in_station_.end(); i++)
			if (tracks_state_[i->GetTrack()] == 0 && i->getVerse() == 1)
				trains_in_station_.erase(i);
	}
	else if (position_ == 2) // Se stazione capolinea
	{
		for (auto i = trains_in_station_.begin(); i != trains_in_station_.end(); i++)
			if (tracks_state_[i->GetTrack()] == 0 && i->getVerse() == 0)
				trains_in_station_.erase(i);
	}
	
	// Rimuovono i treni partiti che si trovano gia' nella stazione successiva
	for (auto i = trains_ahead_east_.begin(); i != trains_ahead_east_.end(); i++)
		if (i->getPosition() <= (read_file_.nextStation(this)->GetDistance() + 5))
			trains_ahead_east_.erase(i);

	for (auto i = trains_ahead_weast_.begin(); i != trains_ahead_weast_.end(); i++)
		if (i->getPosition() <= (read_file_.prevStation(this)->GetDistance() - 5))
			trains_ahead_weast_.erase(i);
}

int MainStation::GetNextTrain(const Train& t) const
{
	/*
	*
	* capolinea/origine 
	*
	*/
	list<const Train&> trains = trains_ahead_east_;
	if (t.getVerse() == 0) { trains = trains_ahead_weast_; }

	auto it_next_train = find(trains.begin(), trains.end(), t);
	if (it_next_train == trains.end())
		return -1;
	else
		return it_next_train->getPosition();
}

void MainStation::PrintDepartureTime(const Train& t, int time) const 
{
	/*Non completo( manca caso ultima stazione)*/
	string next_station = (t.getVerse() == 0) ? read_file_.nextStation(this)->GetName() : read_file_.prevStation(this)->GetName();
	
	string train_type;
	if (t.getType() == 1) { train_type = "Regionale"; }
	else if (t.getType() == 2) { train_type = "Alta Velocita'"; }
	else { train_type = "Alta Velocita' Super"; }
	cout << "Il treno " + train_type + " numero " << t.getId() << " diretto a " + next_station << " e' in partenza dal binario " << t.GetTrack() << "\t" + FormatTime(time) << endl;
}
void MainStation::PrintArrivalTime(const Train& t, int time) const 
{
	/*
	* potrei fare print quando il treno fa la richiesta ai 20km dicendo quando arrivera' e
	* quando parte dalla stazione
	*/
}


vector<int> MainStation::TrackStatus(const Train& t) const
{
	/*
	*
	* capolinea/origine
	*
	*/
	int verse = t.getVerse();
	const vector<Train&> *trains = (verse == 0) ? parked_trains_east_ : parked_trains_weast_;
	
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
	
	// prime due posizioni attesa in muinuti del binario più veloce
	// e il numero di binario
	//
	// Ultime due il tempo binario più lento e numero
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
	int position = t.getPosition();
	int speed = t.getMaxSpeed();

	// Il tempo minimo d'arrivo
	// (se per esempio ha un treno davanti che va più lento
	// allora arriverà più tardi ma comunque non prima del treno che o precede
	int d_to_park = abs(distance_ - position) - 5;
	int time_to_station = (d_to_park / speed) + PARK_TO_STATION_TIME;

	return time_to_station;
}

// l'array parked_trains cresce ogni volta che si fa insert
// Inserisce il treno nella lista e aggiorna direttamente il tempo dìattesa del treno
void MainStation::AddParkedTrain(Train& t)
{
	/*
	*
	* capolinea/origine
	*
	*/
	vector<Train&>& trains = parked_trains_east_;
	trains = (t.getVerse() == 0) ? trains = parked_trains_east_ : trains = parked_trains_weast_;

	int estimated_arrival = GetEstimatedArrivalTime(t);
	// Controlla solo il binario più veloce
	int track = TrackStatus(t)[1];
	// Imposta subito il binario del treno
	t.SetTrack(track);

	for (; track < trains.size(); track+=2)
	{
		// devo vedere se il tempo d'arrivo è < o > dei treni che lo confronto
		if (ComparePriority(t, trains[track]) && estimated_arrival < trains[track].GetTimeLeft())
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
	for (; i < track || i < trains.size(); i += 2)
		wait_time += trains[track].GetTimeLeft() + 5;
	t.SetWaitTime(wait_time);

	// Aggiorno l'attesa per gli eventuali treni che sono stati spostati 
	// a priorità più bassa
	/*devo sapere l'orario d'arrivo in parcheggio del treno*/
	int time_to_add = wait_time + 5;
	if (track < trains.size())
		for (; track < trains.size(); track += 2)
		{
			int prev_time = trains[track].GetTimeLeft();
			trains[track].SetWaitTime(prev_time + time_to_add);
		}
	else
	{
		trains.push_back(t);
	}
}

// il primo treno è quello a 20km o l'altro treno che si trova in stazione,
// il secondo o in parcheggio o stazione
// True se t1 ha priorità più alta
// False altrimenti
bool MainStation::ComparePriority(const Train& t1, const Train& t2) const
{
	if (t2.GetWaitTime() >= t2.getMaxWait())
		return false;

	if (t1.getType() > t2.getType())
		return true;

	return false;

}


int TimeToFree(const list<const Train&>& trains_ahead, const MainStation& s)
{
	int pos = abs(s.GetDistance() - trains_ahead.back().getPosition());

	int s_km{ 0 };
	int f_km{ 5 };
	if (pos < 5) { s_km = pos - 5; }
	else { f_km = (pos - 5) + 5; }

	// nei 5km "veloci" utilizza la velocià del treno regionale
	// cosi nel caso pessimo il treno aspettera del tempo in più
	// ma i 10km saranno sempre risettati
	int time = (s_km / 80) + (f_km / 160);

	return time;
}

string FormatTime(int min)
{
	int minutes = min % 60;
	int hour = min / 60;
	int days = ((hour / 24) % 23);
	hour -= days * 24;

	string m = (minutes < 10) ? "0" + to_string(minutes) : to_string(minutes);
	string h = (hour < 10) ? "0" + to_string(hour) : to_string(hour);
	string d = (days > 0) ? " Giorno +" + to_string(days) : "";

	string time = h + ":" + m + d;

	return time;
}

