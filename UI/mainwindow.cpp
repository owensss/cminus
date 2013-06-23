#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "TextEditor.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // ui->->setFiles(&files);
    // create a new
    // on_action_New_triggered();
    connect(ui->codeTabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    ui->folders->setModel(&files);
    ui->codeTabs->setView(ui->folders);
    ui->codeTabs->setFiles(&files);
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
    if (! files.write(ui->codeTabs->document())) {
        if (ui->codeTabs->document()->filename == "") {
            on_actionSaveAs_triggered();
        }
    }
}

void MainWindow::on_actionSaveAs_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save As"), ".", tr("C Minus Source File( *.cm )"));
    if (fileName.isEmpty()) return;
    if (! fileName.endsWith(".cm", Qt::CaseInsensitive))
        fileName += ".cm";
    ui->codeTabs->document()->filename = fileName;
    files.write(ui->codeTabs->document());
}

void MainWindow::on_action_Close_triggered()
{
    closeTab(ui->codeTabs->currentIndex());
}

void MainWindow::on_action_Undo_triggered()
{
    ui->codeTabs->undo();
}

void MainWindow::on_action_Redo_triggered()
{
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

void MainWindow::on_actionCloseAll_triggered()
{
    if (files.isModified(ui->codeTabs->document())) {
        int ret = QMessageBox::warning(this, "Warning", "File Modified, Save before close?",
                                 QMessageBox::Save | QMessageBox::Discard| QMessageBox::Cancel,
                                 QMessageBox::Save);
        if (ret == QMessageBox::Save)
            on_actionSave_triggered();
        else if (ret == QMessageBox::Cancel)
            return;
    }
    files.close(ui->codeTabs->document());
    // ui->codeTabs->changeCurrent(files.at(0));
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
