#pragma once

#include <QtCore/QObject>

#include <interpreterBase/kitPluginInterface.h>
#include <interpreterBase/robotModel/commonRobotModel.h>

namespace qrTest {
namespace mockKitPlugin2 {

class MockKitPlugin2Interface : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "mockKitPlugin2.MockKitPlugin2Interface")

public:
	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<interpreterBase::robotModel::RobotModelInterface *> robotModels() override;

	// Transfers ownership.
	QWidget *settingsWidget() const override;

	qReal::IdList unsupportedBlocks() const override;

private:
	interpreterBase::robotModel::CommonRobotModel mRobotModel;
};

}
}
