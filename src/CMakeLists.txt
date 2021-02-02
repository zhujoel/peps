# Add Pnl to tests
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})

add_library(DateTime STATIC libs/DateTime.cpp) # On lie la librarie au cpp
add_library(DateTimeVector STATIC libs/DateTimeVector.cpp)
add_library(IMarketData STATIC market_data/IMarketData.cpp)
add_library(MathLib STATIC libs/MathLib.cpp)
add_library(IModel STATIC models/IModel.cpp)
add_library(BlackScholesModel STATIC models/BlackScholesModel.cpp)
add_library(IPricer STATIC pricers/IPricer.cpp)
add_library(StandardMonteCarloPricer STATIC pricers/StandardMonteCarloPricer.cpp)
add_library(HistoricalMarketData STATIC market_data/HistoricalMarketData.cpp)
add_library(IDerivative STATIC derivatives/IDerivative.cpp)
add_library(Ocelia STATIC derivatives/Ocelia.cpp)
add_library(QuantoOption STATIC derivatives/QuantoOption.cpp)
