#include "interpreterCore/managers/devicesConfigurationManager.h"

#include <QtXml/QDomDocument>

#include <qrkernel/settingsManager.h>
#include <kitBase/robotModel/robotModelUtils.h>

using namespace interpreterCore;
using namespace kitBase;
using namespace robotModel;
using namespace qReal;

DevicesConfigurationManager::DevicesConfigurationManager(
		qReal::GraphicalModelAssistInterface &graphicalModelAssistInterface
		, qReal::LogicalModelAssistInterface &logicalModelAssistInterface
		, qReal::gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface
		, qReal::SystemEvents &systemEvents
		)
	: DevicesConfigurationProvider("DevicesConfigurationManager")
	, mGraphicalModelAssistInterface(graphicalModelAssistInterface)
	, mLogicalModelAssistInterface(logicalModelAssistInterface)
	, mMainWindowInterpretersInterface(mainWindowInterpretersInterface)
{
	QObject::connect(&systemEvents, &qReal::SystemEvents::activeTabChanged
			, [&] (const TabInfo &info) { this->onActiveTabChanged(info); });
}

QString DevicesConfigurationManager::save() const
{
	QDomDocument result;
	QDomElement rootElement = result.createElement("devicesConfiguration");
	result.appendChild(rootElement);
	for (const QString &robotModel : configuredModels()) {
		QDomElement robotModelElement = result.createElement("robotModel");
		robotModelElement.setAttribute("name", robotModel);
		rootElement.appendChild(robotModelElement);
		for (const PortInfo &port : configuredPorts(robotModel)) {
			const DeviceInfo device = currentConfiguration(robotModel, port);
			QDomElement configurationElement = result.createElement("configuration");
			configurationElement.setAttribute("port", port.toString());
			configurationElement.setAttribute("device", device.toString());
			robotModelElement.appendChild(configurationElement);
		}
	}

	return result.toString();
}

void DevicesConfigurationManager::load(const QString &configuration)
{
	clearConfiguration(Reason::loading);

	QMap<QString, QMap<PortInfo, DeviceInfo>> const parsed = RobotModelUtils::deserialize(configuration);
	for (const QString &robotModel : parsed.keys()) {
		for (const PortInfo &port : parsed[robotModel].keys()) {
			deviceConfigurationChanged(robotModel, port, parsed[robotModel][port], Reason::loading);
		}
	}
}

void DevicesConfigurationManager::onDeviceConfigurationChanged(const QString &robotModel
		, const PortInfo &port, const DeviceInfo &sensor, Reason reason)
{
	if (reason == Reason::loading) {
		return;
	}

	Q_UNUSED(robotModel)
	Q_UNUSED(port)
	Q_UNUSED(sensor)

	const qReal::Id activeDiagramGraphicalId = mMainWindowInterpretersInterface.activeDiagram();
	if (activeDiagramGraphicalId.isNull()) {
		return;
	}

	const qReal::Id logicalRootId = mGraphicalModelAssistInterface.logicalId(activeDiagramGraphicalId);
	mLogicalModelAssistInterface.setPropertyByRoleName(logicalRootId, save(), "devicesConfiguration");
}

void DevicesConfigurationManager::onActiveTabChanged(const TabInfo &info)
{
	if (info.type() != TabInfo::TabType::editor) {
		return;
	}

	const Id logicalRootId = mGraphicalModelAssistInterface.logicalId(info.rootDiagramId());
	const QString devicesConfiguration = logicalRootId.isNull()
			? QString()
			: mLogicalModelAssistInterface.propertyByRoleName(logicalRootId, "devicesConfiguration").toString();
	load(devicesConfiguration);
}
