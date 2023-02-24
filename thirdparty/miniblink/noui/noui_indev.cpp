#ifndef MSVC_2010
#include "noui_indev.h"
#include "miniblink/miniblink.h"

#if LIB_QT == 1
newobj::noui::indev::indev(newobj::browser::miniblink* miniblink)
	:noui::core(miniblink)
{
	// x
	BIND_FUNCTION("noui_indev_cursor_x",{
		return JS_INT(((newobj::noui::indev*)param)->cursor_x());
    },0);
	// y
	BIND_FUNCTION("noui_indev_cursor_y", {
		return JS_INT(((newobj::noui::indev*)param)->cursor_y());
	},0);
}

newobj::noui::indev::~indev()
{
}
uint32 newobj::noui::indev::cursor_x()
{
	return QCursor::pos().x();
}

uint32 newobj::noui::indev::cursor_y()
{
	return QCursor::pos().y();
}
#endif
#endif
