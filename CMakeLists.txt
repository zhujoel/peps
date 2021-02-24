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
set(SOURCES src/derivatives/IDerivative.cpp src/derivatives/Ocelia.cpp src/derivatives/QuantoOption.cpp 
src/models/IModel.cpp src/models/BlackScholesModel.cpp 
src/libs/Utilities.cpp src/libs/DateTime.cpp src/libs/DateTimeVector.cpp src/libs/MathLib.cpp 
src/pricers/IPricer.cpp src/pricers/StandardMonteCarloPricer.cpp 
src/market_data/IMarketData.cpp src/market_data/HistoricalMarketData.cpp 
src/datafeeds/IDataFeed.cpp src/datafeeds/FTSEDataFeed.cpp src/datafeeds/YahooDataFeed.cpp
src/main.cpp)

set(HEADERS include/derivatives/IDerivative.h include/derivatives/Ocelia.h include/derivatives/QuantoOption.h 
include/models/IModel.h include/models/BlackScholesModel.h 
include/libs/Utilities.h include/libs/DateTime.h include/libs/DateTimeVector.h include/libs/MathLib.h 
include/pricers/IPricer.h include/pricers/StandardMonteCarloPricer.h 
include/market_data/HistoricalMarketData.h include/market_data/IMarketData.h
include/datafeeds/IDataFeed.h include/datafeeds/FTSEDataFeed.h include/datafeeds/YahooDataFeed.h)

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
add_test (NAME test_DataFeeds COMMAND test_DataFeeds)
add_test (NAME test_BlackScholesModel COMMAND test_BlackScholesModel)
add_test (NAME test_StandardMonteCarloPricer COMMAND test_StandardMonteCarloPricer)
add_test (NAME test_Utilities COMMAND test_Utilities)

pnl_add_postbuild(run_unit_tests)