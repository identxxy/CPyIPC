// singleton.h
#ifndef INTERFACE_BASE_H
#define INTERFACE_BASE_H

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>

using namespace boost::interprocess;

template <typename T>
class Interface
{
public:
    static Interface &initInstance()
    {
        static Interface instance(true);
        return instance;
    }

    static Interface &getInstance()
    {
        static Interface instance(false);
        return instance;
    }

    static bool exists()
    {
        try
        {
            shared_memory_object shm(open_only, typeid(T).name(), read_write);
            return true;
        }
        catch (interprocess_exception &)
        {
            return false;
        }
    }

    T* data;

protected:
    ~Interface()
    {
        if (allocate)
        {
            shared_memory_object::remove(typeid(T).name());
            // std::cout << "Destory" << std::endl;
        }
        // else    std::cout << "No destory." << std::endl;
    }

    Interface(const bool allocate) : allocate(allocate)
    {
        try
        {
            // std::cout << (allocate ? "Creat " : "Access ") << typeid(T).name() << std::endl;
            if (allocate) {
                shared_memory_object::remove(typeid(T).name());
                shm = shared_memory_object(create_only, typeid(T).name(), read_write);
                shm.truncate(sizeof(T));
                region = mapped_region(shm, read_write);
                //Get the address of the mapped region
                void * addr = region.get_address();
                //Construct the shared structure in memory
                data = new (addr) T;
            }
            else {
                shm = shared_memory_object(open_only, typeid(T).name(), read_write);
                region = mapped_region(shm, read_write);
                data = static_cast<T*>(region.get_address());
            }
        }
        catch (interprocess_exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            try
            {
                shm = shared_memory_object(open_only, typeid(T).name(), read_only);
                std::cerr << "Existing shared memory handler " << shm.get_mapping_handle().handle << std::endl;
            }
            catch (interprocess_exception &e)
            {
            }
            throw std::runtime_error("Shared memory not available");
        }
    }

private:
    Interface(const Interface &) = delete;
    Interface &operator=(const Interface &) = delete;
    shared_memory_object shm;
    mapped_region region;
    bool allocate;
};

#endif // INTERFACE_BASE_H