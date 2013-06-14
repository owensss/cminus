#include "CMinusHighlighter.hpp"
#include <QFont>
#include <QDebug>

CMinusHighlighter::CMinusHighlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{
}

void CMinusHighlighter::highlightBlock(const QString &text) {
    qDebug() << "called";
    QTextCharFormat keywordFormat;

    keywordFormat.setFontWeight(QFont::Bold);
    // keywordFormat.setForeground();

    QString keywordPattern = "\\b(int|void|return|while|if|else)\\b";

    QRegExp keyexpr(keywordPattern);
    int key_index = text.indexOf(keyexpr);
    while (key_index >= 0) {
        int length = keyexpr.matchedLength();
        setFormat(key_index, length, keywordFormat);
        setFormat(key_index, length, QColor(255, 0, 255));
        key_index = text.indexOf(keyexpr, key_index + length);
    }
}
