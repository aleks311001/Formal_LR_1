cmake_minimum_required(VERSION 3.16)
project(LR_1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
#set(CMAKE_CXX_COMPILER g++)

add_executable(LR_1 main.cpp Algo.h)

#find_package(PCAP REQUIRED)
#find_package(GTest REQUIRED)
#find_package(Threads REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

#enable_testing()
#set(CMAKE_EXE_LINKER_FLAGS --coverage)

#add_executable(Tests_LR_1 runTests.cpp Algo.h Test.cpp Test.h Grammar.h TestUtils.h)
#target_link_libraries(Tests_LR_1 ${GTEST_LIBRARIES} Threads::Threads)
