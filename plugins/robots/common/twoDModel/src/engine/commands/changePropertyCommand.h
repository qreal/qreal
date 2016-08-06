/* Copyright 2016 CyberTech Labs Ltd.
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

#include <qrgui/controller/commands/abstractCommand.h>

namespace graphicsUtils {
class AbstractScene;
class AbstractItem;
}

namespace twoDModel {

namespace model {
class Model;
}

namespace commands {

/// Sets property for a group of items on 2D model scene.
class ChangePropertyCommand : public qReal::commands::AbstractCommand
{
public:
	/// Use this overload to modify properties via models api
	ChangePropertyCommand(const graphicsUtils::AbstractScene &scene, const model::Model &model
			, const QStringList &ids, const QString &property, const QVariant &value);

protected:
	bool execute() override;
	bool restoreState() override;

private:
	bool setProperties(const QMap<QString, QVariant> &values);

	const graphicsUtils::AbstractScene &mScene;
	const model::Model &mModel;
	const QStringList mIds;
	QString mPropertyName;

	QMap<QString, QVariant> mOldValues;
	QMap<QString, QVariant> mNewValues;
};

}
}
