cmake_minimum_required(VERSION 3.26)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR D:/Завантаження/sfml/SFML-2.6.1/lib/cmake/SFML)
#set(GMP_DIR D:/Завантаження/auxilary/gmp)
#set(CGAL_DIR D:/Завантаження/CGAL-5.6/lib/cmake/CGAL)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)
#find_package(CGAL REQUIRED COMPONENTS Core)


include_directories(D:/Завантаження/sfml/SFML-2.6.1/include)
target_link_libraries(untitled sfml-system sfml-window sfml-graphics sfml-audio)
#target_link_libraries(voronoi CGAL::CGAL)