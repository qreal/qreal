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

#include "models/exploser.h"

namespace qReal {
namespace commands {

class QRGUI_MODELS_EXPORT CreateRemoveCommandImplementation
{
public:
	CreateRemoveCommandImplementation(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, const models::Exploser &exploser
			, const Id &logicalParent
			, const Id &graphicalParent
			, const Id &id
			, bool isFromLogicalModel
			, const QString &name
			, const QPointF &position);

	Id create();
	void remove();

	Id id() const;

	bool equals(const CreateRemoveCommandImplementation &other) const;

	/// @todo: Bad method, required only for linkers. Get rid of it.
	/// Modifies command setting new creation position.
	void setNewPosition(const QPointF &position);

private:
	void refreshAllPalettes();

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	const models::Exploser &mExploser;
	const Id mLogicalParent;
	const Id mGraphicalParent;
	Id mId;
	const bool mIsFromLogicalModel;
	const QString mName;
	QPointF mPosition;
	QMap<QString, QVariant> mLogicalPropertiesSnapshot;
	QMap<QString, QVariant> mGraphicalPropertiesSnapshot;
	Id mOldLogicalId;
};

inline bool operator==(const CreateRemoveCommandImplementation &i1
		, const CreateRemoveCommandImplementation &i2)
{
	return i1.equals(i2);
}

}
}
