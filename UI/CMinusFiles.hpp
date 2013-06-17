#ifndef CMINUSFILES_HPP
#define CMINUSFILES_HPP
#include <QFile>
#include <vector>
#include <QTextDocument>

namespace cminus {

	struct CMinusFile {
        QFile* fs;
        QTextDocument* doc;
		// bool dirt; // isModified will do the trick
	};

	class CMinusFiles {
		public:
            typedef CMinusFile                            value_type;
            typedef std::list<value_type>                 container;
            typedef typename container::iterator          iterator;
            typedef typename container::const_iterator    const_iterator;
		public:
			CMinusFiles() {}
			~CMinusFiles();
			bool open(const QString& path, bool set_current = true);
            bool write(const QTextDocument* doc);
            iterator current() {return current_;}

			void writeAll(void);
			void close(int id);
		private:

            iterator find(const QFile* fs) {
                return find_if(list.begin(), list.end(), [&](const CMinusFile& f)
                    { if (f.fs == fs) return true;}
				);
			}
            iterator find(const QTextDocument* doc) {
                return find_if(list.begin(), list.end(), [&](const CMinusFile& f)
                    { if ( f.doc == doc) return true;});
			}
		private:
			container list;
            iterator current_;
	};
}

#endif
