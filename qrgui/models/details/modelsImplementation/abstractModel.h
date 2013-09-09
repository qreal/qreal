#pragma once

#include <QtCore/QAbstractItemModel>
#include <QtCore/QMimeData>
#include <QtCore/QModelIndexList>

#include <qrrepo/repoApi.h>

#include "models/details/modelsAssistApi.h"
#include "pluginManager/editorManagerInterface.h"
#include "models/details/modelsImplementation/abstractModelItem.h"
#include "toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"
#include "models/details/modelsImplementation/modelIndexesInterface.h"

namespace qReal {
namespace models {
namespace details {
namespace modelsImplementation {

class AbstractModel : public QAbstractItemModel, public ModelIndexesInterface
{
	Q_OBJECT

public:
	AbstractModel(EditorManagerInterface const &editorManagerInterface, ConstraintsManager const &constraintsManager);
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &index) const;
	virtual Qt::ItemFlags flags(QModelIndex const &index) const;
	virtual Qt::DropActions supportedDropActions() const;
	virtual QStringList mimeTypes() const;
	virtual qReal::details::ModelsAssistInterface* modelAssistInterface() const = 0;
	bool dropMimeData(QMimeData const *data, Qt::DropAction action, int row, int column, QModelIndex const &parent);

	virtual void addElementToModel(Id const &parent, Id const &id, Id const &logicalId
			, QString const &name, QPointF const &position) = 0;
	QPersistentModelIndex rootIndex() const;
	EditorManagerInterface const &editorManagerInterface() const;
	ConstraintsManager const &constraintsManager() const;

	/// Stacks item element before sibling (they should have the same parent)
	virtual void stackBefore(QModelIndex const &element, QModelIndex const &sibling) = 0;

	QModelIndex indexById(Id const &id) const;
	Id idByIndex(QModelIndex const &index) const;
	Id rootId() const;

	void reinit();

protected:
	EditorManagerInterface const &mEditorManagerInterface;
	ConstraintsManager const &mConstraintsManager;
	QHash<Id, AbstractModelItem *> mModelItems;
	AbstractModelItem *mRootItem;

	QString findPropertyName(Id const &id, int const role) const;
	QModelIndex index(AbstractModelItem const * const item) const;

	void cleanupTree(modelsImplementation::AbstractModelItem * item);

	AbstractModelItem * parentAbstractItem(QModelIndex const &parent) const;
	void removeModelItems(details::modelsImplementation::AbstractModelItem *const root);

private:
	virtual AbstractModelItem *createModelItem(Id const &id, AbstractModelItem *parentItem) const = 0;
	virtual void init() = 0;
	virtual void removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root
			, details::modelsImplementation::AbstractModelItem *child) = 0;
};

}
}
}
}
