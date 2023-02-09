#include "math.h"

#include <math.h>

float newobj::math::to_fixed(float num, uint32 x)
{
   
    uint32 m = (uint32)(num * pow((double)10, (double)x));
    float p = (float)m;
    return (float)(p / pow((double)10, (double)x));
}
double newobj::math::to_fixed(double num, uint32 x)
{
    uint64 m = (uint64)(num * pow((double)10, (double)x));
    double p = (double)m;
    return p / pow((double)10, (double)x);
}

uint32 newobj::math::decimal_last_size(float num)
{
    /* 
    
    num = 5.32
    xs = 0.32
    zs = 5


    num = 10
    



    


     */
    float xs = num - (int)num;
    for (int i = 0; i < 10; i++) {
        uint32 c = (uint32)pow((double)10, (double)(i + 1));
        float p = xs * c;
        if (p - (int)p == 0)
            return i+1;
    }
    return 0;
}

double newobj::math::round(double number, int bits)
{
    for (int i = 0; i < bits; ++i)
    {
        number *= 10;
    }
    number = (double)(number + 0.5);
    for (int i = 0; i < bits; ++i)
    {
        number /= 10;
    }
    return number;
}

int32 newobj::math::to_int(double value)
{
    return (int32)value;
}
