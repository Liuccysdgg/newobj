
#include "kaguya/kaguya.hpp"
#include "util/file.h"

namespace newobj
{
	namespace lua
	{
		
		namespace module
		{
			class algorithm
			{
			public:
				algorithm();
				~algorithm();
			};
			void regist_algorithm(void *config,kaguya::State* L);
		}

	}
}