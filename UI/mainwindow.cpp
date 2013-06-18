#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->wEditor->setFiles(&files);
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
    files.write(ui->wEditor->current());
}

void MainWindow::on_action_Close_triggered()
{
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
