add_executable(
    exchange
    main.cpp
)

target_link_libraries(exchange PUBLIC matching_engine)