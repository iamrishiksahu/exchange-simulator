#include "../orderbook/orderbook.h"
#include "signal.h"
#include <chrono>
#include <iostream>
#include <thread>

bool is_running = false;
void signalHandler(int);

int main(int argc, char *argv[])
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    std::cout << "Starting Exchange!\n";

    exch_ns::Orderbook ob;

    while (is_running)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void signalHandler(int signal)
{
    std::cout << "Application terminate requested: " << signal << ", shutting down!\n";
    is_running = false;
}