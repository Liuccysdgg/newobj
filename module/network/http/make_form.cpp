#include "make_form.h"
#if USE_NET_HTTP_UTIL
#include "util/time.h"
#include "util/system.h"
#include "util/file.h"
newobj::network::http::make_form::make_form()
{
}

newobj::network::http::make_form::~make_form()
{
	for_iter(iter,m_list)
		delete (*iter);
}

bool newobj::network::http::make_form::add(const nstring& name, const nstring& value)
{
    t_ret_f(m_list.find([=](form_info* param, uint32 idx)->bool {
		return param->name == name;
	}));
    form_info* param = new form_info;
	param->name = name;
    param->disposition = "form-data";
	param->data = value;
	m_list.push_back(param);
	return true;
}

bool newobj::network::http::make_form::add(const nstring& name, const nstring& filename, const nstring& content_type,const newobj::buffer& data)
{
    t_ret_f(m_list.find([=](form_info* param, uint32 idx)->bool {
		return param->name == name;
	}));
    form_info* param = new form_info;
	param->name = name;
    param->disposition = "form-data";
	param->data = data;
    param->content_type = content_type;
	param->filename = filename;
	m_list.push_back(param);
	return true;
}

bool newobj::network::http::make_form::make(newobj::buffer& data, nstring& boundary) const
{
	for (uint32 i = 0; i < 27; i++)
	{
		char num = system::random(48, 57, i);
		boundary.append(&num,1);
	}
		

	boundary = "--------" + boundary;

	uint32 current_idx = 0;
	data.append(nstring("--" + boundary + "\r\n"));
	for_iter(iter,m_list)
	{
        if ((*iter)->content_type != "")
		{
			data.append(nstring("Content-Disposition: form-data; name=\""+ (*iter)->name+"\"; filename=\""+ (*iter)->filename+"\"\r\n"));
            data.append(nstring("Content-Type: ")+ (*iter)->content_type +nstring("\r\n\r\n"));
		}
		else
			data.append(nstring("Content-Disposition: form-data; name=\"" + (*iter)->name + "\"\r\n\r\n"));
		data.append((*iter)->data);
		data.append(nstring("\r\n"));
		if (current_idx + 1 == m_list.size())
			data.append(nstring("--"+ boundary +"--\r\n"));
		else
			data.append(nstring("--" + boundary + "\r\n"));
		current_idx++;
	}
	return true;
}
#endif