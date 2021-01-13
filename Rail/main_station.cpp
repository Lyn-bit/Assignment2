// Stojna Lorei

#include "station.h"

#define PARK_TO_STATION_TIME 4

using namespace std;

MainStation::MainStation(string name, int type, int distance, const ReadFile &read_file) : name_{name}, type_{type}, distance_{distance}, read_file_{read_file},
																						   trains_in_station_{}, tracks_state_{0, 0, 0, 0, 0},
																						   trains_ahead_east_{}, trains_ahead_weast_{}, parked_trains_east_{}, parked_trains_weast_{}
{
}

void MainStation::ArrivalRequest(Train *t, const ReadFile &file)
{
	vector<Train *> &trains = parked_trains_east_;
	trains = (t->getVerse() == 0) ? trains = parked_trains_east_ : trains = parked_trains_weast_;

	int estimated_arrival_time = GetEstimatedArrivalTime(t);
	vector<int> track_info = TrackStatus(t);
	int odd_tracks = (trains.size() / 2) + 1;
	int evene_tracks = trains.size() - odd_tracks;

	if (estimated_arrival_time > (track_info[0] + (odd_tracks * 5)))
	{
		t->SetTrack(track_info[1] + 1);
		t->SetWaitTime(0);
		t->setState("s");
	}
	else
		AddParkedTrain(t);
}

bool MainStation::DepartureRequest(Train *t, const ReadFile &file)
{
	vector<Train *> &parked_trains = parked_trains_east_;
	if (t->getVerse() == 1)
	{
		parked_trains = parked_trains_weast_;
	}

	list<const Train *> &trains_ahead = trains_ahead_east_;
	if (t->getVerse() == 1)
	{
		trains_ahead = trains_ahead_weast_;
	}

	if (t->getState() == "p")
	{
		if (tracks_state_[t->GetTrack()] == 0)
		{
			tracks_state_[t->GetTrack()] = PARK_TO_STATION_TIME + 5;
			trains_in_station_.push_back(parked_trains[0]);
			if (parked_trains.size() > 0)
				parked_trains.erase(parked_trains.begin());
			if (t->getVerse() == 0 && Position(file) != 2 || t->getVerse() == 1 && Position(file) != 1)
				trains_ahead.push_back(t);
			t->setState("s");
			return true;
		}
		else
		{
			int delay = t->GetTimeLeft() + tracks_state_[t->GetTrack()];
			t->SetWaitTime(delay);
			for (int i = t->getVerse(); i < parked_trains.size(); i += 2)
				parked_trains[i]->SetWaitTime(parked_trains[i]->GetTimeLeft() + delay);
			return false;
		}
	}

	if (t->getState() == "s")
	{
		for (auto i = trains_in_station_.begin(); i != trains_in_station_.end(); i++)
			if ((*i)->GetTrack() == t->GetTrack() && ComparePriority(*i, t))
			{
				int delay = (*i)->GetTimeLeft();
				for (int i = t->getVerse(); i < parked_trains.size(); i += 2)
					parked_trains[i]->SetWaitTime(parked_trains[i]->GetTimeLeft() + delay);
				return false;
			}
		if (trains_ahead.size() > 0 && abs(distance_ - trains_ahead.back()->getPosition()) < 5)
		{
			int delay = TimeToFree(trains_ahead, *this);
			t->SetWaitTime(t->GetTimeLeft() + delay);
			tracks_state_[t->GetTrack()] += delay;
			for (int i = t->getVerse(); i < parked_trains.size(); i += 2)
				parked_trains[i]->SetWaitTime(parked_trains[i]->GetTimeLeft() + delay);
			return false;
		}
		// Se puo' partire
		else
		{
			int position = Position(file);
			t->SetWaitTime(0);
			int time = t->getTime();
			//PrintDepartureTime(t, time, file);
			tracks_state_[t->GetTrack()] = 0;
			if (position < 0)
				trains_in_station_.remove(t);
			if (t->getVerse() == 0 && position != 2 || t->getVerse() == 1 && position != 1)
				trains_ahead.push_back(t);
			t->setState("v");
			return true;
		}
	}
	return false;
}

void MainStation::Update(const ReadFile &file)
{
	int position;
	if (file.get_First_Station() == this)
	{
		position = 1;
	}
	else if (file.get_Last_Station() == this)
	{
		position = 2;
	}
	position = -1;
	// Decrementa il contatore del tempo passato nei binari
	for (int i = 1; i < 5; i++)
		if (tracks_state_[i] > 0)
			tracks_state_[i] -= 1;
	// Se stazione origine o capolinea devo rimuovere i treni dalla stazione
	if (position == 1) // Se stazione origine
	{
		auto i = trains_in_station_.begin();
		while (i != trains_in_station_.end())
		{
			if (tracks_state_[(*i)->GetTrack()] == 0 && (*i)->getVerse() == 1)
			{
				trains_in_station_.erase(i++);
			}
			else
			{
				++i;
			}
		}
	}
	else if (position == 2) // Se stazione capolinea
	{
		auto i = trains_in_station_.begin();
		while (i != trains_in_station_.end())
		{
			if (tracks_state_[(*i)->GetTrack()] == 0 && (*i)->getVerse() == 1)
			{
				trains_in_station_.erase(i++);
			}
			else
			{
				++i;
			}
		}
	}

	// Rimuovono i treni partiti che si trovano gia' nella stazione successiva
	auto i = trains_ahead_east_.begin();
	while (i != trains_ahead_east_.end())
	{
		if (tracks_state_[(*i)->GetTrack()] == 0 && (*i)->getVerse() == 1)
		{
			trains_ahead_east_.erase(i++);
		}
		else
		{
			++i;
		}
	}

	i = trains_ahead_weast_.begin();
	while (i != trains_ahead_weast_.end())
	{
		if (tracks_state_[(*i)->GetTrack()] == 0 && (*i)->getVerse() == 1)
		{
			trains_ahead_weast_.erase(i++);
		}
		else
		{
			++i;
		}
	}
}

int MainStation::Position(const ReadFile &file) const
{
	if (file.get_First_Station() == this)
	{
		return 1;
	}
	else if (file.get_Last_Station() == this)
	{
		return 2;
	}
	return -1;
}

void MainStation::SetTypeVoid() { type_ = -1; }

int MainStation::GetNextTrain(const Train *t) const
{
	list<const Train *> trains = trains_ahead_east_;
	if (t->getVerse() == 1)
	{
		trains = trains_ahead_weast_;
	}

	auto it_next_train = find(trains.begin(), trains.end(), t);
	if (it_next_train == trains.end())
		return -1;
	else
		return (*it_next_train)->getPosition();
}

string MainStation::GetName() const { return name_; }
int MainStation::GetType() const { return type_; }
int MainStation::GetDistance() const { return distance_; }

list<const Train *> MainStation::GetTrainsAhead(int verse) const
{
	if (verse == 1)
	{
		return trains_ahead_weast_;
	}
	else
	{
		return trains_ahead_east_;
	}
}

void MainStation::PrintDepartureTime(const Train *t, int time, const ReadFile &file) const
{
	string next_station = (t->getVerse() == 0) ? file.nextStation(this)->GetName() : file.prevStation(this)->GetName();

	string train_type;
	if (t->getType() == 1)
	{
		train_type = "Regionale";
	}
	else if (t->getType() == 2)
	{
		train_type = "Alta Velocita'";
	}
	else
	{
		train_type = "Alta Velocita' Super";
	}
	cout << "Il treno " + train_type + " numero " << t->getId() << " diretto a " + next_station << " e' in partenza dal binario " << t->GetTrack() << "\t" + FormatTime(time) << endl;
}
void MainStation::PrintArrivalTime(const Train *t, int time, int delay, const ReadFile &file) const
{

	// Qunado fa la richiest� ai 20km se non deve andare in parcheggio se no appena lascia il parcheggio, forse serve una funzione GetDealy() dal treno
	string origin_station = (t->getVerse() == 0) ? file.get_First_Station()->GetName() : file.get_Last_Station()->GetName();
	string train_type;
	if (t->getType() == 1)
	{
		train_type = "Regionale";
	}
	else if (t->getType() == 2)
	{
		train_type = "Alta Velocita'";
	}
	else
	{
		train_type = "Alta Velocita' Super";
	}

	string t_delay = "";
	if (delay > 0)
	{
		t_delay = "R " + to_string(delay);
	}

	cout << "Il treno " + train_type + " numero " << t->getId() << " in arrivo da " + origin_station << " e' in arrivo al binario " << t->GetTrack() << delay << "\t" + FormatTime(time) << endl;
}

void MainStation::AddParkedTrain(Train *t)
{
	t->setState("p");

	vector<Train *> &trains = parked_trains_east_;
	trains = (t->getVerse() == 0) ? parked_trains_east_ : parked_trains_weast_;

	int estimated_arrival = GetEstimatedArrivalTime(t);
	int track = TrackStatus(t)[1];
	// Imposta subito il binario del treno
	t->SetTrack(track + 1);

	for (; track < trains.size(); track += 2)
	{
		if (ComparePriority(t, trains[track]) && estimated_arrival < trains[track]->GetTimeLeft())
		{
			trains.insert(trains.begin() + track, t);
			break;
		}
	}
	trains.push_back(t);

	int wait_time{tracks_state_[t->GetTrack()]};

	int i = TrackStatus(t)[1] % 2;
	for (; i < track && i < trains.size(); i += 2)
		if (trains.size() > 0 && estimated_arrival < trains[i]->GetTimeLeft() + 5)
			wait_time += (trains[i]->GetTimeLeft() + 5) - estimated_arrival;

	t->SetWaitTime(wait_time);

	int time_to_add = wait_time + 5;
	if (track < trains.size())
		for (; track < trains.size(); track += 2)
		{
			int prev_time = trains[track]->GetTimeLeft();
			trains[track]->SetWaitTime(prev_time + time_to_add);
		}
	else
	{
		trains.push_back(t);
	}
}

bool MainStation::ComparePriority(const Train *t1, const Train *t2) const
{
	if (t2->GetWaitTime() >= t2->getMaxWait())
		return false;

	if (t1->getType() > t2->getType())
		return true;

	return false;
}

int MainStation::GetEstimatedArrivalTime(const Train *t) const
{
	int position = t->getPosition();
	int speed = t->getMaxSpeed();

	int d_to_park = abs(distance_ - position) - 5;
	int time_to_station = (d_to_park / speed) + PARK_TO_STATION_TIME;

	return time_to_station;
}

vector<int> MainStation::TrackStatus(const Train *t) const
{
	int verse = t->getVerse();
	const vector<Train *> &trains = (verse == 0) ? parked_trains_east_ : parked_trains_weast_;

	int wait_time_track_one{0};
	int wait_time_track_two{0};

	wait_time_track_one = tracks_state_[(2 * verse)];
	wait_time_track_two = tracks_state_[1 + (2 * verse)];

	for (int i = 0; i < (trains.size()); i++)
	{
		if (trains[i]->GetTrack() == (2 * verse))
			wait_time_track_one += trains[i]->GetTimeLeft();
		else
			wait_time_track_two += trains[i]->GetTimeLeft();
	}

	vector<int> status(4);
	if (wait_time_track_one <= wait_time_track_two)
	{
		status[0] = wait_time_track_one;
		status[1] = (2 * verse);
		status[2] = wait_time_track_two;
		status[3] = (1 + (2 * verse));
	}
	else
	{
		status[0] = wait_time_track_two;
		status[1] = (1 + (2 * verse));
		status[2] = wait_time_track_one;
		status[3] = (2 * verse);
	}

	// max minuti attesa, binario
	return status;
}

bool operator==(const Station &s_one, const Station &s_two) { return s_one.GetName() == s_two.GetName(); }

int TimeToFree(const list<const Train *> &trains_ahead, const Station &a)
{
	int pos = abs(a.GetDistance() - trains_ahead.back()->getPosition());

	int s_km{0};
	int f_km{5};
	if (pos < 5)
	{
		s_km = pos - 5;
	}
	else
	{
		f_km = (pos - 5) + 5;
	}

	int time = (s_km / 80) + (f_km / 160);

	return time;
}

std::string FormatTime(int min)
{
	int m = min % 60;
	int h = min / 60;
	int d = h / 24;
	h -= d * 24;

	string minute = (m < 10) ? "0" + to_string(m) : to_string(m);
	string hour = (h < 10) ? "0" + to_string(h) : to_string(h);
	string day = (d > 0) ? " Giorno +" + to_string(d) : "";

	string time = hour + ":" + minute + day;

	return time;
}