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

%include "lebai/discovery.hh"
