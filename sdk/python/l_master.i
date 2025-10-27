%module l_master
%include  "jsonbase.i"


%{
// #include "lebai/jsonbase.hh"
// #include "lebai/posture.hh"
// #include "lebai/motion.hh"
#include "lebai/robot.hh"
#include "lebai/lua_robot.hh"
#include "lebai/gripper.hh"
%}

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

%extend lebai::l_master::KinematicsForwardResp {
  std::string __repr__() {
    std::string repr = "{pose: {{x: ";
    repr+=std::to_string(self->pose["x"]);
    repr+="}, {y: ";
    repr+=std::to_string(self->pose["y"]);
    repr+="}, {z: ";
    repr+=std::to_string(self->pose["z"]);
    repr+="}, {rz: ";
    repr+=std::to_string(self->pose["rz"]);
    repr+="}, {ry: ";
    repr+=std::to_string(self->pose["ry"]);
    repr+="}, {rx: ";
    repr+=std::to_string(self->pose["rx"]);
    repr+="}}, ";
    repr+="ok: ";
    repr+=std::to_string(self->ok);
    repr+="}";
    return repr;
  }
};


%extend lebai::l_master::KinematicsInverseResp {
  std::string __repr__() {
    std::string repr = "{ok: ";
    repr+=std::to_string(self->ok);
    if(self->ok){
      repr+=", joint_positions: (";
      for (auto it = self->joint_positions.begin(); it != self->joint_positions.end(); ++it) {
        repr += std::to_string(*it);
        repr+=",";
      }
      repr.pop_back();
      repr+=")";
    }
    repr+="}";
    return repr;
  }
};



// __STR__(lebai::MoveParam)
// __STR__(lebai::MoveRequest)
// %ignore lebai::JSONBase;
// %import "lebai/jsonbase.hh"
// %import "lebai/posture.hh"
// %import "lebai/motion.hh"
%include "lebai/robot.hh"
%include "lebai/lua_robot.hh"
%include "lebai/gripper.hh"



// %module lebai
// %include "stdint.i"
// %include "std_vector.i"
// %include "std_string.i"
// %include "std_pair.i"

// %ignore Deserialize;   // Ignore an include guard constant
// %ignore Serialize;   // Ignore an include guard constant
// %ignore InitDocument;   // Ignore an include guard constant

// %template(DoubleVector) std::vector<double>;
// %{
// #include <lebai-sdk/robot.hh>
// #include <lebai-sdk/jsonbase.hh>
// #include <lebai-sdk/product.hh>
// #include <lebai-sdk/factory.hh>
// %}



// %{
// #include <lebai-sdk/robot.hh>

// %}

// // Process symbols in header
// %include "lebai-sdk/robot.hh"
// %include "lebai-sdk/jsonbase.hh"
// %include "lebai-sdk/product.hh"
// %include "lebai-sdk/factory.hh"
