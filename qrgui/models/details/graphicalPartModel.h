#pragma once

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>
#include <QtCore/QHash>

#include <qrrepo/repoApi.h>

#include "plugins/pluginManager/editorManagerInterface.h"
#include "models/details/modelsImplementation/graphicalPartModelItem.h"
#include "models/details/modelsImplementation/modelIndexesInterface.h"

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
			, modelsImplementation::ModelIndexesInterface const &graphicalModel);

	virtual ~GraphicalPartModel();

	// Override.
	virtual QVariant data(QModelIndex const &index, int role) const;

	// Override.
	virtual bool setData(QModelIndex const &index, QVariant const &value, int role);

	// Override.
	virtual int rowCount(QModelIndex const &parent = QModelIndex()) const;

	// Override.
	virtual int columnCount(QModelIndex const &parent = QModelIndex()) const;

	// Override.
	virtual QModelIndex index(int row, int column, QModelIndex const &parent = QModelIndex()) const;

	// Override.
	virtual QModelIndex parent(QModelIndex const &index) const;

	// Override.
	virtual bool removeRows(int row, int count, QModelIndex const &parent = QModelIndex());

	/// Adds new graphical part to model and repository.
	/// @param element - id of an element to which a part shall be added. It shall already exist in repository,
	///        but not nesessarily in a model.
	/// @param index - index of a part, which uniquely identifies it in an element.
	/// @returns model index of newly added part.
	QModelIndex addGraphicalPart(Id const &element, int index);

	/// Returns index by given id of an element and index of a part within it.
	QModelIndex findIndex(Id const &element, int index) const;

	/// Deletes model contents and reloads it from repo.
	void reinit();

public slots:
	virtual void rowsAboutToBeRemovedInGraphicalModel(QModelIndex const &parent, int start, int end);

private:
	void clear();
	void load(Id const &parent = Id::rootId());
	void loadElement(Id const &id);
	QModelIndex addGraphicalPart(Id const &element, int index, bool addToRepo);

	qrRepo::GraphicalRepoApi &mRepoApi;
	QList<QList<modelsImplementation::GraphicalPartModelItem *> > mItems;  // Has ownership.

	/// Maps id to an index of this id in mItems list.
	QHash<Id, int> mIdPositions;
	modelsImplementation::ModelIndexesInterface const &mGraphicalModel;
};

}
}
}
