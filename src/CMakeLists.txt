add_library(core
    can_sim.cpp
    pid.cpp
    state_machine.cpp
    controller.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# main program
add_executable(eae_firmware main.cpp)
target_link_libraries(eae_firmware PRIVATE core)
