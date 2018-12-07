/* Copyright 2007-2016 QReal Research Group
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

#include <qrrepo/repoApi.h>

#include "qrgui/plugins/pluginManager/editorManagerInterface.h"
#include "modelsImplementation/graphicalModelItem.h"
#include "modelsImplementation/abstractModel.h"
#include "logicalModelView.h"
#include "qrgui/models/graphicalModelAssistApi.h"

namespace qReal {

namespace models {
class GraphicalModelAssistApi;

namespace details {

class LogicalModel;

class QRGUI_MODELS_EXPORT GraphicalModel : public modelsImplementation::AbstractModel
{
	Q_OBJECT

public:
	GraphicalModel(qrRepo::GraphicalRepoApi *repoApi
			, const EditorManagerInterface &editorManagerInterface);

	virtual ~GraphicalModel() override;

	/// Sets graphical assist interface for this model. Takes ownership.
	void setAssistApi(GraphicalModelAssistApi * const graphicalAssistApi);

	void connectToLogicalModel(LogicalModel * const logicalModel);
	void updateElements(const Id &logicalId, const QString &name);
	void addElementToModel(ElementInfo &elementInfo) override;
	void addElementsToModel(QList<ElementInfo> &elementsInfo) override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
	virtual void changeParent(const QModelIndex &element, const QModelIndex &parent, const QPointF &position);
	const qrRepo::GraphicalRepoApi &api() const;
	qrRepo::GraphicalRepoApi &mutableApi() const;
	virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	QList<QPersistentModelIndex> indexesWithLogicalId(const Id &logicalId) const;
	virtual qReal::details::ModelsAssistInterface* modelAssistInterface() const override;
	GraphicalModelAssistApi &graphicalModelAssistApi() const;
	virtual void stackBefore(const QModelIndex &element, const QModelIndex &sibling) override;

signals:
	void nameChanged(const Id &id);

private:
	LogicalModelView mLogicalModelView;
	qrRepo::GraphicalRepoApi &mApi;
	GraphicalModelAssistApi *mGraphicalAssistApi;  // Has ownership.

	virtual void init() override;
	void loadSubtreeFromClient(modelsImplementation::GraphicalModelItem * const parent);
	modelsImplementation::GraphicalModelItem *loadElement(modelsImplementation::GraphicalModelItem *parentItem
			, const Id &id);

	void setNewName(const Id &id, const QString newValue);
	virtual modelsImplementation::AbstractModelItem *createModelItem(const Id &id
			, modelsImplementation::AbstractModelItem *parentItem) const override;
	void addTree(const Id &parent, const QMultiMap<Id, ElementInfo *> &childrenOfParents, QSet<Id> &visited);
	/// Adds entries to row model without inserting rows and notifying about that connected views.
	/// @returns created model item.
	modelsImplementation::AbstractModelItem *createElementWithoutCommit(ElementInfo &elementInfo
			, modelsImplementation::AbstractModelItem *parentItem);
	void initializeElement(const ElementInfo &elementInfo
			, modelsImplementation::AbstractModelItem *parentItem
			, modelsImplementation::AbstractModelItem *item);
	virtual void removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root
			, details::modelsImplementation::AbstractModelItem *child) override;
};
}
}
}
