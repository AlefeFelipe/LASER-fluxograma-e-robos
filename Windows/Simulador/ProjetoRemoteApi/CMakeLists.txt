cmake_minimum_required(VERSION 2.6)
project(cppremoteapi)

INCLUDE(FindPkgConfig)



IF(CMAKE_COMPILER_IS_GNUCC)
SET(CMAKE_CXX_FLAGS
"${CMAKE_CXX_FLAGS}  -Wall -std=c++0x -DBUILD_SHARED_LIBS=OFF ")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# definicoes da remoteAPI para CPP
add_definitions(-DNON_MATLAB_PARSING)
add_definitions(-DMAX_EXT_API_CONNECTIONS=255)


set(SOURCES
    remoteApi/extApi.c
    #remoteApi/extApiCustom.c
    remoteApi/extApiPlatform.c
    rs232.c
)

set(HEADERS
    remoteApi/extApi.h
    #remoteApi/extApiCustom.h
    remoteApi/extApiInternal.h
    remoteApi/extApiPlatform.h
    include/v_repConst.h
    include/v_repLib.h
    rs232.h

)

include_directories("include/")
include_directories("remoteApi/")

add_executable(cppremoteapi
               main.cpp
               ${SOURCES}
	       ${HEADERS})

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




if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(cppremoteapi PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(cppremoteapi "${CMAKE_THREAD_LIBS_INIT}")
endif()


target_link_libraries (cppremoteapi ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS cppremoteapi RUNTIME DESTINATION bin)
