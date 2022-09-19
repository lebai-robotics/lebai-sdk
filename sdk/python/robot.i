%module robot
%include  "jsonbase.i"


%{
#include "lebai-sdk/jsonbase.hh"
#include "lebai-sdk/posture.hh"
#include "lebai-sdk/motion.hh"
#include "lebai-sdk/robot.hh"
%}


// __STR__(lebai::MoveParam)
// __STR__(lebai::MoveRequest)
// %ignore lebai::JSONBase;
%import "lebai-sdk/jsonbase.hh"
%import "lebai-sdk/posture.hh"
%import "lebai-sdk/motion.hh"
%include "lebai-sdk/robot.hh"



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
