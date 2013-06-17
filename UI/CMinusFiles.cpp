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

	bool CMinusFiles::open(const QString& path, bool set_current) {
		CMinusFile file;
        file.fs = new QFile(path);
        if (! file.fs->open(QIODevice::ReadWrite | QIODevice::Text)) {
            delete file.fs;
			return false;
		}

        QTextStream out(file.fs);

        file.doc = new QTextDocument(out.readAll());
        file.doc->setModified(false);
        list.push_back(file);
        if (set_current) {
            current_ = --list.end();
        }
        return true;
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
}

