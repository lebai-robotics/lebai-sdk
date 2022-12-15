%module zeroconf
%include "std_vector.i"
%include "std_string.i"
%template(StringVector) std::vector<std::string>;

%{
// #include "lebai/jsonbase.hh"
// #include "lebai/posture.hh"
// #include "lebai/motion.hh"
#include "lebai/discovery.hh"
%}


%template(ControllerInfoVector) std::vector<lebai::zeroconf::ControllerInfo>;

%extend lebai::zeroconf::ControllerInfo {
    std::string __repr__() {
      return self->str();
    }
};

%include "lebai/discovery.hh"
