#pragma once


#ifdef USE_QWEBENGINE
#include "core/define.h"
#include <QWebEngineView>
#include <QWebChannel>
#include <QString>
#include <QStackedLayout>
#include <QDebug>




/****************************************************+
 * Class��Web����
 * Desc�����ڼ̳и��࣬����ʵ�ִ���H5��
 ****************************************************/
class WebEngine:public QWebEngineView
{

public:
    WebEngine(QWidget *parent):QWebEngineView(parent)
    {
       // this->m_webchannel = new QWebChannel(this);
       // this->m_webchannel->registerObject("bridge", this);
       // this->page()->setWebChannel(this->m_webchannel);
    }
    ~WebEngine()
    {
        if(m_webchannel!=nullptr)
            delete m_webchannel;
    }
public:
    QWebChannel *m_webchannel = nullptr;
public:

};
#endif

