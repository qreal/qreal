#pragma once

#include <QtCore/QAbstractItemModel>
#include <QtCore/QHash>

#include "../../../qrrepo/repoApi.h"
#include "../../pluginManager/editorManagerInterface.h"
#include "modelsImplementation/graphicalPartModelItem.h"

namespace qReal {
namespace models {
namespace details {

class GraphicalPartModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	enum Roles {
		positionRole = Qt::UserRole + 1  // element's position within current context
		, configurationRole  // element's configuration (e.g. shape, size) within current context
	};

	GraphicalPartModel(qrRepo::GraphicalRepoApi &repoApi);

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

	QModelIndex addGraphicalPart(Id const &element, int index);

	QModelIndex findIndex(Id const &element, int index);

private:
	qrRepo::GraphicalRepoApi &mRepoApi;
	QHash<Id, QHash<int, modelsImplementation::GraphicalPartModelItem *> > mItems;  // Has ownership.
};

}
}
}
