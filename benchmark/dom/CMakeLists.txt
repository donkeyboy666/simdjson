include_directories( .. ../linux )
link_libraries(simdjson-windows-headers test-data)
link_libraries(simdjson)

add_executable(perfdiff perfdiff.cpp)
add_executable(parse parse.cpp)
add_executable(parse_stream parse_stream.cpp)
add_executable(statisticalmodel statisticalmodel.cpp)

add_executable(parse_noutf8validation parse.cpp)
target_compile_definitions(parse_noutf8validation PRIVATE SIMDJSON_SKIPUTF8VALIDATION)
add_executable(parse_nonumberparsing parse.cpp)
target_compile_definitions(parse_nonumberparsing PRIVATE SIMDJSON_SKIPNUMBERPARSING)
add_executable(parse_nostringparsing parse.cpp)
target_compile_definitions(parse_nostringparsing PRIVATE SIMDJSON_SKIPSTRINGPARSING)
include(checkperf.cmake)
