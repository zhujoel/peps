cmake_minimum_required(VERSION 3.0)
project(peps)

# Detect PNL
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})

# sources and headers
set(SOURCES src/MonteCarloPricer.cpp src/main.cpp)
set(HEADERS include/MonteCarloPricer.h)

include_directories(include)
add_executable(main src/main.cpp ${SOURCES} ${HEADERS})
target_link_libraries(main ${LIBS})