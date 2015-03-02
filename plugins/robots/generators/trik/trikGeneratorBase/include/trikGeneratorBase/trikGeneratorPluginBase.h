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
	TrikGeneratorPluginBase(const QString &robotName, const QString &robotFriendlyName);
	~TrikGeneratorPluginBase();

	QString kitId() const override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;
	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

protected:
	void regenerateExtraFiles(const QFileInfo &newFileInfo) override;

private:
	QScopedPointer<robotModel::TrikGeneratorRobotModel> mRobotModel;
	blocks::TrikBlocksFactory *mBlocksFactory;  // Transfers ownership
};

}
