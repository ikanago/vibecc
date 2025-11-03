# Source files for the vibecc compiler library
set(VIBECC_LIB_SOURCES
    codegen.c
    lexer.c
    parser.c
    util.c
    vector.c
)

# Header files (for IDE integration)
set(VIBECC_LIB_HEADERS
    codegen.h
    lexer.h
    parser.h
    util.h
    vector.h
)

# Create a library from the compiler sources (excluding main.c)
add_library(vibecc_lib STATIC ${VIBECC_LIB_SOURCES} ${VIBECC_LIB_HEADERS})

# Set include directories for the library
target_include_directories(vibecc_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Create the main executable
add_executable(vibecc main.c)
target_link_libraries(vibecc vibecc_lib)

# Set output directory for the executable
set_target_properties(vibecc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
