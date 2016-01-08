/* Copyright 2013-2016 Dmitry Mordvinov
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

#include <QtWidgets/QTreeWidgetItem>

#include "controller/commands/abstractCommand.h"
#include "controller/commands/trackingEntity.h"
#include "models/exploser.h"

namespace qReal {
namespace commands {

/// This command triggeres palette item editing process for given explosion instance
class QRGUI_MODELS_EXPORT RenameExplosionCommand : public AbstractCommand
{
	Q_OBJECT

public:
	RenameExplosionCommand(models::LogicalModelAssistApi &logicalApi
		, const models::GraphicalModelAssistApi * const graphicalApi
		, const models::Exploser &exploser
		, const Id &target);

	virtual ~RenameExplosionCommand();

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	void ensureLogicalId();

	void promptUserToEnterNewName();

	QTreeWidgetItem *userGroup() const;
	QTreeWidgetItem *paletteItem() const;

	models::LogicalModelAssistApi &mLogicalApi;
	const models::GraphicalModelAssistApi *mGraphicalApi;
	const models::Exploser &mExploser;
	Id mTarget;
	QString mOldName;
	QString mNewName;
};

}
}
