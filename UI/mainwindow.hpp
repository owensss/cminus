#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "CMinusFiles.hpp"
#include <QSplitter>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT
        
    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        
    private slots:
        void on_action_Open_triggered();

        void on_actionSave_triggered();

        void on_action_Close_triggered();

        void on_action_Undo_triggered();
        void on_action_Redo_triggered();
        void on_action_Exit_triggered();

        void on_action_New_triggered();
        void on_actionSaveAs_triggered();
        void on_actionSaveAll_triggered();

        void closeTab(int t);
        void on_folders_clicked(const QModelIndex &index);

        void on_action_Compile_triggered();

        void on_commandLinkButton_clicked();
        void writeOutput(int, QProcess::ExitStatus);
        void writeErrorOutput(QProcess::ProcessError);
        void on_action_Run_triggered();

private:
        bool do_close(cminus::CMinusFiles::iterator iter);
    private:
        bool error;
        Ui::MainWindow *ui;
        cminus::CMinusFiles files;
        QSplitter* splitter;
        QProcess* compiler;
};

#endif // MAINWINDOW_HPP
