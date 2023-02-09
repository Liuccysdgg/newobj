#ifndef MSVC_2010
#include "noui_miniblink.h"
#include "miniblink/miniblink.h"
#ifdef LIB_QT
newobj::noui::miniblink::miniblink()
	:noui::core(nullptr)
{
}

newobj::noui::miniblink::~miniblink()
{
} 
newobj::browser::miniblink* newobj::noui::miniblink::create(uint32 x,uint32 y,uint32 height,uint32 width, QWidget* parent,bool dialog)
{

	newobj::Geometry geo;
	geo.x = x;
	geo.y = y;
	geo.height = height;
	geo.width = width;
	auto miniblink = new newobj::browser::miniblink(geo,dialog);
	miniblink->qwidget()->setParent(parent);
	// ȡ����ָ��
	BIND_FUNCTION2(miniblink, "noui_window_handle", {
		return JS_STRING(nstring::from((ptr)param).c_str());
	}, 0);
	// ����
	BIND_FUNCTION2(miniblink, "noui_window_create", {
		QWidget * parent = nullptr;
		if(nstring(ARG_STRING(4)).to_uint64() != 0)
			parent = ((browser::miniblink*)nstring(ARG_STRING(4)).to_uint64())->qwidget();
		
		nstring result = nstring::from((uint64)newobj::noui::miniblink::create(
			ARG_INT(0),
			ARG_INT(1),
			ARG_INT(2),
			ARG_INT(3),
			parent,
			ARG_INT(5)==1
		));
		return JS_STRING(result.c_str());
	}, 5);
	// ����
	BIND_FUNCTION2(miniblink,"noui_window_destory", {
		newobj::noui::miniblink::destory((newobj::browser::miniblink*)nstring(ARG_STRING(0)).to_uint64());
		return JS_UNDEFINED;
	}, 1);
	// EXEC
	BIND_FUNCTION2(miniblink, "noui_window_exec", {
		 ((newobj::browser::miniblink*)param)->exec();
		return JS_UNDEFINED;
	}, 0);
	// ����
	BIND_FUNCTION2(miniblink, "noui_window_load", {
		((newobj::browser::miniblink*)nstring(ARG_STRING(0)).to_uint64())->load(ARG_STRING(1));
		return JS_UNDEFINED;
	}, 1);
	// �߶�
	BIND_FUNCTION2(miniblink, "noui_window_height", {
		return JS_INT(((newobj::browser::miniblink*)param)->qwidget()->height());
	}, 0);
	// ���
	BIND_FUNCTION2(miniblink, "noui_window_width", {
		return JS_INT(((newobj::browser::miniblink*)param)->qwidget()->width());
		}, 0);
	// x
	BIND_FUNCTION2(miniblink, "noui_window_x", {
		return JS_INT(((newobj::browser::miniblink*)param)->qwidget()->geometry().x());
		}, 0);
	// y
	BIND_FUNCTION2(miniblink, "noui_window_y", {
		return JS_INT(((newobj::browser::miniblink*)param)->qwidget()->geometry().y());
		}, 0);
	// ��С��
	BIND_FUNCTION2(miniblink, "noui_window_minimize", {
		((newobj::browser::miniblink*)param)->qwidget()->setWindowState(Qt::WindowMinimized);
		return JS_UNDEFINED;
		}, 0);
	// ���ô�С
	BIND_FUNCTION2(miniblink, "noui_window_resize", {
		QRect rect = ((newobj::browser::miniblink*)param)->qwidget()->geometry();
		rect.setX(ARG_INT(0));
		rect.setY(ARG_INT(1));
		rect.setWidth(ARG_INT(2));
		rect.setHeight(ARG_INT(3));
		((newobj::browser::miniblink*)param)->qwidget()->setGeometry(rect);
		return JS_UNDEFINED;
	}, 4);

	// �̶��߶�
	BIND_FUNCTION2(miniblink, "noui_window_set_fixed_height", {
		((newobj::browser::miniblink*)param)->qwidget()->setFixedHeight(ARG_INT(0));
		return JS_UNDEFINED;
		}, 1);
	// �̶����
	BIND_FUNCTION2(miniblink, "noui_window_set_fixed_width", {
		((newobj::browser::miniblink*)param)->qwidget()->setFixedWidth(ARG_INT(0));
		return JS_UNDEFINED;
		}, 1);
	// ���ر�����
	BIND_FUNCTION2(miniblink, "noui_window_topbar_hide", {
		((newobj::browser::miniblink*)param)->qwidget()->setWindowFlags(Qt::FramelessWindowHint);
		return JS_UNDEFINED;
		}, 0);
	// ��ʾ
	BIND_FUNCTION2(miniblink, "noui_window_show", {
		((newobj::browser::miniblink*)param)->qwidget()->show();
		return JS_UNDEFINED;
		}, 0);
	// ����
	BIND_FUNCTION2(miniblink, "noui_window_hide", {
		((newobj::browser::miniblink*)param)->qwidget()->hide();
		return JS_UNDEFINED;
		}, 0);
	return miniblink;
}

void newobj::noui::miniblink::destory(newobj::browser::miniblink* miniblink)
{
	if (miniblink == 0)
		return;
	delete (newobj::browser::miniblink*)miniblink;
}

#endif
#endif
