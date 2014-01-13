#pragma once

#include <QtCore/QObject>

#include <interpreterBase/kitPluginInterface.h>
#include <interpreterBase/robotModel/commonRobotModel.h>

namespace qrTest {
namespace mockKitPlugin1 {

class MockKitPlugin1Interface : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "mockKitPlugin1.MockKitPlugin1Interface")

public:
	virtual QString kitId() const override;

	virtual QString friendlyKitName() const override;

	virtual interpreterBase::robotModel::RobotModelInterface &robotModel() override;

	// Transfers ownership.
	virtual QWidget *settingsWidget() const override;

	virtual qReal::IdList unsupportedBlocks() const override;

	// Transfers ownership.
//	virtual QList<QWidget *> toolWidgets() const;

private:
	interpreterBase::robotModel::CommonRobotModel mRobotModel;
};

}
}
