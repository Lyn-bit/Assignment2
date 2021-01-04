#include <iostream>
#include "ReadFile.h"
#include "station.h"
#include "main_station.h"
#include "local_station.h"
#include "Train.h"
#include "Train_Regional.h"
#include "Train_High_Speed.h"
#include "Train_High_Speed_Super.h"

int main()
{
    ReadFile rf;
    int counter=0;
    MainStation ms = rf.get_first_Station();
    std::cout << ms.GetName();
    return 0;
}