#pragma once

#include <generatorBase/robotsGeneratorPluginBase.h>

#include "nxtGeneratorBase/nxtGeneratorBaseDeclSpec.h"

namespace nxt {

namespace robotModel {
class NxtGeneratorRobotModel;
}

namespace blocks {
class NxtBlocksFactory;
}

/// A base class for every generator from the NXT kit.
class ROBOTS_NXT_GENERATOR_BASE_EXPORT NxtGeneratorPluginBase : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT

public:
	NxtGeneratorPluginBase(const QString &robotName, const QString &robotFriendlyName, int priority);
	~NxtGeneratorPluginBase();

	QString kitId() const override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;
	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

private:
	QScopedPointer<robotModel::NxtGeneratorRobotModel> mRobotModel;
	blocks::NxtBlocksFactory *mBlocksFactory;  // Transfers ownership
};

}
