cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(controllsystem)

add_library(Train 
	Train.h Train.cpp
	Train_Regional.h Train_Regional.cpp
	Train_High_Speed.h Train_High_Speed.cpp
	Train_High_Speed_Super.h Train_High_Speed_Super.cpp
	main.cpp
)

add_library(Station
	station.h station.cpp
	main_station.h main_station.cpp
	local_station.h local_station.cpp
	main.cpp
)

add_library(ReadFile 
	ReadFile.h ReadFile.cpp
	main.cpp
)

add_executable(Main 
	main.cpp
)

target_link_libraries(Main 
	Station
	Train
	ReadFile 
)





