cmake_minimum_required(VERSION 3.16)
project(words_to_number)

set(CMAKE_CXX_STANDARD 14)

add_executable(words_to_number main.cpp words2text.h unit_tests.h)
