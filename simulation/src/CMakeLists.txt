set(LIBRARY_NAME simulib)

include_directories(${CMAKE_CURRENT_LIST_DIR})

set(LIB_SOURCES 
	config.cpp
	simulator.cpp
    model/simulation.cpp 
    model/hr.cpp 
    model/spo2.cpp 
    model/temperature.cpp
    model/utilities/fft.cpp 
    model/utilities/tools.cpp
)


add_library(${LIBRARY_NAME} ${LIB_SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)


set_target_properties(${LIBRARY_NAME} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib/
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/
)








