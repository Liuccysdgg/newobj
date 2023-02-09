
#include "kaguya/kaguya.hpp"
#include "util/file.h"
#include <vector>
namespace newobj
{
	namespace lua
	{
		namespace module
		{
			class vector_string
			{
			public:
				vector_string();
				~vector_string();
				void push_back(const std::string& value);
				std::string at(int index);
				void clear();
				uint32 size();
			private:
				std::vector<std::string> m_value;
			};
			void regist_vector_string(void *config,kaguya::State* L);
		}

	}
}