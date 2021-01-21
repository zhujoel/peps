# Add Pnl to tests
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})

add_library(DateTime STATIC DateTime.cpp) # On lie la librarie au cpp
add_library(DateTimeVector STATIC DateTimeVector.cpp)
add_library(IMarketData STATIC IMarketData.cpp)
add_library(SimulatedMarketData STATIC SimulatedMarketData.cpp)
add_library(IUnderlying STATIC IUnderlying.cpp)
add_library(ForeignUnderlying STATIC ForeignUnderlying.cpp)
add_library(IDerivative STATIC IDerivative.cpp)
add_library(Ocelia STATIC Ocelia.cpp)
