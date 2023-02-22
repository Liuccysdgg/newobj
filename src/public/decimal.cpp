#include "decimal.h"
#include <math.h>
#ifndef MSVC_2010
decimal::decimal(double val)
{
    m_int64 = (uint64)(1e6 * val);
}
decimal::~decimal()
{

}
decimal& decimal::operator=(double val)
{
    m_int64 = (uint64)(1e6 * val);
    return *this;
}
decimal& decimal::operator+(double val)
{
    m_int64 += (uint64)(1e6 * val);
    return *this;
}
decimal& decimal::operator-(double val)
{
    m_int64 -= (uint64)(1e6 * val);
    return *this;
}
decimal& decimal::operator*(double val)
{
    uint64 temp = (uint64)(1e6 * val);
    m_int64 *= (uint64)(temp / 1e6);
    return *this;
}
decimal& decimal::operator/(double val)
{
    uint64 temp = (uint64)(1e6 * val);
    m_int64 /= (uint64)(temp / 1e6);
    return *this;
}
decimal& decimal::operator+=(double val)
{
    m_int64 += (uint64)(1e6 * val);
    return *this;
}
decimal& decimal::operator-=(double val)
{
    m_int64 -= (uint64)(1e6 * val);
    return *this;
}
decimal& decimal::operator*=(double val)
{
    uint64 temp = (uint64)(1e6 * val);
    m_int64 *= (uint64)(temp / 1e6);
    return *this;
}
decimal& decimal::operator/=(double val)
{
    uint64 temp = (uint64)(1e6 * val);
    m_int64 /= (uint64)(temp / 1e6);
    return *this;
}
double decimal::to(uint32 len)
{
    if (m_int64 == 0)
        return 0;
    uint64 temp = 0;
    double temp_d = 0;
    if (len == 0)
    {
        temp = (uint64)(m_int64 / 1e6);
        //temp *= 1e6;
        temp_d = (double)temp;
    }
    else
    {
        temp = m_int64 / (uint64)(1e6 * pow(10, len));
        temp_d = temp / pow(10, len);
    }
    return temp_d;
}
double decimal::to()
{
    return m_int64 / 1e6;
}

decimal::decimal()
{
    m_int64 = 0;
}

bool decimal::operator==(const decimal& val)
{
    return this->m_int64 == val.m_int64;
}

bool decimal::operator!=(const decimal& val)
{
    return this->m_int64 != val.m_int64;
}
bool decimal::operator==(const double& val)
{
    uint64 temp = (uint64)(val * 1e6);
    return this->m_int64 == temp;
}

bool decimal::operator!=(const double& val)
{
    uint64 temp = (uint64)(val * 1e6);
    return this->m_int64 != temp;
}
#endif