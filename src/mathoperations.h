#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H
#include "any.h"

Any operator ! (const Any &l);
Any operator + (const Any &l);
Any operator - (const Any &l);
Any operator % (const Any &l, const Any &r);
Any operator * (const Any &l, const Any &r);
Any operator + (const Any &l, const Any &r);
Any operator - (const Any &l, const Any &r);
Any operator / (const Any &l, const Any &r);
Any operator << (const Any &l, const Any &r);
Any shr (const Any &l, const Any &r);
Any sar (const Any &l, const Any &r);
Any operator ^ (const Any &l, const Any &r);
int32_t operator != (const Any &l, const Any &r);
int32_t operator && (const Any &l, const Any &r);
int32_t operator <= (const Any &l, const Any &r);
int32_t operator == (const Any &l, const Any &r);
int32_t operator >= (const Any &l, const Any &r);
int32_t operator || (const Any &l, const Any &r);
int32_t operator > (const Any &l, const Any &r);
int32_t operator < (const Any &l, const Any &r);



#endif


