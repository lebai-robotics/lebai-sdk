%module gripper_module

%include "std_string.i"
%include "stdint.i"
%include "exception.i"

%{
#include "lebai/gripper.hh"
%}

// Handle exceptions for Java
%exception {
    try {
        $action
    } catch (const std::exception &e) {
        SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, e.what());
        return $null;
    } catch (...) {
        SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unknown exception");
        return $null;
    }
}

// Add a Java-friendly toString method for the Gripper class
%extend lebai::l_master::Gripper {
    std::string toString() {
        return "Gripper object for RS485/Modbus RTU communication";
    }
}

// Include the gripper header file
%include "lebai/gripper.hh"
