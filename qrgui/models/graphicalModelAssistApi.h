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

#include <QtCore/QObject>

#include <qrkernel/ids.h>

#include "qrgui/models/modelsDeclSpec.h"
#include "qrgui/models/details/graphicalModel.h"
#include "qrgui/models/details/graphicalPartModel.h"
#include "qrgui/models/details/modelsAssistApi.h"
#include "qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h"

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
			, const EditorManagerInterface &editorManagerInterface
			);

	void setModel(details::GraphicalModel * const graphicalModel);

	const EditorManagerInterface &editorManagerInterface() const override;

	const qrRepo::GraphicalRepoApi &graphicalRepoApi() const override;
	qrRepo::GraphicalRepoApi &mutableGraphicalRepoApi() const override;
	Id createElement(const Id &parent, const Id &type) override;
	Id createElement(const Id &parent, const Id &id, bool isFromLogicalModel, const QString &name
			, const QPointF &position, const Id &preferedLogicalId = Id()) override;
	void createElements(QList<ElementInfo> &elements) override;

	Id copyElement(const Id &source) override;
	Id parent(const Id &element) const override;
	IdList children(const Id &element) const override;
	void changeParent(const Id &element, const Id &parent, const QPointF &position) override;

	void copyProperties(const Id &dest, const Id &src) override;
	QMap<QString, QVariant> properties(const Id &id) override;
	void setProperties(const Id &id, QMap<QString, QVariant> const &properties);

	void stackBefore(const Id &element, const Id &sibling);

	IdList temporaryRemovedLinksFrom(const Id &elem) const override;
	IdList temporaryRemovedLinksTo(const Id &elem) const override;
	IdList temporaryRemovedLinksNone(const Id &elem) const override;
	void removeTemporaryRemovedLinks(const Id &elem) override;

	void setConfiguration(const Id &elem, const QPolygon &newValue);
	QPolygon configuration(const Id &elem) const override;

	void setPosition(const Id &elem, const QPointF &newValue) override;
	QPointF position(const Id &elem) const override;

	void setToPort(const Id &elem, const qreal &newValue);
	qreal toPort(const Id &elem) const override;

	void setFromPort(const Id &elem, const qreal &newValue);
	qreal fromPort(const Id &elem) const override;

	void setName(const Id &elem, const QString &newValue) override;
	QString name(const Id &elem) const override;

	void setToolTip(const Id &elem, const QString &newValue) override;
	QString toolTip(const Id &elem) const override;

	Id logicalId(const Id &elem) const override;
	IdList graphicalIdsByLogicalId(const Id &logicalId) const override;

	bool isGraphicalId(const Id &id) const override;

	void setTo(const Id &elem, const Id &newValue) override;
	Id to(const Id &elem) const override;

	void setFrom(const Id &elem, const Id &newValue) override;
	Id from(const Id &elem) const override;

	QModelIndex indexById(const Id &id) const override;
	Id idByIndex(const QModelIndex &index) const override;
	QPersistentModelIndex rootIndex() const override;
	Id rootId() const override;

	bool hasRootDiagrams() const override;
	int childrenOfRootDiagram() const override;
	int childrenOfDiagram(const Id &parent) const override;

	void removeElement(const Id &graphicalId) override;

	/// Returns true, if a label already exists in repository.
	/// @param graphicalId - id of an element.
	/// @param index - index of a part, which uniquely identifies label in an element.
	bool hasLabel(const Id &graphicalId, int index);

	/// Creates model representation of text label on element.
	/// @param graphicalId - id of an element on which a label shall be created. It shall already be in repository.
	/// @param index - index of a part, which uniquely identifies label in an element.
	/// @param position - initial position of a label in coordinates of an element.
	/// @param size - initial size of a label.
	void createLabel(const Id &graphicalId, int index, const QPointF &position, const QSizeF &size);

	/// Sets label position.
	/// @param graphicalId - id of an element to which label belongs.
	/// @param index - index of a part, which uniquely identifies label in an element.
	/// @param position - position of a label in coordinates of an element.
	void setLabelPosition(const Id &graphicalId, int index, const QPointF &position);

	/// Sets label size.
	/// @param graphicalId - id of an element to which label belongs.
	/// @param index - index of a part, which uniquely identifies label in an element.
	/// @param size - size of a label.
	void setLabelSize(const Id &graphicalId, int index, const QSizeF &size);

	/// Returns label position.
	/// @param graphicalId - id of an element to which label belongs.
	/// @param index - index of a part, which uniquely identifies label in an element.
	/// @returns label position in coordinates of an element.
	QPointF labelPosition(const Id &graphicalId, int index) const;

	/// Returns label size.
	/// @param graphicalId - id of an element to which label belongs.
	/// @param index - index of a part, which uniquely identifies label in an element.
	QSizeF labelSize(const Id &graphicalId, int index) const;

signals:
	void nameChanged(const Id &id);

	/// Emitted each time when new element was added into the graphical model.
	void elementAdded(const Id &id);

private:
	GraphicalModelAssistApi(const GraphicalModelAssistApi &);
	GraphicalModelAssistApi& operator =(const GraphicalModelAssistApi &);

	details::GraphicalModel &mGraphicalModel;
	details::ModelsAssistApi mModelsAssistApi;
	details::GraphicalPartModel &mGraphicalPartModel;
};

}

}
