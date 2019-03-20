cmake_minimum_required(VERSION 2.6)
project(fluxprogbackend)

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
    remoteApi/v_repConst.h
    remoteApi/v_repLib.h
    rs232.h

)

include_directories("remoteApi/")
aux_source_directory(. SRC_LIST)

find_package(Threads REQUIRED)




add_executable(${PROJECT_NAME}
               ${SOURCES}
	       ${HEADERS}
       ${SRC_LIST})

if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(${PROJECT_NAME} PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
endif()

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES} rt)


target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
