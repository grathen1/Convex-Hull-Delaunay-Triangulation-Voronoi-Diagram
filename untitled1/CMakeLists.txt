cmake_minimum_required(VERSION 3.26)
project(untitled1)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled1 main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR D:/Завантаження/sfml/SFML-2.6.1/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(D:/Завантаження/sfml/SFML-2.6.1/include)
target_link_libraries(untitled1 sfml-system sfml-window sfml-graphics sfml-audio)
