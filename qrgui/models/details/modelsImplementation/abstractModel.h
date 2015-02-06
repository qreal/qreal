#pragma once

#include <QtCore/QAbstractItemModel>
#include <QtCore/QMimeData>
#include <QtCore/QModelIndexList>

#include <qrrepo/repoApi.h>

#include "models/modelsDeclSpec.h"
#include "models/details/modelsAssistApi.h"
#include "plugins/pluginManager/editorManagerInterface.h"
#include "models/details/modelsImplementation/abstractModelItem.h"
#include "plugins/toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"
#include "models/details/modelsImplementation/modelIndexesInterface.h"

namespace qReal {
namespace models {
namespace details {
namespace modelsImplementation {

class QRGUI_MODELS_EXPORT AbstractModel : public QAbstractItemModel, public ModelIndexesInterface
{
	Q_OBJECT

public:
	AbstractModel(const EditorManagerInterface &editorManagerInterface);
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

	virtual void addElementToModel(const Id &parent, const Id &id, const Id &logicalId
			, const QString &name, const QPointF &position) = 0;
	QPersistentModelIndex rootIndex() const;
	const EditorManagerInterface &editorManagerInterface() const;

	/// Stacks item element before sibling (they should have the same parent)
	virtual void stackBefore(QModelIndex const &element, QModelIndex const &sibling) = 0;

	QModelIndex indexById(const Id &id) const;
	Id idByIndex(QModelIndex const &index) const;
	Id rootId() const;

	void reinit();

signals:
	/// Emitted each time when new element was added into model.
	void elementAdded(const Id &id);

protected:
	const EditorManagerInterface &mEditorManagerInterface;
	QHash<Id, AbstractModelItem *> mModelItems;
	AbstractModelItem *mRootItem;

	QString findPropertyName(const Id &id, const int role) const;
	QModelIndex index(AbstractModelItem const * const item) const;

	void cleanupTree(modelsImplementation::AbstractModelItem * item);

	AbstractModelItem * parentAbstractItem(QModelIndex const &parent) const;
	void removeModelItems(details::modelsImplementation::AbstractModelItem *const root);

private:
	virtual AbstractModelItem *createModelItem(const Id &id, AbstractModelItem *parentItem) const = 0;
	virtual void init() = 0;
	virtual void removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root
			, details::modelsImplementation::AbstractModelItem *child) = 0;
};

}
}
}
}
