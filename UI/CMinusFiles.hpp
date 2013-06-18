#ifndef CMINUSFILES_HPP
#define CMINUSFILES_HPP
#include <QFile>
#include <vector>
#include <QTextDocument>
#include <QAbstractListModel>

namespace cminus {

	struct CMinusFile {
        QString filename; // left empty to create new one
        QTextDocument* doc;
		// bool dirt; // isModified will do the trick
	};

    class CMinusFiles : public QAbstractListModel {
        Q_OBJECT

		public:
            typedef CMinusFile                            value_type;
            typedef std::list<value_type>                 container;
            typedef typename container::iterator          iterator;
            typedef typename container::const_iterator    const_iterator;
        public:
            // reimpl function
            virtual int rowCount(const QModelIndex &parent = QModelIndex()) const {
                return list.size();
            }
            virtual QVariant data(const QModelIndex& index, int Role = Qt::DisplayRole) const;
            virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		public:
            CMinusFiles(QObject * parent = nullptr) :QAbstractListModel(parent) {}
			~CMinusFiles();
            iterator create(void);
            iterator save_new(const QString& name);
            iterator open(const QString& path);
            bool write(iterator iter);
            bool close(iterator iter);
            iterator at(int index);
            bool valid(const_iterator iter) {if (iter == list.end()) return false; return true;}

			void writeAll(void);
			void close(int id);
		private:

            iterator find(const QString fs) {
                return find_if(list.begin(), list.end(), [&](const CMinusFile& f)
                    { if (f.filename == fs) return true;}
				);
			}
            iterator find(const QTextDocument* doc) {
                return find_if(list.begin(), list.end(), [&](const CMinusFile& f)
                    { if ( f.doc == doc) return true;});
			}
		private:
			container list;
	};
}

#endif
