
#include "kaguya/kaguya.hpp"
#include "util/file.h"

namespace newobj
{
	namespace lua
	{
		
		namespace module
		{
			class system
			{
			public:
				system();
				~system();
			};
			void regist_system(void *config,kaguya::State* L);
		}

	}
}