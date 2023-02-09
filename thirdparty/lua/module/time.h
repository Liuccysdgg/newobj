
#include "kaguya/kaguya.hpp"
#include "util/file.h"

namespace newobj
{
	namespace lua
	{
		
		namespace module
		{
			class time
			{
			public:
				time();
				~time();
			};
			void regist_time(void *config,kaguya::State* L);
		}

	}
}