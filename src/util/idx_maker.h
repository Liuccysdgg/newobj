#pragma once
#include <string>
#include "public/define.h"
#include "util/lock.h"
namespace newobj
{
        /****************************************************
         * Class������������
         ****************************************************/
        class NEWOBJ_API idx_maker
        {
        public:
            idx_maker(uint32 index = 0);
            ~idx_maker();
            uint32 create();
        private:
            uint32 m_index;
            newobj::mutex m_mutex;
        };
}
