#pragma once

#include <QtCore/QAbstractItemModel>

#include "../../../qrrepo/repoApi.h"
#include "../../editorManager/editorManager.h"
#include "abstractModelItem.h"

namespace qReal {

	namespace models {

		namespace details {

			class AbstractModel : public QAbstractItemModel
			{
				Q_OBJECT
			public:
				AbstractModel(qrRepo::RepoApi &repoApi, EditorManager const &editorManager);
				virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
				virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
				virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
				virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
				virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
				virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
				virtual QModelIndex parent(const QModelIndex &index) const;
				virtual Qt::ItemFlags flags(QModelIndex const &index) const;

				void addElementToModel(Id const &parent, Id const &id, QString const &name, QPointF const &position);
				QPersistentModelIndex rootIndex() const;

			protected:
				qrRepo::RepoApi &mApi;
				EditorManager const &mEditorManager;
				QHash<Id, details::AbstractModelItem *> mModelItems;
				details::AbstractModelItem *mRootItem;

				QString findPropertyName(Id const &id, int const role) const;
				QModelIndex index(details::AbstractModelItem const * const item) const;
				bool mNotNeedUpdate;

			private:
				virtual AbstractModelItem *createModelItem(Id const &id, AbstractModelItem *parentItem) const = 0;

				details::AbstractModelItem *parentTreeItem(QModelIndex const &parent) const;
			};

		}

	}

}
