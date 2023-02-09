#include "cjy.h"
#include "util/time.h"
#include "util/codec.h"
#include "util/file.h"
newobj::image::recognition::cjy::cjy()
{
}

newobj::image::recognition::cjy::~cjy()
{
}

void newobj::image::recognition::cjy::set_user(const nstring& username, const nstring& password)
{
	this->m_username = username;
	this->m_password = password;
}

void newobj::image::recognition::cjy::set_domain(const nstring& domain)
{
	this->m_domain = domain;
}

bool newobj::image::recognition::cjy::score(uint32& value)
{

	std::map<nstring, nstring> param;
	param["user"] = m_username;
	param["pass"] = m_password;
	//m_client.post(m_domain + "/Upload/GetScore.php", param);
	//auto response = newobj::json::from(m_client.response().to_string());
	return true;
}
nstring newobj::image::recognition::cjy::regist(const nstring& domain, const nstring& username, const nstring& password)
{
	//network::http::client client;
	//std::map<nstring, nstring> param;
	//param["user"] = username;
	//param["pass"] = password;
	//client.post(domain + "/Upload/UserReg.php",param);
	//return client.response().to_string();
	return "";
}

bool newobj::image::recognition::cjy::detect(const nstring& domain, const nstring& username, const nstring& password_md5, uint32 softid, nstring codetype, const newobj::buffer& image, nstring& result, nstring& error_msg)
{
	//network::http::client client;
	//client.set_timeout(10000);
	//network::http::make_form form;
	//form.add("user",username);
	//form.add("pass2", password_md5);
	//form.add("softid", nstring::from(softid));
	//form.add("codetype", codetype);
	//form.add("len_min", "0");
	//form.add("time_add", "0");
	//form.add("str_debug", "");
	//form.add("userfile", "cc.jpg","image", image);

	//
	//client.header("Cache-control", "no-cache");
	//client.header("Connection","Keep-Alive");
	//
	//
	//
	//newobj::buffer reqData;
	//nstring boundary;
	//form.make(reqData,boundary);
	//client.header("Content-Type", "multipart/form-data; boundary="+boundary);
	//client.header("Accept", "*/*");
	//client.header("User-Agent", "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0");
 //   client.header("Content-Length", nstring::from((uint64)reqData.length()));
	//client.header("Host", domain);
	//client.post(domain+"/Upload/Processing.php",reqData);

	//auto response = newobj::json::from(client.response().to_string());
	//if (response["err_no"].to<uint32>() != 0)
	//{
	//	error_msg = response["err_str"].to<nstring>();
	//	return false;
	//}
	//result = response["pic_str"].to<nstring>();
	return true;
}
