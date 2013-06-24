#include "CMinusFiles.hpp"
#include <QTextStream>
#include <QDebug>
#include <iostream>

namespace cminus {
     const QString CMinusFile::untitled = "Untitled";

	CMinusFiles::~CMinusFiles() {
		iterator end = list.end();
		for ( iterator iter = list.begin(); iter != end; ++iter ) {
            delete iter->doc;
		}
	}

    CMinusFiles::iterator CMinusFiles::open(const QString& path, QPlainTextEdit *parent) {
        iterator iter = find(path);
        if (valid(iter)) return list.end();
		CMinusFile file;
        file.filename = path;
        file.doc = new QTextDocument(parent);
        file.doc->setDocumentLayout(new QPlainTextDocumentLayout(file.doc));
        if (! do_open(file)) {
            delete file.doc;
            return list.end();
        }
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        list.push_back(file);
        endInsertRows();

        return --list.end(); // last element
	}

    CMinusFiles::iterator CMinusFiles::reopen(iterator iter) {
        iterator i = list.begin();
        iterator end = list.end();

        for (; i != end; ++i)
            if (i->filename == iter->filename && iter != i) {
                iter->filename = "";
                return list.end();
            }
        do_open(*iter);
        return iter;
    }

    bool CMinusFiles::do_open(CMinusFile &file) {
        QFile fs(file.filename);

        if (! fs.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return false;
        }

        QTextStream out(&fs);
        file.doc->setPlainText(out.readAll());

        file.doc->clearUndoRedoStacks();
        file.doc->setModified(false);
        fs.close();
        return true;
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
            iter->doc->setModified(false);
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
        // if (rowCount() == 0) create();
        return true;
    }

    CMinusFiles::iterator CMinusFiles::create() {
        CMinusFile file;
        file.doc = new QTextDocument();

        file.doc->clearUndoRedoStacks();
        file.doc->setModified(false);
        file.doc->setDocumentLayout(new QPlainTextDocumentLayout(file.doc));
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        list.push_back(file);
        endInsertRows();
        return --list.end(); // last element
    }

    QVariant CMinusFiles::data(const QModelIndex &index, int role) const {
        if (role==Qt::DisplayRole) {
            size_t end = index.row();
            if (end > list.size()) return QVariant();
            const_iterator iter = list.begin();
            for (size_t i = 0; i < end; ++i) ++iter;
            QString tmp = iter->filename;
            int sidx = tmp.lastIndexOf("/");
            int fidx = tmp.lastIndexOf("\\");
            tmp.remove(0, (sidx>fidx?sidx:fidx)+1);
            if (tmp == "") tmp = "Untitled";
            if (iter->doc->isModified()) tmp += "*";
            return tmp;
        }
        return QVariant();
    }

    QVariant CMinusFiles::headerData(int /*section*/, Qt::Orientation orientation, int role) const {
        if (role==Qt::DisplayRole) {
            if (orientation == Qt::Horizontal)
                return tr("Opened File List");
            if (orientation == Qt::Vertical)
                return QVariant();
        }
        return QVariant();
    }

    CMinusFiles::iterator CMinusFiles::at(size_t index) {
        if (index > list.size()) return list.end();
        iterator iter = list.begin();
        for (int i = 0; i < index; ++i) ++iter;
        return iter;
    }
}

