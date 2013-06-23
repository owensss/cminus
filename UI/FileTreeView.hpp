#ifndef FILETREEVIEW_HPP
#define FILETREEVIEW_HPP

#include <QListView>

class QFileSystemModel;

class FileTreeView : public QListView
{
        Q_OBJECT
    public:
        explicit FileTreeView(QWidget *parent = 0);
        
    signals:
        
    public slots:
    private:
        QFileSystemModel* model;
};

#endif // FILETREEVIEW_HPP
