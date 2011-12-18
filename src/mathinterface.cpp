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
    boost::any a = cb->popValue();
    if (a.type() == typeid(float)) {
        cb->pushValue((int32_t)boost::any_cast<float>(a));
        return;
    }
    if (a.type() == typeid(string)) {
        try {
            cb->pushValue(boost::lexical_cast<int>(boost::any_cast<string>(a)));
            return;
        }
        catch (boost::bad_lexical_cast &error) {
            cb->pushValue(0);
            return;
        }
    }

    cb->pushValue(a);

}

void MathInterface::functionFloat(void) {
    boost::any a = cb->popValue();
    if (a.type() == typeid(int32_t)) {
        cb->pushValue((float)boost::any_cast<int32_t>(a));
        return;
    }
    if (a.type() == typeid(string)) {
        try {
            cb->pushValue(boost::lexical_cast<float>(boost::any_cast<string>(a)));
            return;
        }
        catch (boost::bad_lexical_cast &error) {
            cb->pushValue(0.0f);
            return;
        }
    }
    cb->pushValue(a);
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
    cb->pushValue(abs(boost::any_cast<int>(v)));
}

void MathInterface::functionSqrt(void) {
    float v = cb->popValue<float>();
    cb->pushValue(sqrt(v));
}

void MathInterface::functionSin(void) {
	float angle = cb->popValue<float>();
	
	cb->pushValue(float(sin(angle / 180.0 * 3.14159265358979323)));
}

void MathInterface::functionCos(void) {
	float angle = cb->popValue<float>();
	
	cb->pushValue(float(cos(angle / 180.0 * 3.14159265358979323)));
}

void MathInterface::functionTan(void) {
    float angle = cb->popValue<float>();
    cb->pushValue(float(tan(angle / 180.0 * 3.14159265358979323)));
}

void MathInterface::functionASin(void) {
    float v = cb->popValue<float>();
    cb->pushValue((float)(asinf(v)/3.14159265358979323*180.0));
}

void MathInterface::functionACos(void) {
    float v = cb->popValue<float>();
    cb->pushValue((float)(acosf(v)/3.14159265358979323*180.0));
}

void MathInterface::functionATan(void) {
    float v = cb->popValue<float>();
    cb->pushValue((float)(atanf(v)/3.14159265358979323*180.0));
}

void MathInterface::functionGetAngle(void) {
    float y2 = cb->popValue<float>();
    float x2 = cb->popValue<float>();
    float y1 = cb->popValue<float>();
    float x1 = cb->popValue<float>();
    cb->pushValue((float)((3.14159265358979323 - atan2f(y2-y1,x2-x1))/3.14159265358979323*180.0)); //TODO: Check if works
}

void MathInterface::functionLog(void) {
    float v = cb->popValue<float>();
    cb->pushValue(logf(v));
}

void MathInterface::functionLog10(void) {
    float v = cb->popValue<float>();
    cb->pushValue(log10f(v));
}

void MathInterface::functionRnd(void) {
    float high = cb->popValue<float>();
    float low = cb->popValue<float>();
    cb->pushValue(low + ((float)rand() / RAND_MAX)*(high-low)); //TODO: Better generation (Windows RAND_MAX == 0x7fff)
}

void MathInterface::functionRand(void) {
    int32_t high = cb->popValue<int32_t>();
    int32_t low = cb->popValue<int32_t>();
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
    float smoothness = cb->popValue<float>();
    float oldV = cb->popValue<float>();
    float newV = cb->popValue<float>();
    cb->pushValue(oldV + (newV-oldV)/smoothness);
}

void MathInterface::functionCurveAngle(void) {
	
}

void MathInterface::functionWrapAngle(void) {
	float angle = cb->popValue<float>();
	while (angle > 360.0) {
		angle -= 360.0;
	}
	while (angle < 0.0) {
		angle += 360.0;
	}

	cb->pushValue(angle);
}


//Helper
inline double square(float num)
{
    return (double)num*(double)num;
}

void MathInterface::functionDistance(void) {
    float y2 = cb->popValue<float>();
    float x2 = cb->popValue<float>();
    float y1 = cb->popValue<float>();
    float x1 = cb->popValue<float>();
    cb->pushValue((float)sqrt(square(x2-x1)+square(y2-y1)));
}

void MathInterface::functionBoxOverlap(void) {
    float height2h = cb->popValue<float>()*0.5f;
    float width2h = cb->popValue<float>()*0.5f;
    float y2 = cb->popValue<float>();
    float x2 = cb->popValue<float>();
    float height1h = cb->popValue<float>()*0.5f;
    float width1h = cb->popValue<float>()*0.5f;
    float y1 = cb->popValue<float>();
    float x1 = cb->popValue<float>();

    cb->pushValue((abs(y1-y2) < (height1h+height2h)) && ((abs(x1-x2) < (width1h+width2h))) ? 1:0);


}
