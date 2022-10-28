%include "stdint.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_string.i"
%include "std_pair.i"
%include "std_array.i"
%include "std_tuple.i"

%ignore Deserialize;
%ignore Serialize;
%ignore InitDocument;
%ignore IsNullJSONData;

%template(DoubleVector) std::vector<double>;

%extend std::map {
    std::string __repr__() {
      std::string repr = "{";
      for (auto it = self->begin(); it != self->end(); ++it) {
        repr += "'" + it->first + ": " + std::to_string(it->second) + "',";
      }
      repr.pop_back();
      repr+="}";
      return repr;
    }
};


%extend std::vector {
    std::string __repr__() {
      std::string repr = "(";
      for (auto it = self->begin(); it != self->end(); ++it) {
        repr += std::to_string(it) ",";
      }
      repr.pop_back();
      repr+=")";
      return repr;
    }
};

%extend std::tuple<double, double, bool> {
    std::string __repr__() {
      std::string repr = "(";      
      repr+=std::to_string(std::get<0>(*self));
      repr+=", ";
      repr+=std::to_string(std::get<1>(*self));
      repr+=", ";
      repr+=std::to_string(std::get<2>(*self));
      repr+=")";
      return repr;
    }
};


%extend std::array {
    std::string __repr__() {
      std::string repr = "(";      
      repr+=std::to_string((*self)[0]);
      repr+=", ";
      repr+=std::to_string((*self)[1]);
      repr+=", ";
      repr+=std::to_string((*self)[2]);
      repr+=", ";
      repr+=std::to_string((*self)[3]);
      repr+=", ";
      repr+=std::to_string((*self)[4]);
      repr+=", ";
      repr+=std::to_string((*self)[5]);
      repr+=")";
      return repr;
    }
};

%template(CartArray) std::array<double, 6>;
%template(JointMap) std::map<std::string, double>;
%std_tuple(TupleDDB, double, double, bool);
%std_tuple(TupleIntStr, int, std::string);






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



