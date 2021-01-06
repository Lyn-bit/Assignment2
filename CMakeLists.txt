cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(controllsystem)

add_library(Station station.h station.cpp)
add_library(MainStation main_station.h main_station.cpp)
add_library(LocalStation local_station.h local_station.cpp)

add_library(Train Train.h Train.cpp)
add_library(RTrain Train_Regional.h Train_Regional.cpp)
add_library(HTrain Train_High_Speed.h Train_High_Speed.cpp)
add_library(HSTrain Train_High_Speed_Super.h Train_High_Speed_Super.cpp)

add_library(ReadFile ReadFile.h ReadFile.cpp)

add_executable(Main main.cpp)

target_link_libraries(Main Station)
target_link_libraries(Main MainStation)
target_link_libraries(Main LocalStation)

target_link_libraries(Main Train)
target_link_libraries(Main RTrain)
target_link_libraries(Main HTrain)
target_link_libraries(Main HSTrain)

target_link_libraries(Main TReadFile)


