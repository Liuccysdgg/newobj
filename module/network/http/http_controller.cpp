#include "http_controller.h"
#if USE_NET_HTTP_WEBSITE
#include "public/exception.h"
#include "util/verify.h"
newobj::network::http::controller::controller()
{
	m_reqpack = nullptr;
}
newobj::network::http::controller::~controller()
{

}

nstring newobj::network::http::controller::qry_string(const nstring& name, uint32 pmin, uint32 pmax)
{
	
	nstring value;
	bool result = request_param(name, value);

	if (!result)
	{
		trw_str("The request parameter '"+name+"' was not found");
	}
	if (value.length() < pmin)
	{
		trw_str("The format of parameter '" + name + "'  does not meet the conditions, it should be >"+nstring::from(pmin));
	}
	if (value.length() > pmax)
	{
		trw_str("The format of parameter  '" + name + "'  does not meet the conditions, it should be <" + nstring::from(pmax));
	}
	return value;
}
int32  newobj::network::http::controller::qry_int32(const nstring& name, int32 pmin, int32 pmax)
{
	nstring _param_value_;
	bool result = request_param(name, _param_value_);
	int32 value = _param_value_.to_int32();
	if (!result)
	{
		trw_str("The request parameter '" + name + "' was not found");
	}
	if (!verify::is_nums(_param_value_))
	{
		trw_str("The request parameter '" + name + "' not int32 type");
	}
	if (value < pmin)
	{
		trw_str("The format of parameter '" + name + "'  does not meet the conditions, it should be >" + nstring::from(pmin));
	}
	if (value > pmax)
	{
		trw_str("The format of parameter  '" + name + "'  does not meet the conditions, it should be <" + nstring::from(pmax));
	}
	return value;
}
uint32  newobj::network::http::controller::qry_uint32(const nstring& name, uint32 pmin, uint32 pmax)
{
	nstring _param_value_;
	bool result = request_param(name, _param_value_);
	uint32 value = _param_value_.to_uint32();
	if (!result)
	{
		trw_str("The request parameter '" + name + "' was not found");
	}
	if (!verify::is_nums(_param_value_))
	{
		trw_str("The request parameter '" + name + "' not uint32 type");
	}
	if (value < pmin)
	{
		trw_str("The format of parameter '" + name + "'  does not meet the conditions, it should be >" + nstring::from(pmin));
	}
	if (value > pmax)
	{
		trw_str("The format of parameter  '" + name + "'  does not meet the conditions, it should be <" + nstring::from(pmax));
	}
	return value;
}
int64  newobj::network::http::controller::qry_int64(const nstring& name, int64 pmin, int64 pmax)
{
	nstring _param_value_;
	bool result = request_param(name, _param_value_);
	int64 value = _param_value_.to_int64();
	if (!result)
	{
		trw_str("The request parameter '" + name + "' was not found");
	}
	if (!verify::is_nums(_param_value_))
	{
		trw_str("The request parameter '" + name + "' not int64 type");
	}
	if (value < pmin)
	{
		trw_str("The format of parameter '" + name + "'  does not meet the conditions, it should be >" + nstring::from(pmin));
	}
	if (value > pmax)
	{
		trw_str("The format of parameter  '" + name + "'  does not meet the conditions, it should be <" + nstring::from(pmax));
	}
	return value;
}
uint64  newobj::network::http::controller::qry_uint64(const nstring& name, uint64 pmin, uint64 pmax)
{
	nstring _param_value_;
	bool result = request_param(name, _param_value_);
	uint64 value = _param_value_.to_uint64();
	if (!result)
	{
		trw_str("The request parameter '" + name + "' was not found");
	}
	if (!verify::is_nums(_param_value_))
	{
		trw_str("The request parameter '" + name + "' not uint64 type");
	}
	if (value < pmin)
	{
		trw_str("The format of parameter '" + name + "'  does not meet the conditions, it should be >" + nstring::from(pmin));
	}
	if (value > pmax)
	{
		trw_str("The format of parameter  '" + name + "'  does not meet the conditions, it should be <" + nstring::from(pmax));
	}
	return value;
}
double  newobj::network::http::controller::qry_double(const nstring& name,double pmin, double pmax)
{
	nstring _param_value_;
	bool result = request_param(name, _param_value_);
	double value = _param_value_.to_double();
	if (!result)
	{
		trw_str("The request parameter '" + name + "' was not found");
	}
	if (value < pmin)
	{
		trw_str("The format of parameter '" + name + "'  does not meet the conditions, it should be >" + nstring::from(pmin));
	}
	if (value > pmax)
	{
		trw_str("The format of parameter  '" + name + "'  does not meet the conditions, it should be <" + nstring::from(pmax));
	}
	return value;
}
float  newobj::network::http::controller::qry_float(const nstring& name, float pmin, float pmax)
{
	nstring _param_value_;
	bool result = request_param(name, _param_value_);
	float value = _param_value_.to_float();
	if (!result)
	{
		trw_str("The request parameter '" + name + "' was not found");
	}
	if (value < pmin)
	{
		trw_str("The format of parameter '" + name + "'  does not meet the conditions, it should be >" + nstring::from(pmin));
	}
	if (value > pmax)
	{
		trw_str("The format of parameter  '" + name + "'  does not meet the conditions, it should be <" + nstring::from(pmax));
	}
	return value;
}
bool  newobj::network::http::controller::qry_empty(const nstring& name)
{
	if (request()->parser()->url_param_exist(name) == false && request()->parser()->body_param_exist(name) == false)
		return false;
	return true;
}
bool newobj::network::http::controller::qry_bool(const nstring& name)
{
	nstring _param_value_;
	bool result = request_param(name, _param_value_);
	if (!result)
	{
		trw_str("The request parameter '" + name + "' was not found");
	}
	return _param_value_ == "true";
}
buffer newobj::network::http::controller::qry_buffer(const nstring& name)
{
	network::http::form_info form_info;
	if (request()->parser()->form()->get(name, form_info) == false)
	{
		trw_str("The request parameter '" + name + " was not found");
	}

	if (form_info.start != -1 && form_info.length != -1 && form_info.length != 0)
	{
		return m_reqpack->data()->sub(form_info.start,form_info.length);
	}
	else
	{
		trw_str("failed to parse buffer, start="+nstring::from(form_info.start) + ", length="+nstring::from(form_info.length));
	}
}
bool newobj::network::http::controller::request_param(const nstring& name, nstring& value)
{
	if (request()->parser()->url_param(name, value) == false)
		return request()->parser()->body_param(name, value);
	return true;
}
network::http::request* newobj::network::http::controller::request()
{
	return m_reqpack->request();
}
network::http::response* newobj::network::http::controller::response()
{
	return m_reqpack->response();
}
#endif