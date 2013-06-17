#ifndef TEXTEDITOR_HPP
#define TEXTEDITOR_HPP

#include <QWidget>
#include <QTextEdit>
#include <QTextDocument>
class CMinusHighlighter;

namespace Ui {
class TextEditor;
}

class TextEditor : public QWidget
{
        Q_OBJECT
        
    public:
        explicit TextEditor(QWidget *parent = 0);
        void setDocument(QTextDocument* doc) ;
        ~TextEditor();
    public slots:
        void rehighlight();
    private:
        Ui::TextEditor *ui;
        QTextDocument* document;
        CMinusHighlighter* highlighter;
};

#endif // TEXTEDITOR_HPP
