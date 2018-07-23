project(TesteBoostMemory)
cmake_minimum_required(VERSION 2.8)


set(APP_SOURCES
	main.cpp
	stdafx.cpp
)
set(HEADERS
	stdafx.h
	targetver.h
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(BOOST_LIBRARYDIR="C:/Arquivos de Programa/boost/boost_1_67_0/stage/lib")
find_package(Boost 1.36.0)
find_package(Boost COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${BOOST_ROOT})
include_directories(${BOOST_LIBRARYDIR})
include_directories(${Boost_LIBRARY_DIR_RELEASE}) 
include_directories(${Boost_LIBRARY_DIR_DEBUG})  

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)



LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${BOOST_LIBRARYDIR})

