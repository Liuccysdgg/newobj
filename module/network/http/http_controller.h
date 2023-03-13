#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include "http_request.h"
#include "http_response.h"
#include "http_reqpack.h"
#include "http_interface.h"
#define _DBL_MIN          2.2250738585072014e-308 // min positive value
#define _DBL_MAX          1.7976931348623158e+308 // max value
#define _INT_MIN     (-2147483647 - 1)
#define _INT_MAX       2147483647
#define _UINT_MAX      0xffffffff
#define _LONG_MIN    (-2147483647L - 1)
#define _LONG_MAX      2147483647L
#define _ULONG_MAX     0xffffffff
#ifndef _LLONG_MAX
#define _LLONG_MAX     9223372036854775807
#endif
#define _LLONG_MIN   (-9223372036854775807 - 1)
#define _ULLONG_MAX    0xffffffffffffffff
#define _INT8_MIN         (-127 - 1)
#define _INT16_MIN        (-32767 - 1)
#define _INT32_MIN        (-2147483647 - 1)
#define _INT64_MIN        (-9223372036854775807 - 1)
#define _INT8_MAX         127
#define _INT16_MAX        32767
#define _INT32_MAX        2147483647
#define _INT64_MAX        9223372036854775807
#define _UINT8_MAX        0xff
#define _UINT16_MAX       0xffff
#define _UINT32_MAX       0xffffffff
#define _UINT64_MAX       0xffffffffffffffff
#define _FLT_MIN          1.175494351e-38F        // min normalized positive value
#define _FLT_MAX          3.402823466e+38F        // max value
using namespace newobj::network::http;

// 成功回复
#define REPLY_SUCC rpjson()["code"] = 200;return RT_OK
// 自定义回复
#define REPLY(CODE,MSG) rpjson()["code"] = CODE;rpjson()["msg"]=MSG;return RT_OK
// 失败回复
#define REPLY_ERROR(MSG) rpjson()["code"] = -1;rpjson()["msg"]=MSG;return RT_OK
/**********************************************************
 * Class：Http控制器接口
 *********************************************************/
#define qry_json_string(NAME) request()->parser()->json()[NAME].to<nstring>()
#define qry_json_uint32(NAME) request()->parser()->json()[NAME].to<uint32>()
#define qry_json_int32(NAME) request()->parser()->json()[NAME].to<int32>()
#define qry_json_double(NAME) request()->parser()->json()[NAME].to<double>()
#define qry_json_short(NAME) request()->parser()->json()[NAME].to<short>()
#define qry_json_bool(NAME) request()->parser()->json()[NAME].to<bool>()
namespace newobj
{
	namespace network
	{
		namespace http
		{ 
			class router;
			class MNETWORK_API controller:public network::http::interface_
			{
			public:
				controller();
				~controller();
				network::http::request* request();
				network::http::response* response();


				/*min和max为<=或>= ,若填-1则不使用该条件*/
				nstring qry_string(const nstring& name, uint32 pmin = 0, uint32 pmax = _UINT_MAX);
				int32 qry_int32(const nstring& name, int32 pmin = _INT_MIN, int32 pmax = _INT_MAX);
				uint32 qry_uint32(const nstring& name, uint32 pmin = 0, uint32 pmax = _UINT_MAX);
				int64 qry_int64(const nstring& name, int64 pmin = _INT64_MIN, int64 pmax = _INT64_MAX);
				uint64 qry_uint64(const nstring& name, uint64 pmin = 0, uint64 pmax = _UINT64_MAX);
				double qry_double(const nstring& name, double pmin = _DBL_MIN, double pmax = _DBL_MAX);
				float qry_float(const nstring& name, float pmin = _FLT_MIN, float pmax = _FLT_MAX);
				bool qry_empty(const nstring& name);
				bool qry_bool(const nstring& name);
				stream_view qry_buffer(const nstring& name);

				// 请求参数
				bool request_param(const nstring& name,nstring& value);

                // 获取回复JSON
                inline newobj::json& rpjson(){return response()->sjson["data"];}

                
				friend class router;
			private:
				network::http::reqpack* m_reqpack;
			};
		}
	}
}
#endif
