//#include "util/util_obj.h"
//#include "public/exception.h"
//#include "util/util_str.h"
//#define PUSH_OBJ if (this->m_array == nullptr){this->m_array = new std::vector<obj*>;this->m_type = NVT_Array;}this->m_array->push_back(new obj(val));
//newobj::obj::obj()
//{
//	this->m_type = NVT_Null;
//}
//
//newobj::obj::obj(const nstring& val)
//{
//	*this = val;
//}
//
//newobj::obj::obj(const int32& val)
//{
//	*this = val;
//}
//
//newobj::obj::obj(const int64& val)
//{
//	*this = val;
//}
//
//newobj::obj::obj(const uint32& val)
//{
//	*this = val;
//}
//
//newobj::obj::obj(const uint64& val)
//{
//	*this = val;
//}
//
//newobj::obj::obj(const double& val)
//{
//	*this = val;
//}
//
//newobj::obj::obj(const bool& val)
//{
//	*this = val;
//}
//
//newobj::obj::obj(const obj& val)
//{
//	*this = val;
//}
//
//newobj::obj::~obj()
//{
//	if (this->m_array != nullptr)
//	{
//		for(size_t i = 0;i< this->m_array->size();i++)
//			delete this->m_array->at(i);
//		delete this->m_array;
//	}
//}
//
//void newobj::obj::operator=(const nstring& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	this->m_type = NVarType::NVT_String;
//	this->m_val_str = val;
//}
//
//void newobj::obj::operator=(const int32& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	this->m_type = NVarType::NVT_Int;
//	this->m_val_gen.v_int32 = val;
//}
//
//void newobj::obj::operator=(const int64& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	this->m_type = NVarType::NVT_LLong;
//	this->m_val_gen.v_int64 = val;
//}
//
//void newobj::obj::operator=(const uint32& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	this->m_type = NVarType::NVT_UInt;
//	this->m_val_gen.v_uint32 = val;
//}
//
//void newobj::obj::operator=(const uint64& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	this->m_type = NVarType::NVT_ULLong;
//	this->m_val_gen.v_uint64 = val;
//}
//
//void newobj::obj::operator=(const double& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	this->m_type = NVarType::NVT_Double;
//	this->m_val_gen.v_double = val;
//}
//
//void newobj::obj::operator=(const bool& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	this->m_type = NVarType::NVT_Boolean;
//	this->m_val_gen.v_bool = val;
//}
//void newobj::obj::operator=(const obj& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	this->m_type = val.m_type;
//	this->m_val_gen = val.m_val_gen;
//	this->m_val_str = val.m_val_str;
//	if (val.m_array != nullptr)
//	{
//		if (this->m_array == nullptr)
//			this->m_array = new std::vector<obj*>;
//		this->m_array->clear();
//		for (size_t i = 0; i < this->m_array->size(); i++)
//			this->m_array->push_back(new obj(*val.m_array->at(i)));
//	}
//}
//newobj::obj& newobj::obj::operator[](const uint32& index)
//{
//	std::shared_lock<std::shared_mutex> lock(this->m_mutex);
//
//	if (this->m_array == nullptr)
//	{
//		trw_str("not array");
//	}
//	if (this->m_array->size() <= index)
//	{
//		trw_str("the subscript is out of bounds,");
//	}
//	return *this->m_array->at(index);
//}
//
//void newobj::obj::push(const nstring& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	PUSH_OBJ;
//}
//
//void newobj::obj::push(const int32& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	PUSH_OBJ;
//}
//
//void newobj::obj::push(const int64& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	PUSH_OBJ;
//}
//
//void newobj::obj::push(const uint32& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	PUSH_OBJ;
//}
//
//void newobj::obj::push(const uint64& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	PUSH_OBJ;
//}
//
//void newobj::obj::push(const double& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	PUSH_OBJ;
//}
//
//void newobj::obj::push(const bool& val)
//{
//	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
//	PUSH_OBJ;
//}
//
//uint32 newobj::obj::size()
//{
//	std::shared_lock<std::shared_mutex> lock(this->m_mutex);
//	if (this->m_array == nullptr)
//	{
//		trw_str("not array");
//	}
//	return this->m_array->size();
//}
//
//const nstring& newobj::obj::toString()
//{
//	if (this->m_type != NVarType::NVT_String)
//	{
//		trw_str("not the expected type");
//	}
//	return this->m_val_str;
//}
//
//const int32& newobj::obj::toInt32()
//{
//	if (this->m_type != NVT_Int)
//	{
//		trw_str("not the expected type");
//	}
//	return this->m_val_gen.v_int32;
//}
//
//const int64& newobj::obj::toInt64()
//{
//	if (this->m_type != NVT_LLong)
//	{
//		trw_str("not the expected type");
//	}
//	return this->m_val_gen.v_int64;
//}
//
//const uint32& newobj::obj::toUInt32()
//{
//	if (this->m_type != NVT_UInt)
//	{
//		trw_str("not the expected type");
//	}
//	return this->m_val_gen.v_uint32;
//}
//
//const uint64& newobj::obj::toUInt64()
//{
//	if (this->m_type != NVT_ULLong)
//	{
//		trw_str("not the expected type");
//	}
//	return this->m_val_gen.v_uint64;
//}
//
//const double& newobj::obj::toDouble()
//{
//	if (this->m_type != NVT_Double)
//	{
//		trw_str("not the expected type");
//	}
//	return this->m_val_gen.v_double;
//}
//
//const bool& newobj::obj::toBoolean()
//{
//	if (this->m_type != NVT_Boolean)
//	{
//		trw_str("not the expected type");
//	}
//	return this->m_val_gen.v_bool;
//}
//
//bool newobj::obj::empty()
//{
//	return this->m_type == NVT_Null;
//}
//
//NVarType newobj::obj::type()
//{
//	return this->m_type;
//}
