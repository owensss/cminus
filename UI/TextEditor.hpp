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
        ~TextEditor();
        
    private slots:
        void on_textEdit_textChanged();
    private:
        Ui::TextEditor *ui;
        QTextDocument* document;
        CMinusHighlighter* highlighter;
};

#endif // TEXTEDITOR_HPP
