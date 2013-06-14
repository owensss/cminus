#include "TextEditor.hpp"
#include "ui_TextEditor.h"
#include "CMinusHighlighter.hpp"

TextEditor::TextEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextEditor)
{
    ui->setupUi(this);
    document = ui->textEdit->document();
    highlighter = new CMinusHighlighter(ui->textEdit->document());
    highlighter->setDocument(document);
}

TextEditor::~TextEditor()
{
    delete ui;
    delete highlighter;
}

void TextEditor::on_textEdit_textChanged()
{
    // highlighter->rehighlight();
}
