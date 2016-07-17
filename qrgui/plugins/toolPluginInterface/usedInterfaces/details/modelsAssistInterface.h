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

#include <QtCore/QVariant>
#include <QtCore/QPointF>
#include <QtCore/QModelIndex>
#include <QtCore/QUuid>

#include <qrkernel/ids.h>

namespace qReal {

class EditorManagerInterface;
class ElementInfo;

namespace details {

class ModelsAssistInterface
{
public:
	virtual ~ModelsAssistInterface() {}

	virtual Id createElement(const Id &parent, const Id &type) = 0;
	virtual Id createElement(const Id &parent, const Id &id
		, bool isFromLogicalModel, const QString &name
		, const QPointF &position, const Id &preferedLogicalId = Id()) = 0;
	virtual void createElements(QList<ElementInfo> &elements) = 0;

	virtual Id parent(const Id &element) const = 0;
	virtual IdList children(const Id &element) const = 0;
	virtual void changeParent(const Id &element, const Id &parent, const QPointF &position = QPointF()) = 0;

	virtual void setName(const Id &elem, const QString &newValue) = 0;
	virtual QString name(const Id &elem) const = 0;

	virtual void setTo(const Id &elem, const Id &newValue) = 0;
	virtual Id to(const Id &elem) const = 0;

	virtual void setFrom(const Id &elem, const Id &newValue) = 0;
	virtual Id from(const Id &elem) const = 0;

	virtual QModelIndex indexById(const Id &id) const = 0;
	virtual Id idByIndex(const QModelIndex &index) const = 0;
	virtual QPersistentModelIndex rootIndex() const = 0;
	virtual Id rootId() const = 0;

	virtual bool hasRootDiagrams() const = 0;
	virtual int childrenOfRootDiagram() const = 0;
	virtual int childrenOfDiagram(const Id &parent) const = 0;

	virtual void removeElement(const Id &id) = 0;

	/// Returns reference to Editor Manager object providing information about language metamodel.
	virtual const EditorManagerInterface &editorManagerInterface() const = 0;
};

}
}
