cmake_minimum_required(VERSION 3.0)
project(peps)

# Detect PNL
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})

# sources and headers
set(SOURCES src/IUnderlying.cpp src/ForeignUnderlying src/IDerivative.cpp src/IModel.cpp src/DateTime.cpp src/BlackScholesModel.cpp 
src/IPricer.cpp src/StandardMonteCarloPricer.cpp src/QuantoOption.cpp src/Ocelia.cpp src/main.cpp)

set(HEADERS include/IUnderlying.h include/DomesticUnderlying.h include/IDerivative.h include/IModel.h 
include/ForeignUnderlying.h include/DateTime.h include/BlackScholesModel.h 
include/IPricer.h include/StandardMonteCarloPricer.h include/Ocelia.h 
include/QuantoOption.h)

include_directories(include)
add_executable(main src/main.cpp ${SOURCES} ${HEADERS})
target_link_libraries(main ${LIBS})