
#include "kaguya/kaguya.hpp"
#include "util/file.h"
#include "util/json.h"
namespace newobj
{
	namespace lua
	{
		namespace module
		{
			class json
			{
			public:
				json();
				~json();
				bool parse_file(const std::string& filepath);
				newobj::lua::module::json* at(int index);
				int at_int(int index);
				std::string at_string(int index);
				double at_double(int index);
				bool at_bool(int index);

				std::string to_string();
				std::string get_string(const std::string& name);
				int get_int(const std::string& name);
				double get_double(const std::string& name);
				bool get_bool(const std::string& name);
				newobj::lua::module::json* get_obj(const std::string& name);
				void set_path(const std::string path);
				bool parse(const std::string& value);
				bool save(const std::string& save);
				int size();
				void resize(int size);
				bool exist(const std::string& name);
				void erase_by_name(const std::string& name);
				void erase_by_index(int index);
				void push_int(int value);
				void push_string(const std::string& value);
				void push_double(double value);
				void push_bool(bool value);

				void set_int(const std::string&name,int value);
				void set_string(const std::string& name, const std::string& value);
				void set_double(const std::string& name, double value);
				void set_bool(const std::string& name, bool value);
			private:
				newobj::json m_json;
			};
			void regist_json(void* config, kaguya::State* L);
		}
	}
}