#include "highlighter.h"
#ifdef LIB_QT
#include <QtGui/qfont.h>
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
newobj::ui::highlighter::highlighter(QTextDocument* parent):QSyntaxHighlighter(parent)
{

}
newobj::ui::highlighter::~highlighter()
{
}
void newobj::ui::highlighter::add_rule(const QFont& font, const QString& texts, QBrush* foreground, QBrush* background)
{
    HighlightingRule rule;
    quotationFormat.setFont(font);
    if(foreground != nullptr)
        quotationFormat.setForeground(*foreground);
    if(background != nullptr)
        quotationFormat.setBackground(*background);
    rule.pattern = QRegularExpression(texts);
    rule.format = quotationFormat;
    highlightingRules.append(rule);
}

void newobj::ui::highlighter::highlightBlock(const QString& text)
{
    foreach(const HighlightingRule & rule, highlightingRules) {//�ı����ø�������
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
#endif
#endif

