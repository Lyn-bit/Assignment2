#include "Train.h"

using namespace std;

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