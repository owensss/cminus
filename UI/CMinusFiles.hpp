#ifndef CMINUSFILES_HPP
#define CMINUSFILES_HPP
#include <QFile>
#include <vector>
#include <QDocument>

namespace cminus {
	struct CMinusFile {
		QFile fs;
		QDocument doc; 
		// bool dirt; // isModified will do the trick
	};

	class CMinusFiles {
		public:
			typedef CMinusFile                       value_type;
			typedef std::list<value_type>            container;
			typedef typename container::iterator     iterator;
			typedef typename const iterator          const_iterator;
		public:
			CMinusFiles() {}
			~CMinusFiles();
			bool open(const QString& path, bool set_current = true);
			void write(const QString& );
			void writeAll(void);
			void close(int id);
		private:
			
		private:
			container list;
	};
}

#endif
