#pragma once


#ifdef USE_QWEBENGINE
#include "core/define.h"
#include <QWebEngineView>
#include <QWebChannel>
#include <QString>
#include <QStackedLayout>
#include <QDebug>




/****************************************************+
 * Class：Web引擎
 * Desc：窗口继承该类，用于实现窗口H5化
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

