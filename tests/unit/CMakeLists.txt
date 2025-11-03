# Unit tests configuration using Google Test
file(GLOB_RECURSE UNIT_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

if(UNIT_TEST_SOURCES)
    add_executable(vibecc_tests ${UNIT_TEST_SOURCES})

    # Link with Google Test and the compiler library
    target_link_libraries(vibecc_tests gtest_main vibecc_lib)

    # Use GoogleTest's test discovery
    include(GoogleTest)
    gtest_discover_tests(vibecc_tests)

    # Set output directory
    set_target_properties(vibecc_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    # Add a custom target for running tests
    add_custom_target(run_tests
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS vibecc_tests
        COMMENT "Running unit tests"
    )
else()
    message(STATUS "No unit test sources found in tests/unit/")
endif()
