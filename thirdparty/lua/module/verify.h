
#include "kaguya/kaguya.hpp"
#include "util/file.h"

namespace newobj
{
	namespace lua
	{
		
		namespace module
		{
			class verify
			{
			public:
				verify();
				~verify();
			};
			void regist_verify(void *config,kaguya::State* L);
		}

	}
}