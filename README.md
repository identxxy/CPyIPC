# Boost Interprocess Pybind11

This repo offers a simple interface between C++ and Python processes, using `Boost::interprocess` and shared memory.  

You need to implement **shared data class** in `interface.cpp` and `interface.hpp`, and bind desired methods in `pybind.cpp`, then you can use the this **unique** class across different threads in different processes. Note that only one thread (owner) creats and destorys the shared memory, others only open the memory and do read-write. Synchronization is necessary in the shared data class. For more info, refer to [synchronization_mechanisms in Boost::interprocess](https://www.boost.org/doc/libs/1_72_0/doc/html/interprocess/synchronization_mechanisms.html) .

## Design
```
                Process memory          Shared meomry

Thread 1    |
Thread 2    |-- Process 1 Interface |
                                    |
Thread 1    |                       |
Thread 2    |-- Process 2 Interface |-- SharedData
                                    |
Python module ----------------------|
```

The **shared data class**, e.g. with the class name `SharedData`, is stored in the shared memory (at the path `/dev/*SharedData`).

Each process has an `typedef Interface<SharedData> MyInterface` which is a singleton for each process, and different across processes, as processes have different memory address space.

One thread creats the `Data` by `MyInterface::initInstacne()`. The memory will be destoryed by that thread when the variable is out of scope. Others threads connect to shared memory by `MyInterface::getInstance()`, and do not destory the shared memory.

## Example

In `interface.h` the essential data in `SharedData` is simply `char my_char[128]`  
**IMPORTANT** You cannot use `std::string` in shared memory, because the char memory address is not static.


Two C++ main are provided with a python module named `_c`.

```
main
    |-- main thread
        Manage the memory. Print my_char each 1 seconds.
    |-- Add-worker thread
        blocked by wait(). Once notified, Add "..." to the end to my_char.

main2
    |-- Input thread
        Require the user for input, change my_char and notify_all.
        So the Add-worker thread in main will add "...".

python module _c
$ python
>>> import _c
>>> _c.get_string()
...
>>> _c.set_string("whatever you like.")
...
>>> _c.set_string_ontify("whatever you like.")
...
```

Besides, you can find the location of the shared meomry at `/dev/shm`, and inspect it.
```
$ ls /dev/shm
10SharedData
$ cat /dev/shm/10SharedData
```

# Compile
You may change the way to find `pybind11` in `CMakeLists.txt`

The `interface.cpp` is compiled to a library `libc.a`. If other program needs to use the `MyInterface`, include the `interface.hpp` and add `target_link_libraries(<your_program> PRIVATE c Boost::interprocess)` to the `CMakeLists.txt`.