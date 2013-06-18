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

    private:
        Ui::MainWindow *ui;
        cminus::CMinusFiles files;
};

#endif // MAINWINDOW_HPP
