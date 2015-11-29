/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QAbstractItemModel>
#include <QtCore/QMimeData>
#include <QtCore/QModelIndexList>

#include <qrrepo/repoApi.h>

#include "qrgui/models/elementInfo.h"
#include "qrgui/models/details/modelsAssistApi.h"
#include "qrgui/plugins/pluginManager/editorManagerInterface.h"
#include "qrgui/models/details/modelsImplementation/abstractModelItem.h"
#include "qrgui/plugins/toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"
#include "qrgui/models/details/modelsImplementation/modelIndexesInterface.h"

#include "qrgui/models/modelsDeclSpec.h"

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
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual Qt::DropActions supportedDropActions() const;
	virtual QStringList mimeTypes() const;
	virtual qReal::details::ModelsAssistInterface* modelAssistInterface() const = 0;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

	/// Creates element described by the given parameters set.
	/// @param elementInfo Element properties container describing element that must be created.
	/// @warning logicalId entry may be replaced with actual one if empty or incorrect id is provided.
	virtual void addElementToModel(const ElementInfo &elementInfo) = 0;

	/// Creates elements set described by the given parameters list. Elements will be created in one bunch,
	/// causing rowsInserted with multiple rows in performance considerations. This is much more efficient way
	/// to create group of elements than calling addElementToModel() multiple times.
	/// @param elementsInfo Element properties containers list  describing element that must be created.
	virtual void addElementsToModel(const QList<ElementInfo> &elementsInfo) = 0;

	QPersistentModelIndex rootIndex() const;
	const EditorManagerInterface &editorManagerInterface() const;

	/// Stacks item element before sibling (they should have the same parent)
	virtual void stackBefore(const QModelIndex &element, const QModelIndex &sibling) = 0;

	QModelIndex indexById(const Id &id) const;
	Id idByIndex(const QModelIndex &index) const;
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
	QModelIndex index(const AbstractModelItem * const item) const;

	void cleanupTree(modelsImplementation::AbstractModelItem * item);

	AbstractModelItem * parentAbstractItem(const QModelIndex &parent) const;
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
