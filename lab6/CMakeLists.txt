cmake_minimum_required(VERSION 3.14)
project(lab6 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILES
    src/point.cpp
    src/npc.cpp
    src/rogue.cpp
    src/knight.cpp
    src/elf.cpp
    src/npc_factory.cpp
    src/observer_console.cpp
    src/observer_file.cpp
    src/battle_visitor.cpp
    src/dungeon.cpp
)

add_executable(lab6
    main.cpp
    ${SOURCE_FILES}
)

find_package(GTest REQUIRED)

add_executable(tests
    tests/tests01.cpp
    ${SOURCE_FILES}
)

target_link_libraries(tests
    PRIVATE
        GTest::gtest_main
        GTest::gtest
        Threads::Threads
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)