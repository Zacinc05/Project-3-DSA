cmake_minimum_required(VERSION 3.29)
project(Project_3_DSA)

set(CMAKE_CXX_STANDARD 14)

set(SFML_DIR "./SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)

include_directories(.)

add_executable(Project_3_DSA
    main.cpp
    vendor_list.cpp
    vendor_list.h
        use_algorithms.cpp
        Applicationwindow.h)

target_link_libraries(Project_3_DSA sfml-system sfml-window sfml-graphics sfml-audio)
