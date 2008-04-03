#ifndef REALREPOMODEL_H
#define REALREPOMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>
#include <QtCore/QHash>
#include <QtCore/QPoint>

#include <QtGui/QPolygon>

#include "realrepoinfo.h"

#include "realrepoclient.h"

#include "../common/classes.h"

class RealRepoModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
		RealRepoModel(QObject *parent = 0);
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

		void beginTransaction();
		void commitTransaction();
		void rollbackTransaction();

		int getLastError() { return m_error; }

		int getState() { return repoClient->state(); }

	private:
		RealRepoClient *repoClient;

		struct RepoTreeItem {
			int id;
			int row;
			RepoTreeItem *parent;
			QList<RepoTreeItem *> children;
		};

		enum ElementType { Root, Category, Container };

		QModelIndex index(const RepoTreeItem *item) const;

		QHash <int, QString> hashNames;
		QHash <int, int> hashTypes;
		QHash <int, QList<int> > hashChildren;
		QHash <int, int> hashChildCount;

		QHash <int, QMap<int, QVariant> > hashElementProps;

		QHash <int,QList<RepoTreeItem *> > hashTreeItems;

		struct ElementOnDiagram {
			QPoint position;
			QPolygon configuration;
		};

		QHash <int, QMap<int, ElementOnDiagram> > hashDiagramElements;

		void cleanupTree(RepoTreeItem *root);

		void updateProperties(int id);

		void updateRootTable();

		void createItem(RepoTreeItem *, int type, int id);
		
		void readRootTable();
		void readCategoryTable(RepoTreeItem *root);
		void readContainerTable(RepoTreeItem *root);

		ElementType type(const RepoTreeItem *item) const;
		ElementType type(const QModelIndex &index) const;

		RepoTreeItem *rootItem;

		RealRepoInfo info;

		int m_error;
};

#endif
