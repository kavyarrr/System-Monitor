cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)

# Integration Test Executable
file(GLOB TEST_SOURCES "src/*.cpp")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_SOURCE_DIR}/src/ncurses_display.cpp")
add_executable(integration_test tests/integration_test.cpp ${TEST_SOURCES})
set_property(TARGET integration_test PROPERTY CXX_STANDARD 17)
target_compile_options(integration_test PRIVATE -Wall -Wextra)
# Test doesn't need ncurses, so we don't link it
enable_testing()
add_test(NAME IntegrationTests COMMAND integration_test)
