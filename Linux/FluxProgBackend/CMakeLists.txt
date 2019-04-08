cmake_minimum_required(VERSION 2.6)
project(FluxProgBackend)

INCLUDE(FindPkgConfig)

set(CMAKE_CXX_STANDARD 11)

#Setting build type to release
set(CMAKE_BUILD_TYPE Release)


set(CMAKE_CXX_FLAGS "-Wall")

#Setting directory for the outputted executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

#Defining useful directory macros
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
#set(FORMS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/forms)

# definicoes
add_definitions(-DNON_MATLAB_PARSING)
add_definitions(-DMAX_EXT_API_CONNECTIONS=255)

#Including the project's headers directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/remoteApi
    ${PROJECT_BINARY_DIR}/include
)

#Including Boost
#find_package(Boost COMPONENTS serialization system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

#Including pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(include)
add_subdirectory(source)
#add_subdirectory(forms)
