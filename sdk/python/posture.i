%module posture
%include  "jsonbase.i"



%{
#include <lebai/posture.hh>
%}

__STR__(lebai::JointPose)
__STR__(lebai::JointFrame)
__STR__(lebai::JointTargetPose)
__STR__(lebai::Pose)
%ignore lebai::JSONBase;

%typemap(doctype) const std::vector<double> & "const array of double";
%typemap(doctype) std::vector<double> & "array of double";


%import "lebai/jsonbase.hh"
%include "lebai/posture.hh"





// %module pyPosture
// %include "stdint.i"
// %include "std_vector.i"
// %include "std_string.i"
// %include "std_pair.i"

// %ignore Deserialize;
// %ignore Serialize;
// %ignore InitDocument;
// %ignore IsNullJSONData;

// %template(DoubleVector) std::vector<double>;


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

// __STR__(lebai::JointPose)
// __STR__(lebai::JointFrame)
// __STR__(lebai::JointTargetPose)
// __STR__(lebai::Pose)

// %{
// #include <lebai-sdk/jsonbase.hh>
// #include <lebai-sdk/posture.hh>
// %}

// %include "lebai-sdk/jsonbase.hh"
// %include "lebai-sdk/posture.hh"


