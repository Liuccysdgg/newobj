#pragma once
#include "public/define.h"
#ifdef LIB_QT
#ifdef QT4
#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#else
#include "QtWidgets/qwidget.h"
#include "QtCore/qdebug.h"
#include "QtWidgets/qmouseeventtransition.h"
#include "QtGui/qevent.h"
#endif
#endif
#include "util/window.h"
#include "util/system.h"
namespace newobj
{
#ifdef LIB_QT
    namespace ui
    {
        template<typename T>
        class topbar : public T
        {
        public:
            topbar<T>(QWidget* parent):T(parent)
            {
                m_height = 0;
                m_mousePres = false;
            }
            ~topbar<T>()
            {

            }
            /*�ñ������߶�*/
            void set_topbar_height(uint32 height)
            {
                m_height = height;
            }
        public:
            virtual void on_move() {

            }
        protected:
            virtual void mousePressEvent(QMouseEvent* event)
            {
                if (event->button() == Qt::LeftButton && event->pos().y() < (int)m_height)
                {
                    //�������ڴ����λ�ã�����ʹ��event->globalPos() - this->pos()��
                    m_movePoint = event->pos();
                    m_mousePres = true;
                }
            }
            virtual void mouseReleaseEvent(QMouseEvent*)
            {
                //�������Ϊδ������
                m_mousePres = false;
            }
            virtual void mouseMoveEvent(QMouseEvent* event)
            {
                //��������������
                if (m_mousePres)
                {
                    //��������ƶ�������������
                    if (event->globalPos().y() > (int)getTaskBarRect().top)
                    {
                        QCursor::setPos(QPoint((int)event->globalPos().x(), (int)getTaskBarRect().top));
                    }
                    //����������Ļ��λ��
                    QPoint movePos = event->globalPos();
                    //�ƶ�������λ��
                    this->move(movePos - m_movePoint);

                    on_move();
                }
            }
            
        private:
            Rect getTaskBarRect()
            {
                Rect rect;
#ifdef _WIN32
                auto hwndTaskbar = newobj::window::find("Shell_TrayWnd");
                rect = newobj::window::rect(hwndTaskbar);
#else        
                rect.top = system::screen::device_size().width;
#endif
                return rect;
            }
        private:
            QPoint m_movePoint;
            bool m_mousePres;
            uint32 m_height;

        };
    }
#endif
}
