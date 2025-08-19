# download googletest via FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.14.0.zip
)
# For Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests
    test_pid.cpp
    test_state_machine.cpp
)
target_link_libraries(tests
    PRIVATE
    core
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
