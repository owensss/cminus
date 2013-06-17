#ifndef CMINUSHIGHLIGHTER_HPP
#define CMINUSHIGHLIGHTER_HPP

#include <QSyntaxHighlighter>

class CMinusHighlighter : public QSyntaxHighlighter
{
        Q_OBJECT
    public:
        explicit CMinusHighlighter(QObject *parent = 0);
    protected:
        void highlightBlock(const QString &text);
    signals:
        
    public slots:

};

#endif // CMINUSHIGHLIGHTER_HPP
