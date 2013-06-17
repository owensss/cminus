#include "TextEditor.hpp"
#include "ui_TextEditor.h"
#include "CMinusHighlighter.hpp"
#include <QDebug>

TextEditor::TextEditor(QWidget *parent) :
    QWidget(parent),
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
    // font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);

    ui->textEdit->setFont(font);

    const int tabStop = 4;  // 4 characters

    QFontMetrics metrics(font);
    ui->textEdit->setTabStopWidth(tabStop * metrics.width(' '));
}

TextEditor::~TextEditor()
{
    delete ui;
    delete highlighter;
}

void TextEditor::setDocument(QTextDocument* doc) {
    document = doc;
    ui->textEdit->setDocument(doc);
    highlighter->setDocument(ui->textEdit->document());
}

void TextEditor::rehighlight() {
    qDebug() << "rehighlight";
    highlighter->rehighlight();
}
