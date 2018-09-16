add_executable(eosbulls.wasm ${CMAKE_CURRENT_SOURCE_DIR}/src/eosbulls.cpp)
target_include_directories(eosbulls.wasm
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(eosbulls.wasm
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/abi/eosbulls.abi" "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)