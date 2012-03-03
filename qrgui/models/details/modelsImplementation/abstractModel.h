#pragma once

#include <QtCore/QAbstractItemModel>
#include <QMimeData>
#include <QModelIndexList>

#include "../modelsAssistApi.h"
#include "../../../../qrrepo/repoApi.h"
#include "../../../pluginManager/editorManager.h"
#include "abstractModelItem.h"
#include "../../../toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"

namespace qReal {

namespace models {

namespace details {

namespace modelsImplementation {

class AbstractModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	AbstractModel(InterpreterEditorManager const &editorManager);
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

	virtual void addElementToModel(Id const &parent, Id const &id, Id const &logicalId, QString const &name, QPointF const &position) = 0;
	QPersistentModelIndex rootIndex() const;
	//void initializeElement(Id const &id, details::AbstractModelItem *parentItem,
	//		details::AbstractModelItem *item, QString const &name, QPointF const &position);
	EditorManagerInterface const &editorManager() const;

	/// Stacks item element before sibling (they should have the same parent)
	virtual void stackBefore(QModelIndex const &element, QModelIndex const &sibling) = 0;

	QModelIndex indexById(Id const &id) const;
	Id idByIndex(QModelIndex const &index) const;

	void reinit();

protected:
	InterpreterEditorManager const &mEditorManager;
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
	virtual void removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root, details::modelsImplementation::AbstractModelItem *child) = 0;
};

}

}

}

}
