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

#include <QtCore/QMap>
#include <QtXml/QDomDocument>

#include <qrgui/controller/commands/abstractCommand.h>
#include <qrgui/controller/commands/trackingEntity.h>

namespace graphicsUtils {
class AbstractScene;
class AbstractItem;
}

namespace twoDModel {

namespace model {
class Model;
}

namespace commands {

/// Tracks item`s shape (position, size, rotation, etc.), undoes and redoes its changes.
class ReshapeCommand : public qReal::commands::AbstractCommand, public qReal::commands::TrackingEntity
{
public:
	ReshapeCommand(graphicsUtils::AbstractScene &scene, const model::Model &model, const QStringList &ids);

	void startTracking() override;
	void stopTracking() override;

	/// Returns true if start and finish item`s configurations are different.
	bool modificationsHappened() const;

protected:
	bool execute();
	bool restoreState();

private:
	void takeSnapshot(QMap<QString, QDomElement> &target);
	void setConfiguration(const QMap<QString, QDomElement> &configuration);

	graphicsUtils::AbstractScene &mScene;
	const model::Model &mModel;
	const QStringList mIds;
	QDomDocument mXmlFactory;
	QMap<QString, QDomElement> mOldConfiguration;
	QMap<QString, QDomElement> mNewConfiguration;
};

}
}
