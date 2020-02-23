%begin %{
#ifdef _MSC_VER
#define SWIG_PYTHON_INTERPRETER_NO_DEBUG
#endif
%}
%module gwb
%{
#include <vector>
#include "../include/world_builder/wrapper_cpp.h"
%}

%{
#define SWIG_FILE_WITH_INIT
%}

%include <numpy.i>
%include <std_string.i>

%init %{
import_array();
%}

// using a unsigned int for DIM1 does not work somehow, even
// when defining a typemap for it:
//%numpy_typemaps(unsigned int,    NPY_UINT   , unsigned int)
// Using int for now, since the DIM variable should never contain
// a negtive number anyway and the int range should be large enough 
// for most cases.
// Todo: add some checking somehow?
%apply ( double* INPLACE_ARRAY1, int DIM1) {(double* p_output, unsigned int n_output), (double* p_x, unsigned int n_x),(double* p_z, unsigned int n_z), (double* p_depth, unsigned int n_depth), (double* p_gravity, unsigned int n_gravity)};
%apply ( double* INPLACE_ARRAY1, int DIM1) {(double* p_output, unsigned int n_output), (double* p_x, unsigned int n_x),(double* p_y, unsigned int n_y), (double* p_z, unsigned int n_z), (double* p_depth, unsigned int n_depth), (double* p_gravity, unsigned int n_gravity)};

//%apply ( double* INPLACE_ARRAY1, int DIM1) {(double* p_output, unsigned int n_output), (double* p_x, unsigned int n_x),(double* p_z, unsigned int n_z), (double* p_depth, unsigned int n_depth), (double* p_compositions, unsigned int n_compositions)};

//%apply ( int* INPLACE_ARRAY1, int DIM1) {(unsigned int* p_compositions, unsigned int n_compositions)};

%include "../include/world_builder/wrapper_cpp.h"
