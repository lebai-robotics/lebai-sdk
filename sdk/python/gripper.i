%module gripper

%include "std_string.i"
%include "stdint.i"
%include "exception.i"

%{
#include "lebai/gripper.hh"
%}

// Handle exceptions for Python
%exception {
    try {
        $action
    } catch (const std::exception &e) {
        PyErr_SetString(PyExc_Exception, e.what());
        return NULL;
    } catch (...) {
        PyErr_SetString(PyExc_Exception, "An unknown exception occurred");
        return NULL;
    }
}

// Add a Python-friendly representation for the Gripper class
%extend lebai::l_master::Gripper {
    std::string __repr__() {
        return "Gripper(port_name)";
    }

    std::string __str__() {
        return "Gripper object for RS485/Modbus RTU communication";
    }
}

// Include the gripper header file
%include "lebai/gripper.hh"