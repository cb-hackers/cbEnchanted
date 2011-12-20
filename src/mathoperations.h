#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H
#include "any.h"

Any operator ! (Any &l);
Any operator + (Any &l);
Any operator - (Any &l);
Any operator % (Any &l, Any &r);
Any operator * (Any &l, Any &r);
Any operator + (Any &l, Any &r);
Any operator - (Any &l, Any &r);
Any operator / (Any &l, Any &r);
Any operator << (Any &l, Any &r);
Any operator >> (Any &l, Any &r);
Any operator ^ (Any &l, Any &r);
int32_t operator != (Any &l, Any &r);
int32_t operator && (Any &l, Any &r);
int32_t operator <= (Any &l, Any &r);
int32_t operator == (Any &l, Any &r);
int32_t operator >= (Any &l, Any &r);
int32_t operator || (Any &l, Any &r);
int32_t operator > (Any &l, Any &r);
int32_t operator < (Any &l, Any &r);



#endif


