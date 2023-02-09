#include "public/define.h"
#include <string>
#include "public/string.h"
#define CURR_LOCATION  nstring(__FILE__) + " " + nstring(__FUNCTION__) + " : " + nstring::from((int32)__LINE__)
