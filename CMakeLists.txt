cmake_minimum_required(VERSION 3.14)
project(exchange LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 2. Bring in FetchContent module
include(FetchContent)

# 3. Declare GoogleTest dependency (where to fetch from)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# 4. Download + add googletest’s CMake project
FetchContent_MakeAvailable(googletest)

# 5. Enable testing infrastructure in CMake
enable_testing()


add_subdirectory(exchange)
add_subdirectory(matching_engine)
add_subdirectory(orderbook)
add_subdirectory(order_entry)
add_subdirectory(market_data)
add_subdirectory(tests)
add_subdirectory(benchmarks)