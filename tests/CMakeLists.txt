add_executable(test_DateTime test_DateTime.cxx)
target_link_libraries(test_DateTime gtest DateTime Utilities ${PNL_LIBRARIES}) # Attention l'ordre compte

add_executable(test_DateTimeVector test_DateTimeVector.cxx)
target_link_libraries(test_DateTimeVector gtest DateTimeVector DateTime Utilities ${PNL_LIBRARIES}) 

add_executable(test_MathLib test_MathLib.cxx)
target_link_libraries(test_MathLib gtest MathLib HistoricalMarketData IMarketData MarketDataFeed IDataFeed DateTimeVector DateTime Utilities ${PNL_LIBRARIES})

add_executable(test_Utilities test_Utilities.cxx)
target_link_libraries(test_Utilities gtest Utilities ${PNL_LIBRARIES})

add_executable(test_Ocelia test_Ocelia.cxx)
target_link_libraries(test_Ocelia gtest Ocelia IMarketData IDerivative DateTimeVector DateTime Utilities ${PNL_LIBRARIES})

add_executable(test_Quanto test_Quanto.cxx)
target_link_libraries(test_Quanto gtest QuantoOption BlackScholesModel StandardMonteCarloPricer MathLib IPricer IModel IDerivative Utilities ${PNL_LIBRARIES})

add_executable(test_DataFeeds test_DataFeeds.cxx)
target_link_libraries(test_DataFeeds gtest MarketDataFeed IDataFeed DateTimeVector DateTime Utilities ${PNL_LIBRARIES})

add_executable(test_HistoricalMarketData test_HistoricalMarketData.cxx)
target_link_libraries(test_HistoricalMarketData gtest HistoricalMarketData IMarketData MarketDataFeed IDataFeed DateTimeVector DateTime Utilities ${PNL_LIBRARIES})

add_executable(test_BlackScholesModel test_BlackScholesModel.cxx)
target_link_libraries(test_BlackScholesModel gtest BlackScholesModel IModel MathLib HistoricalMarketData IMarketData MarketDataFeed IDataFeed DateTimeVector DateTime Utilities ${PNL_LIBRARIES})

add_executable(test_StandardMonteCarloPricer test_StandardMonteCarloPricer.cxx)
target_link_libraries(test_StandardMonteCarloPricer gtest StandardMonteCarloPricer IPricer Ocelia IDerivative BlackScholesModel IModel MathLib HistoricalMarketData IMarketData MarketDataFeed IDataFeed DateTimeVector DateTime Utilities HedgingPortfolio ${PNL_LIBRARIES})
