#pragma once
#include "util/point.hpp"
#include "public/public.h"

#define SQL_QUERY(VAR) newobj::query VAR(global::pool)
#define SQL_INSERT(VAR) newobj::insert VAR(global::pool)
#define SQL_UPDATE(VAR) newobj::update VAR(global::pool)
/**********************************************************
 * Class£º¿ØÖÆÆ÷½Ó¿Ú
 *********************************************************/
class icontroller
{
public:
	icontroller()
	{

	}

	~icontroller()
	{


	}

};
