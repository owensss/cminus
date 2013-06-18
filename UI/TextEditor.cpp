#include "TextEditor.hpp"
#include "ui_TextEditor.h"
#include "CMinusHighlighter.hpp"
#include <QDebug>

TextEditor::TextEditor(QWidget *parent) :
    files(nullptr), QWidget(parent),
    ui(new Ui::TextEditor)
{
    ui->setupUi(this);
    document = ui->textEdit->document();
    highlighter = new CMinusHighlighter(ui->textEdit);
    highlighter->setDocument(ui->textEdit->document());
    // highlight

    ui->textEdit->setTabStopWidth(4);
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
    // list View
    ui->listFile->setModel(files);
}

TextEditor::~TextEditor()
{
    delete ui;
    delete highlighter;
}

void TextEditor::do_setDocument(QTextDocument* doc) {
    document = doc;
    ui->textEdit->setDocument(doc);
    highlighter->setDocument(ui->textEdit->document());
}

void TextEditor::rehighlight() {
    highlighter->rehighlight();
}

void TextEditor::setFiles(cminus::CMinusFiles *files_) {
    files = files_;
    ui->listFile->setModel(files);

    connect(ui->listFile, SIGNAL(clicked(QModelIndex)), this, SLOT(changeCurrent(const QModelIndex&)));
}

void TextEditor::changeCurrent(cminus::CMinusFiles::iterator iter) {
    current_ = iter;
    if (! files->valid(iter)) {
        do_setDocument(nullptr);
        return;
    }
    do_setDocument(iter->doc);
}

void TextEditor::changeCurrent(const QModelIndex &idx) {
    changeCurrent(files->at(idx.row()));
}
