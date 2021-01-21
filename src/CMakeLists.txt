# Add Pnl to tests
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})

add_library(DateTime STATIC DateTime.cpp) # On lie la librarie au cpp
add_library(DateTimeVector STATIC DateTimeVector.cpp)
