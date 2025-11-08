enable_testing()

add_executable(
    vibecc_tests
    map_test.cc
)
target_link_libraries(vibecc_tests gtest_main vibecc_lib)

include(GoogleTest)
gtest_discover_tests(vibecc_tests)

add_custom_target(unit_test
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS vibecc_tests
)
