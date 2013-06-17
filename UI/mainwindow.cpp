#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), ".", tr("C Minus Source File( *.cm )"));

    if (! files.open(fileName)) return;
    ui->wEditor->setDocument( files.current()->doc);
    // workaroud for rehighlighting
    ui->wEditor->rehighlight();
}
