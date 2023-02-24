#include "util/config.h"
#include "util/file.h"
#if LIB_QT == 1
#include "QtCore/qsettings.h"
#endif
#ifdef WIN32
#include <Windows.h>
#endif
bool newobj::config::ini::read(const nstring& filepath, const nstring& node, const nstring& name, nstring& value, const nstring& default_value)
{
#ifdef WIN32
	char result[1024 * 10];
	auto resultLength = GetPrivateProfileStringA(node.c_str(), name.c_str(), "", result, 1024 * 10, filepath.c_str());
	if (resultLength > 1024 * 10)
	{
		value = default_value;
		return false;
	}
	if (resultLength == 0)
		value = default_value;
	else
		value.append(result, resultLength);
	return true;
#else
#if LIB_QT == 1
    QSettings setting(newobj::file::format_slash(filepath),QSettings::IniFormat);
    QString key = QString(node.c_str()) + "/" + QString(name.c_str());
    value = setting.value(key).toString();
    return true;
#else
	return false;
#endif
#endif
}

nstring newobj::config::ini::read(const nstring& filepath, const nstring& node, const nstring& name, const nstring& default_value)
{
	nstring result;
    read(newobj::file::format_slash(filepath), node, name, result, default_value);
	return result;
}

bool newobj::config::ini::write(const nstring& filepath, const nstring& node, const nstring& name, const nstring& value)
{
#ifdef WIN32
	return WritePrivateProfileStringA(node.c_str(), name.c_str(), value.c_str(), filepath.c_str());
#else
#if LIB_QT == 1
    QSettings setting(newobj::file::format_slash(filepath),QSettings::IniFormat);
    setting.beginGroup(node.c_str());
    setting.setValue(name.c_str(),value.c_str());
    setting.endGroup();
	return true;
#else
	return false;
#endif
#endif
}
