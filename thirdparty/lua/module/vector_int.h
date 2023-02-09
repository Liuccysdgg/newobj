
#include "kaguya/kaguya.hpp"
#include "util/file.h"
#include <vector>
namespace newobj
{
	namespace lua
	{
		namespace module
		{
			class vector_int
			{
			public:
				vector_int();
				~vector_int();
				void push_back(int value);
				int at(int index);
				void clear();
				void sort(bool ps/*ÕýÐò*/);
				uint32 size();
			private:
				std::vector<int> m_value;
			};
			void regist_vector(void *config,kaguya::State* L);
		}

	}
}