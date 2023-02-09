#ifndef MSVC_2010
#include "noui_file.h"
#include "miniblink/miniblink.h"
#include "util/file.h"
#include "util/codec.h"
#ifdef LIB_QT
newobj::noui::file::file(newobj::browser::miniblink* miniblink)
	:noui::core(miniblink)
{
	// write
	BIND_FUNCTION("noui_file_writeex", {
		return JS_BOOL(((newobj::noui::file*)param)->writeex(
			ARG_STRING(0),
			codec::base64::de(ARG_STRING(1))
		));
    },2);
	// read
	BIND_FUNCTION("noui_file_readex", {
		newobj::buffer data;
		((newobj::noui::file*)param)->readex(ARG_STRING(0), data);
		nstring result = codec::base64::en(data);
		return JS_STRING(result.c_str());
	},1);
	// obj create
	BIND_FUNCTION("noui_file_obj_create", {
		return JS_STRING(((newobj::noui::file*)param)->create().c_str());
	}, 0);
	// obj destory
	BIND_FUNCTION("noui_file_obj_destory", {
		((newobj::noui::file*)param)->destory(ARG_STRING(0));
		return JS_UNDEFINED;
	}, 1);
	// obj open
	BIND_FUNCTION("noui_file_obj_open", {
		return JS_BOOL(((newobj::noui::file*)param)->open(ARG_STRING(0),ARG_STRING(1)));
	}, 2);
	// obj close
	BIND_FUNCTION("noui_file_obj_close", {
		return JS_BOOL(((newobj::noui::file*)param)->close(ARG_STRING(0)));
	}, 1);
	// obj size
	//BIND_FUNCTION("noui_file_obj_size", {
	//	return JS_INT(((newobj::noui::file*)param)->size(ARG_STRING(0)));
	//}, 1);
	// obj read
	BIND_FUNCTION("noui_file_obj_read", {
		newobj::buffer data;
		((newobj::noui::file*)param)->read(
			ARG_STRING(0),
			ARG_INT(1),
			ARG_INT(2),
			data
		);
		return JS_STRING(codec::base64::en(data).c_str());
	}, 3);
	// obj write
	BIND_FUNCTION("noui_file_obj_write", {
		return JS_BOOL(((newobj::noui::file*)param)->write(ARG_STRING(0),codec::base64::de(ARG_STRING(1))));
	}, 2);
	// obj jump
	BIND_FUNCTION("noui_file_obj_jump", {
		return JS_BOOL(((newobj::noui::file*)param)->jump(ARG_STRING(0),ARG_INT(1)));
	}, 2);
	// obj clear
	BIND_FUNCTION("noui_file_obj_clear", {
		return JS_BOOL(((newobj::noui::file*)param)->clear(ARG_STRING(0)));
	}, 1);
	// obj flush
	BIND_FUNCTION("noui_file_obj_flush", {
		return JS_BOOL(((newobj::noui::file*)param)->flush(ARG_STRING(0)));
	}, 1);
}

newobj::noui::file::~file()
{
}
bool newobj::noui::file::writeex(const nstring& path, const newobj::buffer& value)
{
	return newobj::file::write(codec::to_gbk(path),value);
}
bool newobj::noui::file::readex(const nstring& path,newobj::buffer& data)
{
	return newobj::file::read(codec::to_gbk(path), data);
}
nstring newobj::noui::file::create()
{
	return nstring::from((ptr)new newobj::file);
}
void newobj::noui::file::destory(const nstring& obj)
{
	delete (newobj::file*)obj.to_uint64();
}
bool newobj::noui::file::open(const nstring& obj, const nstring& path)
{
	auto f = (newobj::file*)obj.to_uint64();
	return f->open(codec::to_gbk(path));
}
bool newobj::noui::file::close(const nstring& obj)
{
	auto f = (newobj::file*)obj.to_uint64();
	return f->close();
}
bool newobj::noui::file::read(const nstring& obj, uint32 idx, uint32 length, newobj::buffer& data)
{
	auto f = (newobj::file*)obj.to_uint64();
	if (length == 0)
		return false;
	data.setsize(length);
	return f->read(idx,length,(char*)data.data());
}

bool newobj::noui::file::write(const nstring& obj, const newobj::buffer& data)
{
	auto f = (newobj::file*)obj.to_uint64();
	return f->write(data);
}

bool newobj::noui::file::jump(const nstring& obj, uint32 idx)
{
	auto f = (newobj::file*)obj.to_uint64();
	return f->jump(idx);
}

bool newobj::noui::file::clear(const nstring& obj)
{
	auto f = (newobj::file*)obj.to_uint64();
	return f->clear();
}

bool newobj::noui::file::flush(const nstring& obj)
{
	auto f = (newobj::file*)obj.to_uint64();
	return f->flush();
}
#endif
#endif
