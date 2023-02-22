
#include "kaguya/kaguya.hpp"
#include "util/file.h"

namespace newobj
{
	namespace lua
	{
		
		namespace module
		{
			class forstring
			{
			public:
				forstring();
				~forstring();
			};
			void regist_forstring(void *config,kaguya::State* L);
		}

	}
}