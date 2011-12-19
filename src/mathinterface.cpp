#include "precomp.h"
#include "cbenchanted.h"
#include "mathinterface.h"
#include <math.h>
#include "mathoperations.h"





MathInterface::MathInterface() {
	cb = static_cast <CBEnchanted *> (this);
}

void MathInterface::commandRandomize(void) {
    srand(cb->popValue<int32_t>());
}

void MathInterface::functionInt(void) {
    int v = cast<int32_t>(cb->popValue());
    cb->pushValue(v);
}

void MathInterface::functionFloat(void) {
    float v = cast<float>(cb->popValue());
    cb->pushValue(v);
}

void MathInterface::functionRoundUp(void) {
    float v = cb->popValue<float>();
    cb->pushValue(ceil(v));
}

void MathInterface::functionRoundDown(void) {
    float v = cb->popValue<float>();
    cb->pushValue(floor(v));
}

void MathInterface::functionAbs(void)
{
    boost::any v = cb->popValue();

    if (v.type() == typeid(float))
    {
        cb->pushValue(abs(boost::any_cast<float>(v)));
        return;
    }
    if (v.type() == typeid(int32_t)) //TODO: Check if necessary
    {
        cb->pushValue(abs(boost::any_cast<int>(v)));
    }
    cb->pushValue(v);
}

void MathInterface::functionSqrt(void) {
    float v = cast<float>(cb->popValue());
    cb->pushValue(sqrt(v));
}

void MathInterface::functionSin(void) {
    float angle = cast<float>(cb->popValue());
	
	cb->pushValue(float(sin(angle / 180.0 * 3.14159265358979323)));
}

void MathInterface::functionCos(void) {
    float angle = cast<float>(cb->popValue());
	
	cb->pushValue(float(cos(angle / 180.0 * 3.14159265358979323)));
}

void MathInterface::functionTan(void) {
    float angle = cast<float>(cb->popValue());
    cb->pushValue(float(tan(angle / 180.0 * 3.14159265358979323)));
}

void MathInterface::functionASin(void) {
    float v = cast<float>(cb->popValue());
    cb->pushValue((float)(asinf(v)/3.14159265358979323*180.0));
}

void MathInterface::functionACos(void) {
    float v = cast<float>(cb->popValue());
    cb->pushValue((float)(acosf(v)/3.14159265358979323*180.0));
}

void MathInterface::functionATan(void) {
    float v = cast<float>(cb->popValue());
    cb->pushValue((float)(atanf(v)/3.14159265358979323*180.0));
}

void MathInterface::functionGetAngle(void) {
    float y2 = cast<float>(cb->popValue());
    float x2 = cast<float>(cb->popValue());
    float y1 = cast<float>(cb->popValue());
    float x1 = cast<float>(cb->popValue());
    cb->pushValue((float)((3.14159265358979323 - atan2f(y1-y2,x1-x2))/3.14159265358979323*180.0)); //TODO: Check if works
}

void MathInterface::functionLog(void) {
    float v = cast<float>(cb->popValue());
    cb->pushValue(logf(v));
}

void MathInterface::functionLog10(void) {
    float v = cast<float>(cb->popValue());
    cb->pushValue(log10f(v));
}

void MathInterface::functionRnd(void) {
    float high = cast<float>(cb->popValue());
    float low = cast<float>(cb->popValue());
    cb->pushValue(low + ((float)rand() / RAND_MAX)*(high-low)); //TODO: Better generation (Windows RAND_MAX == 0x7fff)
}

void MathInterface::functionRand(void) {
    int32_t high = cast<int32_t>(cb->popValue());
    int32_t low = cast<int32_t>(cb->popValue());
    cb->pushValue(low + rand()%(high-low)); //TODO: Better generation (Windows RAND_MAX == 0x7fff)
}

void MathInterface::functionMin(void) {
    boost::any b = cb->popValue();
    boost::any a = cb->popValue();
    cb->pushValue( (a < b) ? a : b);
}

void MathInterface::functionMax(void) {
    boost::any b = cb->popValue();
    boost::any a = cb->popValue();
    cb->pushValue( (a > b) ? a : b);
}

void MathInterface::functionCurveValue(void) {
    float smoothness = cast<float>(cb->popValue());
    float oldV = cast<float>(cb->popValue());
    float newV = cast<float>(cb->popValue());
    cb->pushValue(oldV + (newV-oldV)/smoothness);
}

inline float wrapAngle(float v)
{
    while(v > 360.0f)
    {
        v -= 360.0f;
    }
    while (v < 0.0f)
    {
        v += 360.0f;
    }
    return v;
}

void MathInterface::functionCurveAngle(void) {
    float smoothness = cast<float>(cb->popValue());
    float oldA = cast<float>(cb->popValue());
    float newA = cast<float>(cb->popValue());
    oldA = wrapAngle(oldA);
    newA = wrapAngle(newA);

    if (abs(newA - oldA) < (360.0-newA) + oldA)
    {
        cb->pushValue((oldA + (newA - oldA)/smoothness));
    }
    else
    {
        cb->pushValue((oldA - ((360.0f-newA) + oldA)/smoothness)+360.0f);
    }

}

void MathInterface::functionWrapAngle(void) {
    boost::any a = cb->popValue();
    if (a.type() == typeid(float))
    {
        float angle = boost::any_cast<float>(a);
        while (angle > 360.0) {
            angle -= 360.0;
        }
        while (angle < 0.0) {
            angle += 360.0;
        }
        cb->pushValue(angle);
        return;
    }
    if (a.type() == typeid(int32_t))
    {
        int32_t angle = boost::any_cast<int32_t>(a);
        while (angle > 360) {
            angle -= 360;
        }
        while (angle < 0) {
            angle += 360;
        }
        cb->pushValue(angle);
        return;
    }
    if (a.type() == typeid(uint16_t))
    {
        uint16_t angle = boost::any_cast<uint16_t>(a);
        while (angle > 360) {
            angle -= 360;
        }
        cb->pushValue(angle);
        return;
    }
    cb->pushValue(a);
}


//Helper
inline double square(float num)
{
    return (double)num*(double)num;
}

void MathInterface::functionDistance(void) {
    float y2 = cast<float>(cb->popValue());
    float x2 = cast<float>(cb->popValue());
    float y1 = cast<float>(cb->popValue());
    float x1 = cast<float>(cb->popValue());
    cb->pushValue((float)sqrt(square(x2-x1)+square(y2-y1)));
}

void MathInterface::functionBoxOverlap(void) {
    float height2h = cast<float>(cb->popValue())*0.5f;
    float width2h = cast<float>(cb->popValue())*0.5f;
    float y2 = cast<float>(cb->popValue());
    float x2 = cast<float>(cb->popValue());
    float height1h = cast<float>(cb->popValue())*0.5f;
    float width1h = cast<float>(cb->popValue())*0.5f;
    float y1 = cast<float>(cb->popValue());
    float x1 = cast<float>(cb->popValue());

    cb->pushValue((abs(y1-y2) < (height1h+height2h)) && ((abs(x1-x2) < (width1h+width2h))) ? 1:0);


}
