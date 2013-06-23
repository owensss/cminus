#ifndef TEXTEDITOR_HPP
#define TEXTEDITOR_HPP

#include <QWidget>
#include <QTextEdit>
#include <QTextDocument>
#include "CMinusFiles.hpp"
class CMinusHighlighter;

namespace Ui {
class TextEditor;
}

class TextEditor : public QWidget
{
        Q_OBJECT
        
    public:
        explicit TextEditor(QWidget *parent = 0);
        void setDocument(cminus::CMinusFiles::iterator);
        ~TextEditor();
        cminus::CMinusFiles::iterator document() {return file;}
    public slots:
        void undo();
        void redo();
        void rehighlight();
    private slots:
        void on_textEdit_textChanged();
        void text_modified();
    signals:
        void modified(TextEditor*);
    private:
        void do_setDocument(QTextDocument* doc);
        void autoIndent();
        void reset_tabWidth();
    private:
        Ui::TextEditor *ui;
        QTextDocument* document_;
        CMinusHighlighter* highlighter;
        cminus::CMinusFiles::iterator file;
};

#endif // TEXTEDITOR_HPP
