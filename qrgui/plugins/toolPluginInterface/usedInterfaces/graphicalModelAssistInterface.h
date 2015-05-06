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

#include <qrkernel/ids.h>
#include <qrrepo/graphicalRepoApi.h>

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"

namespace qReal {

class GraphicalModelAssistInterface : public details::ModelsAssistInterface
{
public:
	virtual const qrRepo::GraphicalRepoApi &graphicalRepoApi() const = 0;
	virtual qrRepo::GraphicalRepoApi &mutableGraphicalRepoApi() const = 0;
	virtual Id createElement(const Id &parent, const Id &type) = 0;
	virtual Id createElement(const Id &parent, const Id &id, bool isFromLogicalModel
		, const QString &name, const QPointF &position
		, const Id &preferedLogicalId = Id()) = 0;
	virtual Id copyElement(const Id &source) = 0;
	virtual IdList children(const Id &element) const = 0;
	virtual void changeParent(const Id &element, const Id &parent, const QPointF &position) = 0;
	virtual void copyProperties(const Id &dest, const Id &src) = 0;
	virtual QMap<QString, QVariant> properties(const Id &id) = 0;
	virtual IdList temporaryRemovedLinksFrom(const Id &elem) const = 0;
	virtual IdList temporaryRemovedLinksTo(const Id &elem) const = 0;
	virtual IdList temporaryRemovedLinksNone(const Id &elem) const = 0;
	virtual void removeTemporaryRemovedLinks(const Id &elem) = 0;

	virtual void setConfiguration(const Id &elem, const QPolygon &newValue) = 0;
	virtual QPolygon configuration(const Id &elem) const = 0;

	virtual void setPosition(const Id &elem, const QPointF &newValue) = 0;
	virtual QPointF position(const Id &elem) const = 0;

	virtual void setToPort(const Id &elem, const qreal &newValue) = 0;
	virtual qreal toPort(const Id &elem) const = 0;

	virtual void setFromPort(const Id &elem, const qreal &newValue) = 0;
	virtual qreal fromPort(const Id &elem) const = 0;

	virtual void setToolTip(const Id &elem, const QString &newValue) = 0;
	virtual QString toolTip(const Id &elem) const = 0;

	virtual Id logicalId(const Id &elem) const = 0;
	virtual IdList graphicalIdsByLogicalId(const Id &logicalId) const = 0;

	virtual bool isGraphicalId(const Id &id) const = 0;
};

}
