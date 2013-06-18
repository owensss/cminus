#include "CMinusFiles.hpp"
#include <QTextStream>
#include <QDebug>
#include <iostream>

namespace cminus {
	CMinusFiles::~CMinusFiles() {
		iterator end = list.end();
		for ( iterator iter = list.begin(); iter != end; ++iter ) {
            delete iter->doc;
		}
	}

    CMinusFiles::iterator CMinusFiles::open(const QString& path) {
		CMinusFile file;
        QFile fs(path);
        iterator found = find(path);
        if (found != list.end()) return found;

        if (! fs.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return list.end();
		}

        QTextStream out(&fs);
        file.filename = path;
        file.doc = new QTextDocument(out.readAll());
        file.doc->setModified(false);
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        list.push_back(file);
        endInsertRows();
        return --list.end(); // last element
        fs.close();
	}

	void CMinusFiles::writeAll(void) {
        iterator end = list.end();
		for ( iterator iter = list.begin(); iter != end; ++iter ) {
            QFile fs(iter->filename);
            if (fs.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
                QTextStream in(&fs);
                in << iter->doc->toPlainText();
            }
		}
	}

    bool CMinusFiles::write(iterator iter) {
        if (iter == list.end()) return false;
        QFile fs(iter->filename);
        if (fs.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            QTextStream in(&fs);
            in << iter->doc->toPlainText();
            return true;
		}
		return false;
	}

    bool CMinusFiles::close(iterator iter) {
        if (iter == list.end()) return false;
        iterator i = list.begin();
        iterator end = list.end();
        int count;
        for (count = 0; i != iter; ++i, ++count) if (i==end) break;

        if (i == list.end()) return false;

        beginRemoveRows(QModelIndex(), count, count);
        list.erase(iter);
        endRemoveRows();

        return true;
    }

    QVariant CMinusFiles::data(const QModelIndex &index, int role) const {
        if (role==Qt::DisplayRole) {
            size_t end = index.row();
            if (end > list.size()) return QVariant();
            const_iterator iter = list.begin();
            for (int i = 0; i < end; ++i) ++iter;
            QString tmp = iter->filename;
            int sidx = tmp.lastIndexOf("/");
            int fidx = tmp.lastIndexOf("\\");
            tmp.remove(0, (sidx>fidx?sidx:fidx)+1);
            return tmp;
        }
        return QVariant();
    }

    QVariant CMinusFiles::headerData(int section, Qt::Orientation orientation, int role) const {
        if (role==Qt::DisplayRole) {
            if (orientation == Qt::Horizontal)
                return tr("打开文件列表");
            if (orientation == Qt::Vertical)
                return section;
        }
        return QVariant();
    }

    CMinusFiles::iterator CMinusFiles::at(int index) {
        if (index > list.size()) return list.end();
        iterator iter = list.begin();
        for (int i = 0; i < index; ++i) ++iter;
        return iter;
    }
}

