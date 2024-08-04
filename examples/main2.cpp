// main.cpp
#include "interface.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <csignal>

std::atomic<bool> running(true);


int main()
{
    try
    {
        MyInterface &iface = MyInterface::getInstance();

        signal(SIGINT, [](int)
               { running = false; });

        std::cout << "Press Ctrl-C to stop..." << std::endl;
        while (running)
        {
            std::cout << "Main2: \t" << iface.data->getString() << std::endl;
            std::string s;
            std::getline(std::cin, s);
            if (!running)
                break;
            std::cout << "Main2: \t" << s << std::endl;
            iface.data->setString(s);
            // Tell everyone we have change.
            iface.data->notify_all();
        }

    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
