#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>

#include "models/details/graphicalModel.h"
#include "models/details/graphicalPartModel.h"
#include "models/details/modelsAssistApi.h"
#include "toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h"

namespace qReal {

class EditorManagerInterface;
class ConstraintsManager;

namespace models {

namespace details {
class GraphicalModel;
}

class GraphicalModelAssistApi : public QObject, public GraphicalModelAssistInterface
{
	Q_OBJECT

public:
	GraphicalModelAssistApi(
			details::GraphicalModel &graphicalModel
			, details::GraphicalPartModel &graphicalPartModel
			, EditorManagerInterface const &editorManagerInterface
			, ConstraintsManager const &constraintsManager
			);

	void setModel(details::GraphicalModel * const graphicalModel);

	/// Interface for accessing metamodel information.
	EditorManagerInterface const &editorManagerInterface() const;

	ConstraintsManager const &constraintsManager() const;
	qrRepo::GraphicalRepoApi const &graphicalRepoApi() const;
	qrRepo::GraphicalRepoApi &mutableGraphicalRepoApi() const;
	Id createElement(Id const &parent, Id const &type);
	Id createElement(Id const &parent, Id const &id, bool isFromLogicalModel
			, QString const &name, QPointF const &position
			, Id const &preferedLogicalId = Id());
	Id copyElement(Id const &source);
	IdList children(Id const &element) const;
	void changeParent(Id const &element, Id const &parent, QPointF const &position);

	void copyProperties(Id const &dest, Id const &src);
	QMap<QString, QVariant> properties(Id const &id);
	void setProperties(Id const &id, QMap<QString, QVariant> const &properties);

	virtual void stackBefore(Id const &element, Id const &sibling);

	IdList temporaryRemovedLinksFrom(Id const &elem) const;
	IdList temporaryRemovedLinksTo(Id const &elem) const;
	IdList temporaryRemovedLinksNone(Id const &elem) const;
	void removeTemporaryRemovedLinks(Id const &elem);

	void setConfiguration(Id const &elem, QPolygon const &newValue);
	QPolygon configuration(Id const &elem) const;

	void setPosition(Id const &elem, QPointF const &newValue);
	QPointF position(Id const &elem) const;

	void setToPort(Id const &elem, qreal const &newValue);
	qreal toPort(Id const &elem) const;

	void setFromPort(Id const &elem, qreal const &newValue);
	qreal fromPort(Id const &elem) const;

	void setName(Id const &elem, QString const &newValue);
	QString name(Id const &elem) const;

	void setToolTip(Id const &elem, QString const &newValue);
	QString toolTip(Id const &elem) const;

	Id logicalId(Id const &elem) const;
	IdList graphicalIdsByLogicalId(Id const &logicalId) const;

	bool isGraphicalId(Id const &id) const;

	void setTo(Id const &elem, Id const &newValue);
	Id to(Id const &elem) const;

	void setFrom(Id const &elem, Id const &newValue);
	Id from(Id const &elem) const;

	QModelIndex indexById(Id const &id) const;
	Id idByIndex(QModelIndex const &index) const;
	QPersistentModelIndex rootIndex() const;
	Id rootId() const;

	bool hasRootDiagrams() const;
	int childrenOfRootDiagram() const;
	int childrenOfDiagram(const Id &parent) const;
	void removeElement(Id const &graphicalId);

	/// Returns true, if a label already exists in repository.
	/// @param graphicalId - id of an element.
	/// @param index - index of a part, which uniquely identifies label in an element.
	bool hasLabel(Id const &graphicalId, int index);

	/// Creates model representation of text label on element.
	/// @param graphicalId - id of an element on which a label shall be created. It shall already be in repository.
	/// @param index - index of a part, which uniquely identifies label in an element.
	/// @param position - initial position of a label in coordinates of an element.
	/// @param size - initial size of a label.
	void createLabel(Id const &graphicalId, int index, QPointF const &position, QSizeF const &size);

	/// Sets label position.
	/// @param graphicalId - id of an element to which label belongs.
	/// @param index - index of a part, which uniquely identifies label in an element.
	/// @param position - position of a label in coordinates of an element.
	void setLabelPosition(Id const &graphicalId, int index, QPointF const &position);

	/// Sets label size.
	/// @param graphicalId - id of an element to which label belongs.
	/// @param index - index of a part, which uniquely identifies label in an element.
	/// @param size - size of a label.
	void setLabelSize(Id const &graphicalId, int index, QSizeF const &size);

	/// Returns label position.
	/// @param graphicalId - id of an element to which label belongs.
	/// @param index - index of a part, which uniquely identifies label in an element.
	/// @returns label position in coordinates of an element.
	QPointF labelPosition(Id const &graphicalId, int index) const;

	/// Returns label size.
	/// @param graphicalId - id of an element to which label belongs.
	/// @param index - index of a part, which uniquely identifies label in an element.
	QSizeF labelSize(Id const &graphicalId, int index) const;

public slots:
	void nameChangedSlot(Id const &element);

signals:
	void nameChanged(Id const &id);

private:
	GraphicalModelAssistApi(GraphicalModelAssistApi const &);
	GraphicalModelAssistApi& operator =(GraphicalModelAssistApi const &);

	details::GraphicalModel &mGraphicalModel;
	details::ModelsAssistApi mModelsAssistApi;
	details::GraphicalPartModel &mGraphicalPartModel;
};

}

}
