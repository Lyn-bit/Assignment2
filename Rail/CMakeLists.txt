cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(Ferrovia)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2")


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/timetables.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/filetxt/timetables.txt)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/line_description.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/filetxt/line_description.txt)

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
)
add_library(declarations
	declarations.h
	declarations.cpp	
)

add_executable(main
	main.cpp
)

target_link_libraries(main
	Controll
	declarations
	Trains	
	Stations	
)