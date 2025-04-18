cmake_minimum_required(VERSION 3.29)
project(Project_3_DSA)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project_3_DSA
    main.cpp
    vendor_list.cpp
    vendor_list.h)
