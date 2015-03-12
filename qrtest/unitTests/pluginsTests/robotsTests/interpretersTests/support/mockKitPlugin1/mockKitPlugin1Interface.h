#pragma once

#include <QtCore/QObject>

#include <kitBase/additionalPreferences.h>
#include <kitBase/kitPluginInterface.h>
#include <kitBase/robotModel/commonRobotModel.h>

namespace qrTest {
namespace mockKitPlugin1 {

class MockKitPlugin1Interface : public QObject, public kitBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "mockKitPlugin1.MockKitPlugin1Interface")

public:
	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;

	// Transfers ownership.
	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			kitBase::robotModel::RobotModelInterface const *model) override;

	QList<qReal::ActionInfo> customActions() override;

	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

	QIcon iconForFastSelector(kitBase::robotModel::RobotModelInterface const &robotModel) const override;
};

}
}
