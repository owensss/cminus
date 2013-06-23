#include "CMinusHighlighter.hpp"
#include <QFont>
#include <QDebug>

CMinusHighlighter::CMinusHighlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{
}
void CMinusHighlighter::highlightBlock(const QString &text) {
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
    // highlight comments
    QTextCharFormat multiLineCommentFormat;
    multiLineCommentFormat.setForeground(Qt::blue);
    QRegExp startExpression("/\\*");
    QRegExp endExpression("\\*/");

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(startExpression);

    while (startIndex >= 0) {
       int endIndex = text.indexOf(endExpression, startIndex);
       int commentLength;
       if (endIndex == -1) {
           setCurrentBlockState(1);
           commentLength = text.length() - startIndex;
       } else {
           commentLength = endIndex - startIndex
                           + endExpression.matchedLength();
       }
       setFormat(startIndex, commentLength, multiLineCommentFormat);
       startIndex = text.indexOf(startExpression,
                                 startIndex + commentLength);
    }

    // highlight numbers
    QString numPattern = "[0-9]+";

    QRegExp numexp(numPattern);
    int num_index = text.indexOf(numexp);
    while (num_index >= 0) {
        int length = numexp.matchedLength();
        // setFormat(key_index, length, keywordFormat);
        setFormat(num_index, length, QColor(0, 100, 0));
        num_index = text.indexOf(numexp, num_index + length);
    }
}
