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
#include <QtCore/QList>
#include <QtCore/QHash>

#include <qrrepo/repoApi.h>

#include "qrgui/plugins/pluginManager/editorManagerInterface.h"
#include "qrgui/models/details/modelsImplementation/graphicalPartModelItem.h"
#include "qrgui/models/details/modelsImplementation/modelIndexesInterface.h"

namespace qReal {
namespace models {
namespace details {

/// A model that contains separate parts of graphical elements, like labels or pins.
/// Model organized as a list of graphical elements, each cell of which contains a list of graphical parts.
/// It allows to freely add graphical parts to new or existing elements (elements always shall already be in a repo),
/// or to remove elements as a whole, but only from model --- GraphicalPartModel does not own elements, and graphical
/// parts can not be removed separately.
class GraphicalPartModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	/// Roles specific to graphical parts.
	enum Roles {
		positionRole = Qt::UserRole + 1  // element's position within current context
		, configurationRole  // element's configuration (e.g. shape, size) within current context
	};

	/// Constructor.
	/// @param repoApi - reference to repository API.
	GraphicalPartModel(qrRepo::GraphicalRepoApi &repoApi
			, const modelsImplementation::ModelIndexesInterface &graphicalModel);

	~GraphicalPartModel() override;

	QVariant data(const QModelIndex &index, int role) const override;

	bool setData(const QModelIndex &index, const QVariant &value, int role) override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

	QModelIndex parent(const QModelIndex &index) const override;

	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

	/// Adds new graphical part to model and repository.
	/// @param element - id of an element to which a part shall be added. It shall already exist in repository,
	///        but not nesessarily in a model.
	/// @param index - index of a part, which uniquely identifies it in an element.
	/// @returns model index of newly added part.
	QModelIndex addGraphicalPart(const Id &element, int index);

	/// Returns index by given id of an element and index of a part within it.
	QModelIndex findIndex(const Id &element, int index) const;

	/// Deletes model contents and reloads it from repo.
	void reinit();

public slots:
	virtual void rowsAboutToBeRemovedInGraphicalModel(const QModelIndex &parent, int start, int end);

private:
	void clear();
	void load(const Id &parent = Id::rootId());
	void loadElement(const Id &id);
	QModelIndex addGraphicalPart(const Id &element, int index, bool addToRepo);

	qrRepo::GraphicalRepoApi &mRepoApi;
	QList<QList<modelsImplementation::GraphicalPartModelItem *> > mItems;  // Has ownership.

	/// Maps id to an index of this id in mItems list.
	QHash<Id, int> mIdPositions;
	const modelsImplementation::ModelIndexesInterface &mGraphicalModel;
};

}
}
}
