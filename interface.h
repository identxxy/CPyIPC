// interface.h
#ifndef INTERFACE_H
#define INTERFACE_H

#include "interface_base.h"
#include <string>
#include <mutex>

/************************************
Synchronization refer to 
    https://www.boost.org/doc/libs/1_72_0/doc/html/interprocess/synchronization_mechanisms.html
Choose the mechanism based on demand.
*************************************/
// #include <boost/interprocess/sync/interprocess_mutex.hpp>
// #include <boost/interprocess/sync/interprocess_recursive_mutex.hpp>
// #include <boost/interprocess/sync/shared_mutex.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/upgradable_lock.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

// The shared data class.
// The name of the class will be used as the shared memory name.
// Boost will create a shared memory file 
// at the path /dev/{nName}, where n is a random number.
class SharedData {
    // IMPORTANT: ONLY char[] allowed, NO std::string!
    char my_char[128];
    // For read-write lock
    boost::interprocess::interprocess_upgradable_mutex upmutex;
    // For condition variable
    boost::interprocess::interprocess_mutex mutex;
    // To notify change
    boost::interprocess::interprocess_condition cond;

public:
    SharedData();
    void setString(const std::string& new_str);
    std::string getString();
    void wait();
    void notify_one();
    void notify_all();
};

template class Interface<SharedData>;
typedef Interface<SharedData> MyInterface;

#endif // INTERFACE_H
