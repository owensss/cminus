#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "CMinusFiles.hpp"

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

    private:
        Ui::MainWindow *ui;
        cminus::CMinusFiles files;
};

#endif // MAINWINDOW_HPP
