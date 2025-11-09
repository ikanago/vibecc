add_subdirectory(unit)

add_custom_target(integration_tests
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/integration_tests.sh
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS vibecc
)
