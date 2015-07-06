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
#include <qrrepo/logicalRepoApi.h>

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"

namespace qReal {

class LogicalModelAssistInterface: public details::ModelsAssistInterface
{
public:
	/// Returns reference to a constant logical repository.
	virtual const qrRepo::LogicalRepoApi &logicalRepoApi() const = 0;

	/// Returns reference to a mutable logical repository.
	virtual qrRepo::LogicalRepoApi &mutableLogicalRepoApi() = 0;

	virtual Id createElement(const Id &parent, const Id &type) = 0;
	virtual Id createElement(const Id &parent, const Id &id
			, bool isFromLogicalModel, const QString &name
			, const QPointF &position, const Id &preferedLogicalId = Id()) = 0;
	virtual IdList children(const Id &element) const = 0;
	virtual void changeParent(const Id &element, const Id &parent, const QPointF &position = QPointF()) = 0;

	virtual void addExplosion(const Id &source, const Id &destination) = 0;
	virtual void removeExplosion(const Id &source, const Id &destination) = 0;

	virtual void setPropertyByRoleName(const Id &elem, const QVariant &newValue, const QString &roleName) = 0;
	virtual QVariant propertyByRoleName(const Id &elem, const QString &roleName) const = 0;

	virtual bool isLogicalId(const Id &id) const = 0;

	virtual void removeReferencesTo(const Id &id) = 0;
	virtual void removeReferencesFrom(const Id &id) = 0;
	virtual void removeReference(const Id &id, const Id &reference) = 0;
};

}
