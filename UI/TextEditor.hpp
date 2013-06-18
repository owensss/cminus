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
        void setFiles(cminus::CMinusFiles* files_);
        ~TextEditor();
        cminus::CMinusFiles::iterator current() {return current_;}
    public slots:
        void rehighlight();
        void changeCurrent(cminus::CMinusFiles::iterator);
        void changeCurrent(const QModelIndex& idx);
    private:
        void do_setDocument(QTextDocument* doc);
    private:
        Ui::TextEditor *ui;
        QTextDocument* document;
        CMinusHighlighter* highlighter;
        cminus::CMinusFiles* files;
        cminus::CMinusFiles::iterator current_;
};

#endif // TEXTEDITOR_HPP
