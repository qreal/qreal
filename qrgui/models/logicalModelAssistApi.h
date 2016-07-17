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

#include <qrkernel/ids.h>

#include "qrgui/models/modelsDeclSpec.h"
#include "qrgui/models/details/logicalModel.h"
#include "qrgui/models/details/modelsAssistApi.h"
#include "qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"

namespace qReal {

class EditorManager;
class EditorManagerInterface;

namespace models {

class QRGUI_MODELS_EXPORT LogicalModelAssistApi : public QObject, public qReal::LogicalModelAssistInterface
{
	Q_OBJECT

public:
	LogicalModelAssistApi(details::LogicalModel &logicalModel, const EditorManagerInterface &editorManagerInterface);
	virtual ~LogicalModelAssistApi();

	const EditorManagerInterface &editorManagerInterface() const override;

	const qrRepo::LogicalRepoApi &logicalRepoApi() const override;
	qrRepo::LogicalRepoApi &mutableLogicalRepoApi() override;
	Id createElement(const Id &parent, const Id &type) override;
	Id createElement(const Id &parent, const Id &id, bool isFromLogicalModel, const QString &name
			, const QPointF &position, const Id &preferedLogicalId = Id()) override;
	void createElements(QList<ElementInfo> &elements) override;

	Id parent(const Id &element) const;
	IdList children(const Id &element) const override;
	void changeParent(const Id &element, const Id &parent, const QPointF &position = QPointF()) override;

	void addExplosion(const Id &source, const Id &destination) override;
	void removeExplosion(const Id &source, const Id &destination) override;

	void setPropertyByRoleName(const Id &elem, const QVariant &newValue, const QString &roleName) override;
	QVariant propertyByRoleName(const Id &elem, const QString &roleName) const override;

	bool isLogicalId(const Id &id) const override;

	void removeReferencesTo(const Id &id) override;
	void removeReferencesFrom(const Id &id) override;
	void removeReference(const Id &id, const Id &reference) override;

	void setName(const Id &elem, const QString &newValue) override;
	QString name(const Id &elem) const override;

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

	void removeElement(const Id &logicalId) override;

	/// Returns a mapping of known editors used for current save creation to their versions.
	QMap<Id, Version> editorVersions() const;

signals:
	/// Emitted each time when new element was added into the logical model.
	void elementAdded(const Id &id);

private:
	LogicalModelAssistApi(const LogicalModelAssistApi &);  // Copying is forbidden
	LogicalModelAssistApi& operator =(const LogicalModelAssistApi &); // Assignment is forbidden too

	details::ModelsAssistApi mModelsAssistApi;
	details::LogicalModel &mLogicalModel;
};

}
}
