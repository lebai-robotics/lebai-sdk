%module motion
%include  "jsonbase.i"

%{
#include "lebai-sdk/posture.hh"
#include "lebai-sdk/motion.hh"
%}


__STR__(lebai::MoveParam)
__STR__(lebai::MoveRequest)
%ignore lebai::JSONBase;
%import "lebai-sdk/jsonbase.hh"
%import "lebai-sdk/posture.hh"
%include "lebai-sdk/motion.hh"




// %module pyMotion
// %include "stdint.i"
// %include "std_vector.i"
// %include "std_string.i"
// %include "std_pair.i"
// %import  "posture.i"

// %ignore Deserialize;
// %ignore Serialize;
// %ignore InitDocument;
// %ignore IsNullJSONData;

// %template(DoubleVector) std::vector<double>;
// %{
// #include <lebai-sdk/jsonbase.hh>
// // #include "lebai-sdk/posture.hh"
// #include "lebai-sdk/motion.hh"
// %}

// // Process symbols in header
// // %naturalvar JointPose::joints;

// // %feature("python:slot", "tp_str", functype="reprfunc") class_name::py_to_string();


// // %extend lebai::JointPose {
// //     std::string __repr__() {
// //       return self->ToJSONString();
// //     }
// // };

// %define __STR__(class_name) 
// //%feature("python:slot", "tp_str", functype="reprfunc") class_name::py_to_string();
// %extend class_name{
//     std::string __repr__() {
//       return self->ToJSONString();
//     }
// }
// %enddef


// __STR__(lebai::MoveParam)
// __STR__(lebai::MoveRequest)

// %include "lebai-sdk/jsonbase.hh"
// %include "lebai-sdk/motion.hh"


