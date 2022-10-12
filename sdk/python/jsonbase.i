%include "stdint.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_string.i"
%include "std_pair.i"
%include "std_array.i"

%ignore Deserialize;
%ignore Serialize;
%ignore InitDocument;
%ignore IsNullJSONData;

%template(DoubleVector) std::vector<double>;
%template(CartArray) std::array<double, 6>;
%template(JointMap) std::map<std::string, double>;
%{
#include <protos/jsonbase.hh>
%}


%define __STR__(class_name) 
//%feature("python:slot", "tp_str", functype="reprfunc") class_name::py_to_string();
%extend class_name{
    std::string __repr__() {
      return self->ToJSONString();
    }
}
%enddef



