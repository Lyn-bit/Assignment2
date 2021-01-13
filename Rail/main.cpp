#include "declarations.h"

#include "read_file.h"
#include "train.h"
#include "station.h"
#include <iomanip>

using namespace std;

Station *find_s(list<Station *> &stations, const Station *st);

int main()
{
	ReadFile *file = new ReadFile{};

	ofstream myfile;
	myfile.open("./output.txt");

	myfile << "PLANNED TIME TABLE" << endl;
	list<Train *> t{file->get_train_list()};
	for (auto q = t.begin(); q != t.end(); q++)
	{
		myfile << "Train ID: " << (*q)->getId() << "\t";
		list<int> time = (*q)->getTimes();
		for (auto a = time.begin(); a != time.end(); a++)
		{
			myfile << *a << " ";
		}
		myfile << endl;
	}

	int count{0};
	list<Station *> stations = file->get_station_list();
	list<Train *> trains = file->get_train_list();

	int first_pos = file->get_First_Station()->GetDistance();
	int last_pos = file->get_Last_Station()->GetDistance();

	for (auto p = trains.begin(); p != trains.end(); p++)
	{
		(*p)->update();
	}

	// LISTA ORARI ARRIVO DEI TRENI IN STAZIONE
	vector<list<int>> times(trains.size());
	// LISTA DEBUG
	vector<int> prev_position(trains.size());

	while (count < 2160)
	{
		myfile << "\nTime: " << FormatTime(count) << endl;
		int train_ind{0};
		for (auto p = trains.begin(); p != trains.end(); p++)
		{
			if ((*p)->getVerse() == 0 && (*p)->getPosition() >= last_pos)
			{
				continue;
			}
			else if ((*p)->getVerse() == 1 && (*p)->getPosition() <= first_pos)
			{
				continue;
			}

			// Debug
			const Station *ds{(*p)->getCurrentStation()};
			string train = to_string((*p)->getId());
			string track = ((*p)->GetTrack() > 0) ? " BINARIO " + to_string((*p)->GetTrack()) : "";
			string track_d = ((*p)->GetTrack() > 0) ? " E' PARTITO DAL BINARIO " + to_string((*p)->GetTrack()) : "";
			string track_a = ((*p)->GetTrack() > 0) ? " E' PARTITO DAL BINARIO " + to_string((*p)->GetTrack()) : "";
			string destination = ((*p)->getVerse() == 0) ? file->get_Last_Station()->GetName() : file->get_First_Station()->GetName();
			string origin = ((*p)->getVerse() == 1) ? file->get_Last_Station()->GetName() : file->get_First_Station()->GetName();
			string position = to_string((*p)->getPosition());
			string next_station = (*p)->getNextStation()->GetName();

			myfile << "TRENO " << train << " DIRETTO A " << std::left << std::setw(10) << destination << track << " PROSSIMA STAZIONE " << next_station << endl;

			if ((*p)->getVerse() == 0)
			{
				if ((*p)->getPosition() == (*p)->getNextStation()->GetDistance())
				{
					if (times[train_ind].size() < 0)
					{
						times[train_ind].push_back(count);
						myfile << "\nIL TRENO " << train << " DIRETTO A " << std::left << std::setw(10) << destination << " IN ARRIVO DA " << origin << track_a << endl;
					}
					if (times[train_ind].back() != count)
						times[train_ind].push_back(count);
					myfile << "\nIL TRENO " << train << " DIRETTO A " << std::left << std::setw(10) << destination << " IN ARRIVO DA " << origin << track_a << endl;
				}
			}
			else
			{
				if ((*p)->getPosition() == (*p)->getNextStation()->GetDistance())
				{

					if ((*p)->getPosition() == (*p)->getNextStation()->GetDistance())
					{
						if (times[train_ind].size() < 0)
						{
							times[train_ind].push_back(count);
							myfile << "IL TRENO " << train << " DIRETTO A " << std::left << std::setw(10) << destination << " IN ARRIVO DA " << origin << track_a << endl;
						}
						if (times[train_ind].back() != count)
							times[train_ind].push_back(count);
						myfile << "IL TRENO " << train << " DIRETTO A " << std::left << std::setw(10) << destination << " IN ARRIVO DA " << origin << track_a << endl;
					}
				}
			}
			if ((*p)->getPosition() == (*p)->getNextStation()->GetDistance())
			{
				if (times[train_ind].size() < 0)
				{
					times[train_ind].push_back(count);
					myfile << std::right << std::setw(7) << "IL TRENO " << train << " DIRETTO A " << std::left << std::setw(10) << destination << track_d << endl;
				}
				if (times[train_ind].back() != count)
					myfile << "IL TRENO " << train << " DIRETTO A " << std::left << std::setw(10) << destination << track_d << endl;
			}

			pair<int, const Station *> info{(*p)->update()};
			if (info.first == 0)
			{
				Station *s = find_s(stations, info.second);

				s->ArrivalRequest((*p), *file);
			}
			else if (info.first == 1)
			{
				Station *s = find_s(stations, info.second);
				bool left = s->DepartureRequest((*p), *file);

				if ((*p)->getPosition() == (*p)->getNextStation()->GetDistance())
				{
					if (times[train_ind].size() < 0)
					{
						times[train_ind].push_back(count);
						myfile << "IL TRENO " << train << " DIRETTO A " << std::left << std::setw(10) << destination << track_d << endl;
					}
					if (times[train_ind].back() != count)
						times[train_ind].push_back(count);
					myfile << "IL TRENO " << train << " DIRETTO A " << std::left << std::setw(10) << destination << track_d << endl;
				}
			}
			// AGGIORNO CONTATORI
			prev_position[train_ind] = (*p)->getPosition();
			train_ind++;
		}
		for (auto p = stations.begin(); p != stations.end(); p++)
		{
			(*p)->Update(*file);
		}
		count++;
	}
	myfile << endl;

	myfile.close();
}

Station *find_s(list<Station *> &stations, const Station *st)
{
	for (auto p = stations.begin(); p != stations.end(); p++)
	{
		if ((*p) == st)
			return *p;
	}
	return nullptr;
}