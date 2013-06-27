#ifndef TABDOCS_HPP
#define TABDOCS_HPP

#include <QTabWidget>
#include <QStatusBar>
#include "CMinusFiles.hpp"
#include "TextEditor.hpp"
class TextEditor;
class QListView;

class TabDocs : public QTabWidget
{
        Q_OBJECT
    public:
        explicit TabDocs(QWidget *parent = 0);
        void setView(QListView* view_) {view = view_;}
        void setFiles(cminus::CMinusFiles* file_) {files = file_;}
        void setStatusBar(QStatusBar* bar_) {bar = bar_;}
        void insert(cminus::CMinusFiles::iterator iter);
        void updateLabel(void);
        void removeCurrent(void);
        int DocumentIndex(cminus::CMinusFiles::iterator iter);
        // void remove(cminus::CMinusFiles::iterator iter);
        void undo();
        void redo();
        TextEditor* current() ;
        cminus::CMinusFiles::iterator document() {return current()->document(); }
    signals:
        
    public slots:
        void changeTabLabel(TextEditor* widget_);
    private:
        QListView* view;
        cminus::CMinusFiles* files;
        QStatusBar* bar;
};

#endif // TABDOCS_HPP
