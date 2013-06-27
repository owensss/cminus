#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QProcess>
#include <QFileInfo>
#include "TextEditor.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(ui->codeTabs);
    splitter->addWidget(ui->outputText);

    ui->verticalLayout_2->addWidget(splitter);
    ui->verticalLayout_2->addWidget(ui->commandLinkButton);
    // ui->outputText->setFixedHeight(200);

    ui->outputText->hide();

    // ui->->setFiles(&files);
    // create a new
    // on_action_New_triggered();
    connect(ui->codeTabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    ui->folders->setModel(&files);
    ui->codeTabs->setView(ui->folders);
    ui->codeTabs->setFiles(&files);
    compiler = new QProcess(this);
    connect(compiler, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(writeOutput(int, QProcess::ExitStatus)));
    connect(compiler, SIGNAL(error(QProcess::ProcessError)), this, SLOT(writeErrorOutput(QProcess::ProcessError)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), ".", tr("C Minus Source File( *.cm )"));

    if (    ui->codeTabs->count() > 0
         && ui->codeTabs->document()->filename == ""
         && (! ui->codeTabs->document()->doc->isModified())
    ) {
            ui->codeTabs->document()->filename = fileName;
            if (! files.valid(files.reopen(ui->codeTabs->document()))) {
                QMessageBox::warning(this, "Warning", "Unable to Open File!");
                return ;
            }
    } else {
        cminus::CMinusFiles::iterator iter = files.open(fileName);
        if (! files.valid(iter)) {
            QMessageBox::warning(this, "Warning", "Unable to Open File!");
            return;
        }
        ui->codeTabs->insert(iter);
    }

    // workaroud for rehighlighting
    // ui->wEditor->rehighlight();
}

void MainWindow::on_actionSave_triggered()
{
    qDebug () << "save";
    if (files.rowCount() == 0) return;
    if (! files.write(ui->codeTabs->document())) {
        if (ui->codeTabs->document()->filename == "") {
            on_actionSaveAs_triggered();
        }
    }
}

void MainWindow::on_actionSaveAs_triggered() {
    if (files.rowCount() == 0) return;
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save As"), ".", tr("C Minus Source File( *.cm )"));
    if (fileName.isEmpty()) return;
    if (! fileName.endsWith(".cm", Qt::CaseInsensitive))
        fileName += ".cm";
    ui->codeTabs->document()->filename = fileName;
    ui->codeTabs->updateLabel();
    files.write(ui->codeTabs->document());
}

void MainWindow::on_action_Close_triggered()
{
    if (files.rowCount() == 0) return;
    closeTab(ui->codeTabs->currentIndex());
}

void MainWindow::on_action_Undo_triggered()
{
    if (files.rowCount() == 0) return;
    ui->codeTabs->undo();
}

void MainWindow::on_action_Redo_triggered()
{
    if (files.rowCount() == 0) return;
    ui->codeTabs->redo();
}

void MainWindow::on_action_Exit_triggered()
{
    this->close();
}

void MainWindow::on_action_New_triggered()
{
    cminus::CMinusFiles::iterator iter = files.create();
    if (! files.valid(iter)) return;

    ui->codeTabs->insert(iter);
    // workaroud for rehighlighting
    // ui->wEditor->rehighlight();
}

void MainWindow::on_actionSaveAll_triggered()
{
    files.writeAll();
}

void MainWindow::closeTab(int t) {
    qDebug() << "close" << t;
    if (! do_close(dynamic_cast<TextEditor*>(ui->codeTabs->widget(t))->document()))
        return;
    QWidget* wg = ui->codeTabs->widget(t);
    ui->codeTabs->removeTab(t);
    delete wg;
}

bool MainWindow::do_close(cminus::CMinusFiles::iterator iter) {
    if (files.isModified(iter)) {
        int ret = QMessageBox::warning(this, "Warning", "File Modified, Save before close?",
                                 QMessageBox::Save | QMessageBox::Discard| QMessageBox::Cancel,
                                 QMessageBox::Save);

        if (ret == QMessageBox::Save)
            on_actionSave_triggered();
        else if (ret == QMessageBox::Cancel)
            return false;
        // discard => do nothing
    }
    files.close(iter);
    // ui->codeTabs->removeCurrent();
    return true;
}

void MainWindow::on_folders_clicked(const QModelIndex &index)
{
    int i = ui->codeTabs->DocumentIndex(files.at(index.row()));
    if (i == -1) return;
    ui->codeTabs->setCurrentIndex(i);
}

QString removeSuffix(const QString& arg) {
    QFileInfo info(arg);
    return info.path()+"/"+info.baseName();
}

void MainWindow::on_action_Compile_triggered()
{
    error = false;
    if (files.rowCount() == 0) return;
    cminus::CMinusFiles::iterator iter = ui->codeTabs->document();
    if (compiler->state() == QProcess::Running || iter->filename.isEmpty()) return;
    if (iter->doc->isModified()) {
        this->on_actionSave_triggered();
    }
    QString args;
    QString filename = iter->filename;

    args += " ./cminus ";
    args += iter->filename+" ";
    // args += " >"+file_no_suffix+".mips";

    qDebug() << args;
    compiler->start(args);
    compiler->waitForStarted();
    ui->outputText->show();
}

void MainWindow::on_commandLinkButton_clicked()
{
    if (ui->outputText->isHidden()) ui->outputText->show();
    else ui->outputText->hide();
}

void MainWindow::writeOutput(int code, QProcess::ExitStatus status) {
    qDebug() << code;
    if (code) error = true;
    else error = false;
    ui->outputText->setText(compiler->readAllStandardError());

    if (! error) {
        QString file_no_suffix = removeSuffix(compiler->arguments()[0]);
        QFile file(file_no_suffix+".mips");
        if (! file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QMessageBox::warning(this, "Warning", "Unable to open "+file_no_suffix+".mips for write");
            return;
        }
        QTextStream stream(&file);
        stream << compiler->readAllStandardOutput();
        file.close();
    }
    compiler->close();
}

void MainWindow::writeErrorOutput(QProcess::ProcessError) {
    ui->outputText->setText("unable to execute "+compiler->program()+" "+compiler->arguments().join(" "));
    compiler->close();
}

void MainWindow::on_action_Run_triggered()
{
    if (files.rowCount() == 0) return;
    on_action_Compile_triggered();
    compiler->waitForFinished();
    QString filename(ui->codeTabs->document()->filename);
    filename = removeSuffix(filename)+".mips";
    QFileInfo info(filename);

    if (! info.exists()) {
        QMessageBox::warning(this, "Warning", "Unable to open "+info.filePath());
        return;
    }

    if (error) {
        int ret = QMessageBox::warning(this, "Warning",
                             "There Are some errors, would you like to execute the lastest succeed version?",
                             QMessageBox::Ok, QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel) return;
        // if OK, proceed
    }
    system(QString("./QtSpim "+filename).toStdString().c_str());
}
