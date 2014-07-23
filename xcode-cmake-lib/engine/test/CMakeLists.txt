cmake_minimum_required (VERSION 2.8)
project(MovieBallEngineTests)

find_package (Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(test1_source main.cpp)
set(test2_source main1.cpp)

add_executable(test1 ${test1_source})
add_executable(test2 ${test2_source})

target_link_libraries(test1 engine ${Boost_LIBRARIES})
target_link_libraries(test2 engine ${Boost_LIBRARIES})

add_test(NAME Test1 COMMAND test1 --log_level=all)
add_test(NAME Test2 COMMAND test2 --log_level=all)

