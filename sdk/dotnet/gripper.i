%module gripper

%include "std_string.i"
%include "stdint.i"
%include "exception.i"

%{
#include "lebai/gripper.hh"
%}

// Handle exceptions for C#/.NET
%exception {
    try {
        $action
    } catch (const std::exception &e) {
        SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, e.what());
        return $null;
    } catch (...) {
        SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, "Unknown exception");
        return $null;
    }
}

// Add a C#-friendly ToString method for the Gripper class
%extend lebai::l_master::Gripper {
    std::string ToString() {
        return "Gripper object for RS485/Modbus RTU communication";
    }
}

// Include the gripper header file
%include "lebai/gripper.hh"