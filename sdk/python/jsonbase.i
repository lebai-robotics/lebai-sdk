%include "stdint.i"
%include "std_vector.i"
%include "std_string.i"
%include "std_pair.i"

%ignore Deserialize;
%ignore Serialize;
%ignore InitDocument;
%ignore IsNullJSONData;

%template(DoubleVector) std::vector<double>;
%{
#include <lebai-sdk/jsonbase.hh>
%}


%define __STR__(class_name) 
//%feature("python:slot", "tp_str", functype="reprfunc") class_name::py_to_string();
%extend class_name{
    std::string __repr__() {
      return self->ToJSONString();
    }
}
%enddef



