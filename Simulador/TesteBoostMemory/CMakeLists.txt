project(TesteBoostMemory)
cmake_minimum_required(VERSION 2.8)


set(APP_SOURCES
	main.cpp
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} Threads::Threads)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} rt)
