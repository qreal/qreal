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

#include <generatorBase/robotsGeneratorPluginBase.h>

#include "trikGeneratorBase/trikGeneratorBaseDeclSpec.h"

namespace trik {

namespace robotModel {
class TrikGeneratorRobotModel;
}

namespace blocks {
class TrikBlocksFactory;
}

/// A base class for every generator from the TRIK kit.
class ROBOTS_TRIK_GENERATOR_BASE_EXPORT TrikGeneratorPluginBase : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT

public:
	TrikGeneratorPluginBase(const QString &robotName, const QString &robotFriendlyName, int priority);
	~TrikGeneratorPluginBase();

	QString kitId() const override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;
	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

protected:
	void regenerateExtraFiles(const QFileInfo &newFileInfo) override;

	QScopedPointer<robotModel::TrikGeneratorRobotModel> mRobotModel;

private:
	blocks::TrikBlocksFactory *mBlocksFactory;  // Transfers ownership
};

}
