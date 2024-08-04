// interface.cpp
#include "interface.h"
#include <iostream>

using namespace boost::interprocess;

SharedData::SharedData() {
    strcpy(my_char, "Hello world!");
}

void SharedData::setString(const std::string& new_str) {
    std::cout << "- setString ->" << std::endl;
    scoped_lock<interprocess_upgradable_mutex> lock(upmutex);
    strcpy(my_char, new_str.c_str());
}

std::string SharedData::getString() {
    std::cout << "- getString ->";
    sharable_lock<interprocess_upgradable_mutex> lock(upmutex);
    return std::string(my_char);
}

void SharedData::notify_one() {
    cond.notify_one();
}

void SharedData::notify_all() {
    cond.notify_all();
}
void SharedData::wait() {
    scoped_lock<interprocess_mutex> lock(mutex);
    cond.wait(lock);
}