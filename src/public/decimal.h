#pragma once
#include "define.h"
#ifndef MSVC_2010
/*小数封装类*/
class decimal
{
public:
	decimal();
	decimal(double val);
	~decimal();
	decimal& operator=(double val);
	decimal& operator+(double val);
	decimal& operator-(double val);
	decimal& operator*(double val);
	decimal& operator/(double val);
	decimal& operator+=(double val);
	decimal& operator-=(double val);
	decimal& operator*=(double val);
    decimal& operator/=(double val);
	bool operator==(const decimal& val);
	bool operator!=(const decimal& val);
	bool operator==(const double& val);
	bool operator!=(const double& val);
    double to(uint32 len);
    double to();
private:
    uint64 m_int64;

};
#endif