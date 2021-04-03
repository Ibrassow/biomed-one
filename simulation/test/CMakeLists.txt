

add_executable(simulation simu_testing.cpp)


target_link_libraries(simulation PRIVATE simulib)

set_target_properties(simulation PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/python/
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/python/
)








