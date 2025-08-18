find_package(benchmark REQUIRED)

add_executable(orderbook_benchmarks orderbook_bench.cpp)

target_link_libraries(orderbook_benchmarks benchmark::benchmark pthread)
