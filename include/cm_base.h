#ifndef CM_BASE_HPP
#define CM_BASE_HPP
#include <cstddef>

namespace cminus {

enum cm_type {CM_INT, CM_VOID, CM_INT_ARRAY};
typedef int cm_size_type;
typedef int cm_int_type;
enum cm_ops {cm_plus, cm_minus, cm_multi, cm_div};
enum cm_relops {cm_gt, cm_lt, cm_ge, cm_le, cm_equ, cm_ueq};

};
#endif
