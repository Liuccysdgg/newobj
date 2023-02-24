#pragma once
#include "public/define.h"
#if LIB_QT == 1
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
         * Class：文本高亮
         **************************************************************************************************/
        class MQT_API highlighter :private QSyntaxHighlighter
        {
        public:
            highlighter(QTextDocument* parent = nullptr);
            ~highlighter();
            /*
                添加规则
                   font                     ：字体
                   reg                      ：表达式
                   foreground         ：前景色
                   background        ：背景色
            */
            void add_rule(const QFont& font, const QString& reg, QBrush* foreground = nullptr, QBrush* background = nullptr);
        private:
            virtual void highlightBlock(const QString& text) override;//重写父类这个函数 自动调用
        private:
            struct HighlightingRule//语法规则结构体，包含正则表达式模式串和匹配的样式  
            {
                QRegularExpression pattern;
                QTextCharFormat format;
            };
            QVector<HighlightingRule> highlightingRules;//规则的集合，可以定义多个高亮规则

            QRegularExpression commentStartExpression; //注释的高亮，使用highliangBlock()匹配，下文提到
            QRegularExpression commentEndExpression;

            QTextCharFormat keywordFormat;//高亮样式，关键词，一下顾名思义
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