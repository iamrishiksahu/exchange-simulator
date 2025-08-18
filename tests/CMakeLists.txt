# Include GoogleTest helpers
include(GoogleTest)

# Define test executable(s)
add_executable(orderbook_tests orderbook_tests.cpp)

# Link against GoogleTest
target_link_libraries(orderbook_tests gtest_main)

# Register with CTest
gtest_discover_tests(orderbook_tests)
