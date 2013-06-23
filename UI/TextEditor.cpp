#include "TextEditor.hpp"
#include "ui_TextEditor.h"
#include "CMinusHighlighter.hpp"
#include <QDebug>
#include "codeeditor.hpp"

TextEditor::TextEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextEditor),
    document_(nullptr),
    highlighter(nullptr)
{
    ui->setupUi(this);
    // highlight
    highlighter = new CMinusHighlighter(ui->textEdit);
    // highlighter->setDocument(ui->textEdit->document());
}

TextEditor::~TextEditor()
{
    delete ui;
    ui = nullptr;
}

void TextEditor::rehighlight() {
    highlighter->rehighlight();
}

void TextEditor::setDocument(cminus::CMinusFiles::iterator iter) {
    file = iter;
    document_ = file->doc;
    document_->setParent(ui->textEdit);
    highlighter->setParent(document_);
    highlighter->setDocument(document_);
    ui->textEdit->setDocument(document_);

    connect(file->doc, SIGNAL(modificationChanged(bool)), this, SLOT(text_modified()));
    ui->textEdit->setTabWidth();
}
// workaround for connect SIGNAL, SIGNAL
void TextEditor::text_modified() {
    qDebug() << "aaa";
    emit modified(this);
}

void TextEditor::undo() {
    ui->textEdit->undo();
}

void TextEditor::redo() {
    ui->textEdit->redo();
}

/* should do the following:
 * 1. If modified, add a * after the filename
 * 2. auto indent
 */
void TextEditor::on_textEdit_textChanged()
{
    // testModified();
    // 无节操实现 括号匹配
    /*
    QString text = this->document_->toPlainText();
    if (text.size() == 0) return;
    if (text[text.size()-1] == '{') {
        text = text + "\n\t\n}";
        this->document_->setPlainText(text);
        ui->textEdit->moveCursor(QTextCursor::Up);
        ui->textEdit->moveCursor(QTextCursor::Right);
    }
    */
}

void TextEditor::reset_tabWidth() {
    // tabwidth
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);

    ui->textEdit->setFont(font);

    const int tabStop = 4;  // 4 characters

    QFontMetrics metrics(font);
    ui->textEdit->setTabStopWidth(tabStop * metrics.width(' '));
}

void TextEditor::autoIndent() {

}
