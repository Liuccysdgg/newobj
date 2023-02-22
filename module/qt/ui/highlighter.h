#pragma once
#include "public/define.h"
#ifdef LIB_QT
#include "public/define.h"
#include "basetype/string.h"
#include "util/map.hpp"
#include <QtCore/qvariant.h>
#include <functional>
#include <QtGui/QSyntaxHighlighter>
#include <QtGui/QTextCharFormat>
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtCore/QRegularExpression>
namespace newobj
{
    namespace ui
    {
        /**************************************************************************************************
         * Class���ı�����
         **************************************************************************************************/
        class MQT_API highlighter :private QSyntaxHighlighter
        {
        public:
            highlighter(QTextDocument* parent = nullptr);
            ~highlighter();
            /*
                ��ӹ���
                   font                     ������
                   reg                      �����ʽ
                   foreground         ��ǰ��ɫ
                   background        ������ɫ
            */
            void add_rule(const QFont& font, const QString& reg, QBrush* foreground = nullptr, QBrush* background = nullptr);
        private:
            virtual void highlightBlock(const QString& text) override;//��д����������� �Զ�����
        private:
            struct HighlightingRule//�﷨����ṹ�壬����������ʽģʽ����ƥ�����ʽ  
            {
                QRegularExpression pattern;
                QTextCharFormat format;
            };
            QVector<HighlightingRule> highlightingRules;//����ļ��ϣ����Զ�������������

            QRegularExpression commentStartExpression; //ע�͵ĸ�����ʹ��highliangBlock()ƥ�䣬�����ᵽ
            QRegularExpression commentEndExpression;

            QTextCharFormat keywordFormat;//������ʽ���ؼ��ʣ�һ�¹���˼��
            QTextCharFormat classFormat;
            QTextCharFormat singleLineCommentFormat;
            QTextCharFormat multiLineCommentFormat;
            QTextCharFormat quotationFormat;
            QTextCharFormat functionFormat;
        };

    }
}
#endif

#endif