add_subdirectory(unit)

add_custom_target(integration_tests
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/integration/tests.sh
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS vibecc
)
