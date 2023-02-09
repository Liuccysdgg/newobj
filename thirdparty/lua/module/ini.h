
#include "kaguya/kaguya.hpp"
#include "util/file.h"

namespace newobj
{
	namespace lua
	{
		
		namespace module
		{
			class ini
			{
			public:
				ini();
				~ini();
			};
			void regist_ini(void *config,kaguya::State* L);
		}

	}
}