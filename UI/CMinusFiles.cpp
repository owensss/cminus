#include "CMinusFiles.hpp"
#include <QTextStream>

namespace cminus {
	CMinusFiles::~CMinusFiles() {
		iterator end = list.end();
		for ( iterator iter = list.begin(); iter != end; ++iter ) {
            if (iter->fs->isOpen()) {
                iter->fs->close();
                delete iter->fs;
                delete iter->doc;
			}
		}
	}

    CMinusFiles::iterator CMinusFiles::open(const QString& path, bool set_current) {
		CMinusFile file;
        file.fs = new QFile(path);
        if (! file.fs->open(QIODevice::ReadWrite | QIODevice::Text)) {
            delete file.fs;
            return list.end();
		}

        QTextStream out(file.fs);

        file.doc = new QTextDocument(out.readAll());
        file.doc->setModified(false);
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        list.push_back(file);
        endInsertRows();
        return --list.end(); // last element
	}

	void CMinusFiles::writeAll(void) {
        iterator end = list.end();
		for ( iterator iter = list.begin(); iter != end; ++iter ) {
            if (iter->fs->isOpen()) {
                QTextStream in(iter->fs);
                in << iter->doc->toPlainText();
			}
		}
	}

    bool CMinusFiles::write(const QTextDocument* doc) {
        iterator iter = find(doc);
		if (iter == list.end()) return false;
        if (! iter->fs->isOpen()) {
            QTextStream in(iter->fs);
            in << iter->doc->toPlainText();
			return true;
		}
		return false;
	}

    QVariant CMinusFiles::data(const QModelIndex &index, int role) const {
        if (role==Qt::DisplayRole) {
            size_t end = index.row();
            if (end > list.size()) return QVariant();
            const_iterator iter = list.begin();
            for (int i = 0; i < end; ++i) ++iter;
            QString tmp = iter->fs->fileName();
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

