// main.cpp
#include "interface.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <csignal>

std::atomic<bool> running(true);

void printLoop()
{
    MyInterface &iface = MyInterface::getInstance();
    while (running)
    {
        iface.data->wait();
        if (!running)
            break;
        std::cout << "Add-worker: \t";
        std::string s = iface.data->getString() + "...";
        std::cout << s << std::endl;
        iface.data->setString(s);
    }
    std::cout << "Add-worker stopped." << std::endl;
}

int main()
{
    try
    {
        MyInterface &iface = MyInterface::initInstance();
        std::thread worker(printLoop);

        signal(SIGINT, [](int) {
                running = false;
                MyInterface &iface = MyInterface::getInstance();
                iface.data->notify_all();
            }
        );

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Press Ctrl-C to stop..." << std::endl;
        while (running)
        {
            std::cout << "Main: \t\t" << iface.data->getString() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        if (worker.joinable())
        {
            worker.join();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
