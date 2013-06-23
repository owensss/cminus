#include "FileTreeView.hpp"
#include <QFileSystemModel>
#include <QDebug>

FileTreeView::FileTreeView(QWidget *parent) :
    QListView(parent)
{
    model = new QFileSystemModel(this);
    model->setRootPath(".");
    setModel(model);
    setRootIndex(model->index(QDir::currentPath()));
    qDebug() << model->nameFilters();
}
