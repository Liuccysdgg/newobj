#ifndef MSVC_2010
#include "noui_core.h"
#include "miniblink/miniblink.h"
#ifdef LIB_QT
newobj::noui::core::core(newobj::browser::miniblink* miniblink)
{
	m_miniblink = miniblink;
}

newobj::noui::core::~core()
{
}

newobj::browser::miniblink* newobj::noui::core::web()
{
	return m_miniblink;
}
#endif
#endif
