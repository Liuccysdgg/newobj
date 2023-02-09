#pragma once
#include "public/define.h"
#include "public/string.h"
#include <vector>

namespace newobj
{
    namespace config
    {
        namespace ini
        {
            /*∂¡INI≈‰÷√*/
            bool NEWOBJ_API read(const nstring& filepath,const nstring& node,const nstring& name,nstring& value, const nstring& default_value = "");
            nstring NEWOBJ_API read(const nstring& filepath, const nstring& node, const nstring& name,const nstring& default_value = "");
            /*–¥INI≈‰÷√*/
            bool NEWOBJ_API write(const nstring& filepath, const nstring& node, const nstring& name,const nstring& value);
        }
    }
}