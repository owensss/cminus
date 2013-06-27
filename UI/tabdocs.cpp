#include "tabdocs.hpp"
#include "TextEditor.hpp"
#include <QDebug>
#include <QListView>
TabDocs::TabDocs(QWidget *parent) :
    QTabWidget(parent)
{
}

void TabDocs::updateLabel() {
    changeTabLabel(current());
}

void TabDocs::insert(cminus::CMinusFiles::iterator iter) {

    TextEditor* editor = new TextEditor(this);
    editor->setDocument(iter);

    this->setCurrentIndex(this->insertTab(currentIndex()+1, editor, iter->name()));
    connect(editor, SIGNAL(modified(TextEditor*)), this, SLOT(changeTabLabel(TextEditor*)));
}

void TabDocs::removeCurrent(void) {
    if (currentIndex() == -1) return;
    QWidget* cw = currentWidget();
    removeTab(currentIndex());
    delete cw;
}

void TabDocs::changeTabLabel(TextEditor *widget_) {
    int i = indexOf(widget_);
    if (i == -1) return;
    if (widget_->document()->doc->isModified())
        setTabText(i, widget_->document()->name()+"*");
    else setTabText(i, widget_->document()->name());
    int idx = files->at(widget_->document());
    view->update(files->index(idx));
}

TextEditor* TabDocs::current() {
    return dynamic_cast<TextEditor*>(currentWidget());
}

void TabDocs::undo() {
    current()->undo();
}

void TabDocs::redo() {
    current()->redo();
}

int TabDocs::DocumentIndex(cminus::CMinusFiles::iterator iter) {
    for (int i = 0; i < count(); ++i) {
        TextEditor* ed = dynamic_cast<TextEditor*>(widget(i));
        if (ed->document() == iter) return i;
    }
    return -1;
}
