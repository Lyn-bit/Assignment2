//Stojna Lorei

#include "local_station.h"

#define PARK_TO_STATION_TIME 4

using namespace std;

LocalStation::LocalStation(string name, int type, int distance, const ReadFile& read_file) :
	name_{ name }, type_{ type }, distance_{ distance }, position_{ -1 }, read_file_{ read_file },
	trains_in_station_{}, tracks_state_{ 0, 0, 0, 0, 0, 0 },
	trains_ahead_east_{}, trains_ahead_weast_{}, parked_trains_east_{}, parked_trains_weast_{}
{
	if (read_file.get_first_Station() == *this) { position_ = 1; }
	else if (read_file.get_last_Station() == *this) { position_ = 2; }
}

// I binari devono essere occupati appena il treno lascia il parcheggio
// (e forse appena fa la riechiesta se gia disponibile)
void LocalStation::ArrivalRequest(Train& t)
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
	//
	// devo controllare se in partenza dalla stazione c'e un treno regionale che aspetta
	// da piu di 25 minuti sè è cosi allora metto il treno velocita in parcheggio se no aumento il
	// tempo di attesa del treno in stazione del tempo che il treno che sta per passare ci impiegherà
	// a superarli di 10km
	if (t.getType() > 1)
	{
		int time_to_clear = GetHighSpeedTrainTime(t);
		// controllo se nel tempo che ci impieaga a percorrere i 30km (20 + 10)
		// c'è un altro treno che deve partire in meno del tempo che ci impiegha il treno
		// a fare i 30km
		for (int i = t.getVerse(); i < 4; i+=2)
			if (GetHighSpeedTrainTime(t) < tracks_state_[i] && FindTrain(i).GetWaitTime() >= FindTrain(i).getMaxWait())
				AddParkedTrain(t);
		// Se puo procedere allora bisogna aggiungere ritardo ai treni in stazione e in parcheggio
		// che sara uguale a time_to_clear piu il tempo precedente
		AddDelay(time_to_clear, t.getVerse());

	}


	int odd_tracks = (trains.size() / 2) + 1;
	int evene_tracks = trains.size() - odd_tracks;
	// se all arrivo trova libero il binario più veloce lo metto la
	if (estimated_arrival_time > (track_info[0] + (odd_tracks * 5)))
	{
		t.SetTrack(track_info[1]); t.SetWaitTime(0);
	}
	// se all arrivo trova libero il binario più "lento" lo metto la
	/*
	else if (estimated_arrival_time > (track_info[2] + (evene_tracks * 5)))
	{
		t.SetTrack(track_info[3]); t.SetWaitTime(0);
	}
	*/
	// altri casi
	else
		AddParkedTrain(t);
}



vector<int> LocalStation::TrackStatus(const Train& t) const
{
	/*
	*
	* capolinea/origine
	*
	*/
	int verse = t.getVerse();
	const vector<Train>* trains = (verse == 0) ? trains = &parked_trains_east_ : trains = &parked_trains_weast_;
	//trains = (verse == 0) ? trains = &parked_trains_east_ : trains = &parked_trains_weast_;

	int wait_time_track_one{ 0 };
	int wait_time_track_two{ 0 };
	int wait_time_transit{ 0 };

	wait_time_track_one = tracks_state_[(2 * verse)];
	wait_time_track_two = tracks_state_[1 + (2 * verse)];
	wait_time_transit = tracks_state_[5 + verse];

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
	
	status[5] = wait_time_transit;

	// max minuti attesa, binario
	return status;
}


void MainStation::AddParkedTrain(Train& t)
{
	/*
	*
	* capolinea/origine
	*
	*/
	
	// Se si tratta di un treno veloce o super parte appena 
	// l'altro treno si trova a più di 5km dalla stazione
	if (t.getType() > 1)
	{

	}


	vector<Train&>& trains = parked_trains_east_;
	trains = (t.getVerse() == 0) ? trains = parked_trains_east_ : trains = parked_trains_weast_;

	int estimated_arrival = GetEstimatedArrivalTime(t);
	// Controlla solo il binario più veloce
	int track = TrackStatus(t)[1];
	// Imposta subito il binario del treno
	t.SetTrack(track);

	for (; track < trains.size(); track += 2)
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

int LocalStation::GetHighSpeedTrainTime(const Train& t) const
{
	int total_distance = abs(distance_ - t.getPosition()) + 10;
	return total_distance / t.getSpeed();
}

const Train& LocalStation::FindTrain(int track) const
{
	for (auto i = trains_in_station_.begin(); i != trains_in_station_.end(); i++)
		if (i->GetTrack() == track)
			return *i;
	//////da coreggere
	return trains_in_station_.front();
}


void LocalStation::AddDelay(int delay, int verse)
{
	vector<Train&> parked_trains = (verse == 0) ? parked_trains_east_ : parked_trains_weast_;
	for (auto i = parked_trains.begin(); i != parked_trains.end(); i++)
		i->SetWaitTime(i->GetTimeLeft() + delay);

	
	for (auto i = trains_in_station_.begin(); i != trains_in_station_.end(); i++)
		if (i->getVerse() == verse)
			i->SetWaitTime(i->GetTimeLeft() + delay);

	for (int i = verse; i < 4; i += 2)
		tracks_state_[i] += delay;
	
}