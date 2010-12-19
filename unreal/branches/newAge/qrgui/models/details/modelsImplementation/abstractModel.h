#pragma once

#include <QtCore/QAbstractItemModel>
#include <QMimeData>
#include <QModelIndexList>

#include "../../../qrrepo/repoApi.h"
#include "../../editorManager/editorManager.h"
#include "abstractModelItem.h"

namespace qReal {

	namespace models {

		namespace details {

			namespace modelsImplementation {

				class AbstractModel : public QAbstractItemModel
				{
					Q_OBJECT

				public:
					AbstractModel(EditorManager const &editorManager);
					virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
					virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
					virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
					virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
					virtual QModelIndex parent(const QModelIndex &index) const;
					virtual Qt::ItemFlags flags(QModelIndex const &index) const;
					virtual Qt::DropActions supportedDropActions() const;
					virtual QStringList mimeTypes() const;
					AbstractModelItem * parentAbstractItem(QModelIndex const &parent) const;

					virtual void addElementToModel(Id const &parent, Id const &id, Id const &logicalId, QString const &name, QPointF const &position) = 0;
					QPersistentModelIndex rootIndex() const;
					//void initializeElement(Id const &id, details::AbstractModelItem *parentItem,
					//		details::AbstractModelItem *item, QString const &name, QPointF const &position);
					EditorManager const &editorManager() const;

					QModelIndex indexById(Id const &id) const;
					Id idByIndex(QModelIndex const &index) const;

				protected:
					EditorManager const &mEditorManager;
					QHash<Id, AbstractModelItem *> mModelItems;
					AbstractModelItem *mRootItem;

					QString findPropertyName(Id const &id, int const role) const;
					QModelIndex index(AbstractModelItem const * const item) const;

				private:
					virtual AbstractModelItem *createModelItem(Id const &id, AbstractModelItem *parentItem) const = 0;
				};

			}

		}

	}

}
