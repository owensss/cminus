#include "TextEditor.hpp"
#include "ui_TextEditor.h"
#include "CMinusHighlighter.hpp"
#include <QDebug>

TextEditor::TextEditor(QWidget *parent) :
    QWidget(parent),files(nullptr),
    ui(new Ui::TextEditor),
    document(nullptr),
    highlighter(nullptr),
    current_(nullptr)
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
    ui = nullptr;
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

    current_ = files->at(0);
    ui->listFile->setCurrentIndex(ui->listFile->model()->index(0, 0));

    connect(ui->listFile, SIGNAL(clicked(QModelIndex)), this, SLOT(changeCurrent(const QModelIndex&)));
    connect(files, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(changeCurrent(QModelIndex, int, int)) );
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
    qDebug() << "change to " << idx.row();
    if (idx.row() != ui->listFile->currentIndex().row()) {
        ui->listFile->setCurrentIndex(idx);
        changeCurrent(files->at(idx.row()));
    }
}

void TextEditor::changeCurrent(const QModelIndex &parent, int begin, int /*end*/) {
   QModelIndex idx = files->index(begin, 0);
   changeCurrent(idx);
}

void TextEditor::undo() {
    ui->textEdit->undo();
    testModified();
}

void TextEditor::redo() {
    ui->textEdit->redo();
    testModified();
}

/* should do the following:
 * 1. If modified, add a * after the filename
 * 2. auto indent
 */
void TextEditor::on_textEdit_textChanged()
{
    testModified();
}

void TextEditor::testModified() {
    if (! ui) return; // workaround for an unbelievable problem 0 0
    if (ui->listFile->currentIndex().row() == -1) return;
    QModelIndex index = ui->listFile->currentIndex();
    ui->listFile->update(index);
}
