#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->wEditor->setFiles(&files);
    // create a new
    on_action_New_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), ".", tr("C Minus Source File( *.cm )"));

    cminus::CMinusFiles::iterator iter = files.open(fileName);
    if (! files.valid(iter)) return;

    ui->wEditor->changeCurrent(iter);
    // workaroud for rehighlighting
    ui->wEditor->rehighlight();
}

void MainWindow::on_actionSave_triggered()
{
    if (! files.write(ui->wEditor->current())) {
        if (ui->wEditor->current()->filename == "") {
            on_actionSaveAs_triggered();
        }
    }
}

void MainWindow::on_actionSaveAs_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save As"), ".", tr("C Minus Source File( *.cm )"));
    if (! fileName.endsWith(".cm", Qt::CaseInsensitive))
        fileName += ".cm";
    ui->wEditor->current()->filename = fileName;
    files.write(ui->wEditor->current());
}

void MainWindow::on_action_Close_triggered()
{
    if (files.isModified(ui->wEditor->current())) {
        int ret = QMessageBox::warning(this, "Warning", "File Modified, Save before close?",
                                 QMessageBox::Save | QMessageBox::Discard| QMessageBox::Cancel,
                                 QMessageBox::Save);
        if (ret == QMessageBox::Save)
            on_actionSave_triggered();
        else if (ret == QMessageBox::Cancel)
            return;
    }
    files.close(ui->wEditor->current());
    ui->wEditor->changeCurrent(files.at(0));
}

void MainWindow::on_action_Undo_triggered()
{
    ui->wEditor->undo();
}

void MainWindow::on_action_Redo_triggered()
{
    ui->wEditor->redo();
}

void MainWindow::on_action_Exit_triggered()
{
    this->close();
}

void MainWindow::on_action_New_triggered()
{
    cminus::CMinusFiles::iterator iter = files.create();
    if (! files.valid(iter)) return;

    ui->wEditor->changeCurrent(iter);
    // workaroud for rehighlighting
    ui->wEditor->rehighlight();
}

void MainWindow::on_actionSaveAll_triggered()
{
    files.writeAll();
}
