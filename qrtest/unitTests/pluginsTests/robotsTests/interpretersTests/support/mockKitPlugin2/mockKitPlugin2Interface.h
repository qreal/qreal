#pragma once

#include <QtCore/QObject>

#include <interpreterBase/kitPluginInterface.h>

namespace qrTest {
namespace mockKitPlugin2 {

class MockKitPlugin2Interface : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "mockKitPlugin2.MockKitPlugin1Interface")

public:
	// Override.
	virtual QString kitId() const;

	// Override.
	// Transfers ownership.
	virtual QWidget *settingsWidget() const;

	// Override.
	virtual qReal::IdList unsupportedBlocks() const;
};

}
}
