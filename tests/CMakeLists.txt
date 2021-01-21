add_executable(test_DateTime test_DateTime.cxx)
target_link_libraries(test_DateTime gtest DateTime) # Attention l'ordre compte

add_executable(test_DateTimeVector test_DateTimeVector.cxx)
target_link_libraries(test_DateTimeVector gtest DateTimeVector DateTime ${PNL_LIBRARIES}) 

add_executable(test_Ocelia test_Ocelia.cxx)
target_link_libraries(test_Ocelia gtest Ocelia SimulatedMarketData IMarketData IDerivative ForeignUnderlying IUnderlying DateTimeVector DateTime ${PNL_LIBRARIES})