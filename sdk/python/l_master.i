%module l_master
%include  "jsonbase.i"


%{
// #include "lebai/jsonbase.hh"
// #include "lebai/posture.hh"
// #include "lebai/motion.hh"
#include "lebai/robot.hh"
%}

%extend lebai::l_master::KinematicsForwardResp {
  std::string __repr__() {
    std::string repr = "(pose: (";
    repr+=std::to_string(self->pose[0]);
    repr+=", ";
    repr+=std::to_string(self->pose[1]);
    repr+=", ";
    repr+=std::to_string(self->pose[2]);
    repr+=", ";
    repr+=std::to_string(self->pose[3]);
    repr+=", ";
    repr+=std::to_string(self->pose[4]);
    repr+=", ";
    repr+=std::to_string(self->pose[5]);
    repr+="), ";
    repr+="ok: ";
    repr+=std::to_string(self->ok);
    repr+=")";
    return repr;
  }
};


%extend lebai::l_master::KinematicsInverseResp {
  std::string __repr__() {
    std::string repr = "(joint_positions: (";
    repr+=std::to_string(self->joint_positions["j1"]);
    repr+=", ";
    repr+=std::to_string(self->joint_positions["j2"]);
    repr+=", ";
    repr+=std::to_string(self->joint_positions["j3"]);
    repr+=", ";
    repr+=std::to_string(self->joint_positions["j4"]);
    repr+=", ";
    repr+=std::to_string(self->joint_positions["j5"]);
    repr+=", ";
    repr+=std::to_string(self->joint_positions["j6"]);
    repr+="), ";
    repr+="ok: ";
    repr+=std::to_string(self->ok);
    repr+=")";
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
