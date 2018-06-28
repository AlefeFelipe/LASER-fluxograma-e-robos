cmake_minimum_required(VERSION 2.6)
project(fluxprog)

IF(CMAKE_COMPILER_IS_GNUCC)
SET(CMAKE_CXX_FLAGS
"${CMAKE_CXX_FLAGS} -wall `pkg-config --cflags --libs allegro-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5` -I /home/robocup/Documents/boost_1_66_0/ -pthread -lrt -std=c++11 ")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

add_executable(fluxprog main.cpp fluxprog.cpp fluxprog.h)
