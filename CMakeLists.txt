cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(Ferrovia)

add_library(Trains
	Train.h
	Train.cpp
	Train_High_Speed.cpp
	Train_Regional.cpp
	Train_High_Speed_Super.cpp
)

add_library(Stations
	station.h
	station.cpp
	main_station.cpp
	local_station.cpp
)

add_library(Controll
	read_file.h
	ReadFile.cpp
	declarations.h
	declarations.cpp	
)

add_executable(main
	main.cpp
)

target_link_libraries(main
	Trains
	Stations
	Controll
)