#pragma once

#include <QAbstractItemModel>
#include <QVariant>
#include <QStringList>
#include <QMimeData>
#include <QModelIndexList>
#include <QPointF>
#include <QPersistentModelIndex>

#include "../../qrrepo/repoApi.h"
#include "../kernel/definitions.h"
#include "classes/modelTreeItem.h"
#include "../mainwindow/editormanager.h"

namespace qReal {

	namespace model {

		class Model : public QAbstractItemModel
		{
			Q_OBJECT

		public:
			explicit Model(EditorManager const &editorManager, QString const &workingDirectory);
			virtual ~Model();
			QPersistentModelIndex rootIndex() const;
			virtual Qt::ItemFlags flags(const QModelIndex &index) const;
			virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
			virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
			virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
			virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
			virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
			virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
			virtual QModelIndex parent(const QModelIndex &index) const;
			virtual Qt::DropActions supportedDropActions() const;
			virtual QStringList mimeTypes() const;
			virtual QMimeData* mimeData(const QModelIndexList &indexes) const;
			virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
			virtual void changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position);
			qrRepo::RepoApi const &api() const;
			virtual EditorManager const &editorManager() const;

			void open(QString const &workingDirectory);
			void saveTo(QString const &workingDirectory);
		public slots:
			void exterminate();

		protected:
			QMultiHash<Id, details::ModelTreeItem*> mTreeItems;
			QModelIndex index(details::ModelTreeItem const * const item) const;
			details::ModelTreeItem* addElementToModel(details::ModelTreeItem *parentItem, const Id &id,
				const QString &oldPathToItem, const QString &name, const QPointF &position, Qt::DropAction action);

		private:
			qrRepo::RepoApi mApi;
			details::ModelTreeItem *mRootItem;
			EditorManager const &mEditorManager;

			Model(Model const &);  // Копировать модель нельзя
			Model& operator =(Model const &);  // Присваивать тоже

			QString pathToItem(details::ModelTreeItem const * const item) const;
			void removeConfigurationInClient(details::ModelTreeItem const * const item);
			void removeModelItems(details::ModelTreeItem * const root);
			void loadSubtreeFromClient(details::ModelTreeItem * const parent);
			details::ModelTreeItem *loadElement(details::ModelTreeItem *parentItem, const Id &id);
			details::ModelTreeItem *parentTreeItem(QModelIndex const &parent) const;

			QString positionPropertyName(details::ModelTreeItem const *item) const;
			QString configurationPropertyName(details::ModelTreeItem const *item) const;
			QString findPropertyName(Id const &id, int const role) const;
			bool isDiagram(Id const &id) const;

			void init();
			void reinit();
			void cleanupTree(details::ModelTreeItem *root);
			void checkProperties(Id const &id);
		};

	}

}
