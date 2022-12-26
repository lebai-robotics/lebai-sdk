%include "stdint.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_string.i"
%include "std_pair.i"
%include "std_array.i"

#if defined(SWIGCSHARP)
#if defined(SWIGWORDSIZE64)
// By default SWIG map C++ long int (i.e. int64_t) to C# int
// But we want to map it to C# long so we reuse the typemap for C++ long long.
// ref: https://github.com/swig/swig/blob/master/Lib/csharp/typemaps.i
// ref: https://docs.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/integral-numeric-types
%define PRIMITIVE_TYPEMAP(NEW_TYPE, TYPE)
%clear NEW_TYPE;
%clear NEW_TYPE *;
%clear NEW_TYPE &;
%clear const NEW_TYPE &;
%apply TYPE { NEW_TYPE };
%apply TYPE * { NEW_TYPE * };
%apply TYPE & { NEW_TYPE & };
%apply const TYPE & { const NEW_TYPE & };
%enddef // PRIMITIVE_TYPEMAP
PRIMITIVE_TYPEMAP(long int, long long);
PRIMITIVE_TYPEMAP(unsigned long int, unsigned long long);
#undef PRIMITIVE_TYPEMAP
#endif // defined(SWIGWORDSIZE64)
#endif // defined(SWIGCSHARP)

%ignore Deserialize;
%ignore Serialize;
%ignore InitDocument;
%ignore IsNullJSONData;

%template(DoubleVector) std::vector<double>;

%{
#include <protos/jsonbase.hh>
#include <array>
%}

%extend std::map {
    std::string __repr__() {
      std::string repr = "{";
      for (auto it = self->begin(); it != self->end(); ++it) {
        repr += "'" + it->first + "': " + std::to_string(it->second) + ",";
      }
      repr.pop_back();
      repr+="}";
      return repr;
    }
};


// %extend std::vector<unsigned int> {
    // std::string __repr__() {
      // std::string repr = "(";
      // for (auto it = self->begin(); it != self->end(); ++it) {
        // repr += std::to_string(it);
        // repr+=",";
      // }
      // repr.pop_back();
      // repr+=")";
      // return repr;
    // }
// };

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
%template(UintVector) std::vector<unsigned int>;
%template(StrVector) std::vector<std::string>;
%template(IntVector) std::vector<int>;
// %template(DoubleVector) std::vector<double>;
%template(BSVector) std::vector<std::tuple<bool,std::string>>;

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



