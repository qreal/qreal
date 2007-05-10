#ifndef REALREPOMODEL_H
#define REALREPOMODEL_H

#include <QAbstractItemModel>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QVariant>
#include <QHash>

class RealRepoModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
		RealRepoModel(QSqlDatabase db, QObject *parent = 0);
		~RealRepoModel();

		QModelIndex index(int row, int column,
				const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &child) const;

		QVariant headerData(int section, Qt::Orientation orientation,
				int role = Qt::DisplayRole) const;

		QVariant data(const QModelIndex &index, int role) const;
		bool setData(const QModelIndex & index, const QVariant & value,
				int role = Qt::EditRole );
		
		Qt::ItemFlags flags(const QModelIndex &index) const;

		QStringList mimeTypes () const;
		Qt::DropActions supportedDropActions () const;
		bool dropMimeData(const QMimeData *data, Qt::DropAction action,
				int row, int column, const QModelIndex &parent);
		
		bool removeRows ( int row, int count,
				const QModelIndex & parent = QModelIndex() );
		
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

	private:
		struct RepoTreeItem {
			int id;
			int row;
			RepoTreeItem *parent;
			QList<RepoTreeItem *> children;
		};

		QModelIndex index(const RepoTreeItem *item) const;

		QHash <int, QString> hashNames;
		QHash <int, int> hashTypes;
		QHash <int, QList<int> > hashChildren;
		QHash <int, int> hashChildCount;

		QHash <int,QList<RepoTreeItem *> > hashTreeItems;

		void cleanupTree(RepoTreeItem *root);
		
		void readRootTable();
		void readCategoryTable(RepoTreeItem *root);
		void readContainerTable(RepoTreeItem *root);

		inline bool isElement(RepoTreeItem *root) const
				{ return ( root->id >= 100 ); };
		
		QSqlDatabase db;
		RepoTreeItem *rootItem;

		int bytesCleaned;
};

#endif
