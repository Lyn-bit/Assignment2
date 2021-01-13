//Stojna Lorei

#include "station.h"

#define PARK_TO_STATION_TIME 4

using namespace std;

LocalStation::LocalStation(string name, int type, int distance, const ReadFile &read_file) : name_{name}, type_{type}, distance_{distance}, read_file_{read_file},
																							 trains_in_station_{}, tracks_state_{0, 0, 0, 0, 0, 0, 0}, trains_ahead_east_{}, trains_ahead_weast_{}, parked_trains_east_{}, parked_trains_weast_{}
{
}

void LocalStation::ArrivalRequest(Train *t, const ReadFile &file)
{
	vector<Train *> &parked_trains = parked_trains_east_;
	parked_trains = (t->getVerse() == 0) ? parked_trains_east_ : parked_trains_weast_;

	int estimated_arrival_time = GetEstimatedArrivalTime(t);
	vector<int> track_info = TrackStatus(t);
	if (t->getType() > 1)
	{
		int time_to_clear = GetHighSpeedTrainTime(t);
		for (int i = t->getVerse(); i < 4; i += 2)
		{
			t->SetTrack(5 + t->getVerse() + 1);
			int track = t->GetTrack();
			for (auto p = trains_in_station_.begin(); p != trains_in_station_.end(); p++)
			{
				if ((*p)->getVerse() == t->getVerse() && (*p)->GetWaitTime() >= (*p)->getMaxWait())
				{
					t->SetTrack(5 + t->getVerse() + 1);
					t->SetWaitTime(0);
					AddDelay(time_to_clear, t->getVerse());
					return;
				}
			}
		}
		AddParkedTrain(t);
		return;
	}

	int odd_tracks = (parked_trains.size() / 2) + 1;
	int evene_tracks = parked_trains.size() - odd_tracks;
	if (estimated_arrival_time > (track_info[0] + (odd_tracks * 5)))
	{
		t->SetTrack(track_info[1] + 1);
		t->SetWaitTime(0);
		t->setState("s");
	}
	// altri casi
	else
		AddParkedTrain(t);
}

bool LocalStation::DepartureRequest(Train *t, const ReadFile &file)
{
	vector<Train *> &parked_trains = parked_trains_east_;
	if (t->getVerse() == 0)
	{
		parked_trains = parked_trains_weast_;
	}

	list<const Train *> &trains_ahead = trains_ahead_east_;
	if (t->getVerse() == 0)
	{
		trains_ahead = trains_ahead_weast_;
	}

	bool empty_park = parked_trains.size() <= 0;
	bool free_ahead = trains_ahead.size() <= 0;
	// Se si trova in parcheggio devo vedere se il suo binario e' stato liberato
	if (t->getState() == "p")
	{
		if (t->getType() >= 1)
		{
			for (int i = t->getVerse(); i < 4 && !empty_park && FindTrain(i, parked_trains) != nullptr; i += 2)
				if (FindTrain(i, parked_trains)->GetWaitTime() >= FindTrain(i, parked_trains)->getMaxWait())
				{
					AddDelay(tracks_state_[i], t->getVerse());
					return false;
				}
			if (!free_ahead && abs(trains_ahead.back()->getPosition() - distance_) < 5)
			{
				AddDelay(abs(trains_ahead.back()->getPosition() - distance_) / trains_ahead.back()->getSpeed(), t->getVerse());
				return false;
			}
			trains_ahead.push_back(t);
			if (parked_trains.size() > 0)
				parked_trains.erase(parked_trains.begin());
			t->setState("s");
			return true;
		}
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
		if (!free_ahead && abs(distance_ - trains_ahead.back()->getPosition()) < 5)
		{

			int delay = 0; 
			t->SetWaitTime(t->GetTimeLeft() + delay);
			tracks_state_[t->GetTrack()] += delay;
			for (int i = t->getVerse(); i < parked_trains.size(); i += 2)
				parked_trains[i]->SetWaitTime(parked_trains[i]->GetTimeLeft() + delay);
			return false;
		}
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

void LocalStation::Update(const ReadFile &file)
{
	/*
	*
	* Una Volta arrivati al capolinea/origine (in base al verso) non devono pi� partire ma scompaiono e basta
	*
	*/
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

int LocalStation::Position(const ReadFile &file) const
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

void LocalStation::SetTypeVoid() { type_ = -1; }

int LocalStation::GetNextTrain(const Train *t) const
{
	/*
	*
	* capolinea/origine
	*
	*/
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

std::string LocalStation::GetName() const { return name_; }
int LocalStation::GetType() const { return type_; }
int LocalStation::GetDistance() const { return distance_; }

std::list<const Train *> LocalStation::GetTrainsAhead(int verse) const
{
	if (verse == 0)
	{
		return trains_ahead_weast_;
	}
	else
	{
		return trains_ahead_east_;
	}
}

void LocalStation::PrintDepartureTime(const Train *t, int time, const ReadFile &file) const
{
	const Station *st = file.prevStation(this);
	string name = st->GetName();
	string next_station = (t->getVerse() == 0) ? file.nextStation(this)->GetName() : name;

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
	std::cout << "Il treno " + train_type + " numero " << t->getId() << " diretto a " + next_station << " e' in partenza dal binario " << t->GetTrack() << "\t" + FormatTime(time) << endl;
}
void LocalStation::PrintArrivalTime(const Train *t, int time, int delay, const ReadFile &file) const
{

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

	std::cout << "Il treno " + train_type + " numero " << t->getId() << " in arrivo da " + origin_station << " e' in arrivo al binario " << t->GetTrack() << delay << "\t" + FormatTime(time) << endl;
}

void LocalStation::AddParkedTrain(Train *t)
{
	t->setState("p");

	vector<Train *> &trains = parked_trains_east_;
	trains = (t->getVerse() == 0) ? parked_trains_east_ : parked_trains_weast_;

	int estimated_arrival = GetEstimatedArrivalTime(t);
	int track = TrackStatus(t)[1];
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

	int wait_time{tracks_state_[t->GetTrack()] + 1};
	int i = TrackStatus(t)[1] % 2;
	for (; i < track && i < trains.size(); i += 2)
		wait_time += (trains[i])->GetTimeLeft() + 5;
	t->SetWaitTime(wait_time);

	int time_to_add = wait_time + 5;
	if (trains.size() > 0 && track < trains.size())
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

void LocalStation::AddDelay(int delay, int verse)
{
	vector<Train *> parked_trains = (verse == 1) ? parked_trains_east_ : parked_trains_weast_;
	for (auto i = parked_trains.begin(); i != parked_trains.end(); i++)
		(*i)->SetWaitTime((*i)->GetTimeLeft() + delay);

	for (auto i = trains_in_station_.begin(); i != trains_in_station_.end(); i++)
		if ((*i)->getVerse() == verse)
			(*i)->SetWaitTime((*i)->GetTimeLeft() + delay);

	for (int i = verse; i < 4; i += 2)
		tracks_state_[i] += delay;
}

bool LocalStation::ComparePriority(const Train *t1, const Train *t2) const
{
	if (t2->GetWaitTime() >= t2->getMaxWait())
		return false;

	if (t1->getType() > t2->getType())
		return true;

	return false;
}

int LocalStation::GetEstimatedArrivalTime(const Train *t) const
{
	int position = t->getPosition();
	int speed = t->getMaxSpeed();

	int d_to_park = abs(distance_ - position) - 5;
	int time_to_station = (d_to_park / speed) + PARK_TO_STATION_TIME;

	return time_to_station;
}

vector<int> LocalStation::TrackStatus(const Train *t) const
{
	vector<int> status(4);

	int verse = t->getVerse();
	const vector<Train *> &trains = (verse == 1) ? parked_trains_east_ : parked_trains_weast_;
	if (t->getType() > 1)
	{
		status[0] = 0;
		status[1] = 4 + verse;
		status[2] = 0;
		status[3] = 0;
		return status;
	}
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

	return status;
}

int LocalStation::GetHighSpeedTrainTime(const Train *t) const
{
	int total_distance = abs(distance_ - t->getPosition()) + 10;
	return total_distance / t->getSpeed();
}

const Train *LocalStation::FindTrain(int track, const vector<Train *> trains) const
{
	for (int i = 0; i < trains.size(); i++)
	{
		if (trains[i]->GetTrack() == track)
			return trains[i];
	}
	return nullptr;
}