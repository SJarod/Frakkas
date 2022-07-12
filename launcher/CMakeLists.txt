project(Frakkarena
        LANGUAGES CXX C
        VERSION 1.0
        DESCRIPTION "Launch the game"
)

add_executable(${PROJECT_NAME}
               ${CMAKE_SOURCE_DIR}/launcher.rc
               launcher.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Game)

set_target_properties(${PROJECT_NAME} PROPERTIES
                                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/debug
                                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/release
)
