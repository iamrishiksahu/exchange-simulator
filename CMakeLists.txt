cmake_minimum_required(VERSION 3.14)
project(exchange LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(exchange)
add_subdirectory(matching_engine)
add_subdirectory(orderbook)
add_subdirectory(order_entry)
add_subdirectory(market_data)
add_subdirectory(tests)