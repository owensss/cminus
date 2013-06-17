#include "CMinusFile.hpp"

namespace cminus {
	CMinusFiles::~CMinusFiles() {
		for (auto& f : fs ) 
			if (f.fs.isOpen()) f.fs.close();
	}

	bool CMinusFiles::open(const QString& path, bool set_current) {
		CMinusFile file;
		file.fs.setFileName(path);
		if (! file.fs.open(QIODevice::ReadWrite, QIODevice::Text)) {
			return false;
		}

		QTextStream text(&file.fs);

		file.doc.setPlainText(text);
		file.doc.setModified(false);
	}
}
