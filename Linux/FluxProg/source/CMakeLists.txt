#Defining the executable's sources
aux_source_directory(. SRC_LIST)
#Creating the executable
set(APPLICATION_NAME FluxProg)

add_executable(${APPLICATION_NAME} ${SRC_LIST})

#Linking external libraries to the executable
#target_link_libraries(${APPLICATION_NAME} Boost::serialization)
#target_link_libraries(${APPLICATION_NAME} Boost::system)
#target_link_libraries(${APPLICATION_NAME} Boost::filesystem)
target_link_libraries(${APPLICATION_NAME} Threads::Threads)
target_link_libraries(${APPLICATION_NAME} rt)
target_link_libraries(${APPLICATION_NAME} ${allegro_LIBRARIES})
#target_link_libraries (${APPLICATION_NAME} ${CMAKE_THREAD_LIBS_INIT})
