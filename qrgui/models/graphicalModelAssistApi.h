#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>

#include "models/modelsDeclSpec.h"
#include "models/details/graphicalModel.h"
#include "models/details/graphicalPartModel.h"
#include "models/details/modelsAssistApi.h"
#include "plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h"

namespace qReal {

class EditorManagerInterface;

namespace models {

namespace details {
class GraphicalModel;
}

class QRGUI_MODELS_EXPORT GraphicalModelAssistApi : public QObject, public GraphicalModelAssistInterface
{
	Q_OBJECT

public:
	GraphicalModelAssistApi(
			details::GraphicalModel &graphicalModel
			, details::GraphicalPartModel &graphicalPartModel
			, EditorManagerInterface const &editorManagerInterface
			);

	void setModel(details::GraphicalModel * const graphicalModel);

	const EditorManagerInterface &editorManagerInterface() const override;

	qrRepo::GraphicalRepoApi const &graphicalRepoApi() const override;
	qrRepo::GraphicalRepoApi &mutableGraphicalRepoApi() const override;
	Id createElement(Id const &parent, Id const &type) override;
	Id createElement(Id const &parent, Id const &id, bool isFromLogicalModel, QString const &name
			, QPointF const &position, Id const &preferedLogicalId = Id()) override;
	Id copyElement(Id const &source) override;
	IdList children(Id const &element) const override;
	void changeParent(Id const &element, Id const &parent, QPointF const &position) override;

	void copyProperties(Id const &dest, Id const &src) override;
	QMap<QString, QVariant> properties(Id const &id) override;
	void setProperties(Id const &id, QMap<QString, QVariant> const &properties);

	void stackBefore(Id const &element, Id const &sibling);

	IdList temporaryRemovedLinksFrom(Id const &elem) const override;
	IdList temporaryRemovedLinksTo(Id const &elem) const override;
	IdList temporaryRemovedLinksNone(Id const &elem) const override;
	void removeTemporaryRemovedLinks(Id const &elem) override;

	void setConfiguration(Id const &elem, QPolygon const &newValue);
	QPolygon configuration(Id const &elem) const override;

	void setPosition(Id const &elem, QPointF const &newValue) override;
	QPointF position(Id const &elem) const override;

	void setToPort(Id const &elem, qreal const &newValue);
	qreal toPort(Id const &elem) const override;

	void setFromPort(Id const &elem, qreal const &newValue);
	qreal fromPort(Id const &elem) const override;

	void setName(Id const &elem, QString const &newValue) override;
	QString name(Id const &elem) const override;

	void setToolTip(Id const &elem, QString const &newValue) override;
	QString toolTip(Id const &elem) const override;

	Id logicalId(Id const &elem) const override;
	IdList graphicalIdsByLogicalId(Id const &logicalId) const override;

	bool isGraphicalId(Id const &id) const override;

	void setTo(Id const &elem, Id const &newValue) override;
	Id to(Id const &elem) const override;

	void setFrom(Id const &elem, Id const &newValue) override;
	Id from(Id const &elem) const override;

	QModelIndex indexById(Id const &id) const override;
	Id idByIndex(QModelIndex const &index) const override;
	QPersistentModelIndex rootIndex() const override;
	Id rootId() const override;

	bool hasRootDiagrams() const override;
	int childrenOfRootDiagram() const override;
	int childrenOfDiagram(const Id &parent) const override;

	void removeElement(Id const &graphicalId) override;

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

signals:
	void nameChanged(Id const &id);

	/// Emitted each time when new element was added into the graphical model.
	void elementAdded(Id const &id);

private:
	GraphicalModelAssistApi(GraphicalModelAssistApi const &);
	GraphicalModelAssistApi& operator =(GraphicalModelAssistApi const &);

	details::GraphicalModel &mGraphicalModel;
	details::ModelsAssistApi mModelsAssistApi;
	details::GraphicalPartModel &mGraphicalPartModel;
};

}

}
