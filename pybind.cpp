// pybind.cpp
#include <pybind11/pybind11.h>
#include "interface.h"
#include <iostream>

namespace py = pybind11;

PYBIND11_MODULE(_c, m)
{
    MyInterface &iface = MyInterface::getInstance();
    m.def("exists", []() -> bool
          { return MyInterface::exists(); }, "Check if shared memory is available and Interface is initialized");

    m.def("get_string", [&iface]() -> std::string
          {
        if (!MyInterface::exists()) {
            throw std::runtime_error("get_string: Shared memory not available");
        }
        std::cout << "get string" << std::endl;
        return iface.data->getString(); }, "Get the string from shared memory");

    m.def("set_string", [&iface](const std::string &new_str)
          {
            if (!MyInterface::exists()) {
                throw std::runtime_error("set_string: Shared memory not available");
        }
        iface.data->setString(new_str); }, "Set the string in shared memory");

    m.def("set_string_notify", [&iface](const std::string &new_str)
          {
            if (!MyInterface::exists()) {
                throw std::runtime_error("set_string_notify: Shared memory not available");
            }
            std::cout << "get string" << std::endl;
            iface.data->setString(new_str);
            iface.data->notify_all(); }, "Get the string from shared memory");
}
