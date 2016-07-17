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

#include <QtCore/QVariant>
#include <QtCore/QModelIndex>

#include "controller/commands/abstractCommand.h"
#include "models/propertyEditorModel.h"
#include "models/logicalModelAssistApi.h"

namespace qReal {
namespace commands {

/// Sets given property to a given value
class QRGUI_MODELS_EXPORT ChangePropertyCommand : public AbstractCommand
{
public:
	/// Use this overload to modify properties via models api with automatic assigning current value as old one.
	ChangePropertyCommand(models::LogicalModelAssistApi * const model
			, const QString &property, const Id &id, const QVariant &newValue);

	/// Use this overload to modify properties via models api with manual old value specification.
	ChangePropertyCommand(models::LogicalModelAssistApi * const model
			, const QString &property, const Id &id, const QVariant &oldValue, const QVariant &newValue);

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	bool setProperty(const QVariant &value);

	models::LogicalModelAssistApi *mLogicalModel;
	const Id mId;
	QString mPropertyName;

	QVariant mOldValue;
	QVariant mNewValue;
};

}
}
