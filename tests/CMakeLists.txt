add_executable(test_DateTime test_DateTime.cxx)
target_link_libraries(test_DateTime gtest DateTime ${PNL_LIBRARIES}) # Attention l'ordre compte

add_executable(test_DateTimeVector test_DateTimeVector.cxx)
target_link_libraries(test_DateTimeVector gtest DateTimeVector DateTime ${PNL_LIBRARIES}) # Attention l'ordre compte
