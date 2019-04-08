#Defining the executable's sources
aux_source_directory(. SRC_LIST)
add_subdirectory(remoteApi)
#Creating the executable
set(APPLICATION_NAME FluxProgBackend)

set(SOURCES
    remoteApi/extApi.c
    #remoteApi/extApiCustom.c
    remoteApi/extApiPlatform.c
    rs232.c
)
add_executable(${APPLICATION_NAME} ${SRC_LIST} ${SOURCES})

#Linking external libraries to the executable
#target_link_libraries(${APPLICATION_NAME} Boost::serialization)
#target_link_libraries(${APPLICATION_NAME} Boost::system)
#target_link_libraries(${APPLICATION_NAME} Boost::filesystem)
target_link_libraries(${APPLICATION_NAME} Threads::Threads)
target_link_libraries(${APPLICATION_NAME} rt)
#target_link_libraries (${APPLICATION_NAME} ${CMAKE_THREAD_LIBS_INIT})
