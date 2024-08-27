cmake_minimum_required(VERSION 3.5)

project(s21_Test_SmartCalc_v2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_LIBRARIES stdc++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -g")
set(prev_req_flags ${CMAKE_REQUIRED_FLAGS})
# set(CMAKE_REQUIRED_FLAGS ${prev_req_flags} -lstdc++)
set(CMAKE_LINK_LIBRARY_FLAG -lstdc++)

find_package(Qt6 REQUIRED COMPONENTS Core)
# enable_testing()
find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIR})
# ADD_SUBDIRECTORY (gtest-1.6.0)
# enable_testing()
# include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SOURCES
        s21_test_smart.cc
        ../model/calculate.cc
        ../model/calculate.h
        ../model/economy.cc
        ../model/economy.h
)

add_executable(s21_Test_SmartCalc_v2 ${SOURCES})

target_link_libraries(s21_Test_SmartCalc_v2 PRIVATE Qt6::Core)
# target_link_libraries(s21_Test_SmartCalc_v2 PRIVATE ${GTEST_LIBRARY_DEBUG} ${GTEST_MAIN_LIBRARY_DEBUG})
target_link_libraries(s21_Test_SmartCalc_v2 PRIVATE GTest::gtest)
# target_link_libraries(s21_Test_SmartCalc_v2 PRIVATE gtest gtest_main)

include(GoogleTest)
# gtest_discover_tests(s21_Test_SmartCalc_v2)