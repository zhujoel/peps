cmake_minimum_required(VERSION 3.0)

project(Demo)

set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories(include)

add_subdirectory(src) # AJOUTER CELA 

# Detect PNL
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})

# sources and headers
set(SOURCES  src/IDerivative.cpp src/QuantoOption.cpp src/models/IModel.cpp src/DateTime.cpp 
src/BlackScholesModel.cpp src/IPricer.cpp src/DateTimeVector.cpp src/StandardMonteCarloPricer.cpp 
src/Ocelia.cpp src/IMarketData.cpp src/HistoricalMarketData.cpp src/MathLib.cpp src/main.cpp)

set(HEADERS include/IDerivative.h include/Ocelia.h include/QuantoOption.h include/models/IModel.h 
include/DateTimeVector.h include/DateTime.h include/BlackScholesModel.h 
include/IPricer.h include/StandardMonteCarloPricer.h include/HistoricalMarketData.h include/MathLib.h include/IMarketData.h)

include_directories(include ${PNL_INCLUDE})
add_executable(main src/main.cpp ${SOURCES} ${HEADERS})
target_link_libraries(main ${LIBS})

# google-test
enable_testing ()

# Télécharge googletest au moment de la configureation
configure_file(CMakeLists.txt.in
               googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

# Ajout googletest directement au moment de la compilation
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src)

include_directories(${gtest_SOURCE_DIR}/include)
add_subdirectory(tests)

add_test (NAME test_DateTime COMMAND test_DateTime)
add_test (NAME test_DateTimeVector COMMAND test_DateTimeVector)
add_test (NAME test_MathLib COMMAND test_MathLib)
add_test (NAME test_Ocelia COMMAND test_Ocelia)
add_test (NAME test_Quanto COMMAND test_Quanto)
add_test (NAME test_HistoricalMarketData COMMAND test_HistoricalMarketData)

pnl_add_postbuild(run_unit_tests)