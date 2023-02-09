#pragma once

#include "util/json.h"

newobj::json script_exec(const nstring& script,const nstring& function,const newobj::json& param,nstring& error_msg);