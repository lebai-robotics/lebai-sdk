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

%include "lebai/robot.hh"
