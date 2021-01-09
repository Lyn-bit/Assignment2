#include "declarations.h"

#include "train.h"
#include "read_file.h"
#include "station.h"

using namespace std;

int main() {

	ReadFile file{};
	Station* s{ file.get_First_Station() };

	//list<Station*> st{ file.get_station_list() };
	//list<Train*> tr{ file.get_train_list() };

	int counter{ 0 };
	/*
	for (; counter < 10; counter++)
	{
		//cout << (*i)->GetName() << endl;
		for (auto p = tr.begin(); p != tr.end(); p++)
		{
			(*p)->update();
			cout << (*p)->getPosition() << endl;
		}
	}
	*/
	
	//cout << s->GetDistance() << endl;
	
	int n = 1440;
	string time = FormatTime(n);
	//cout << time << endl;
}