#include "devicesConfigurationManager.h"

#include <QtXml/QDomDocument>

#include <qrkernel/settingsManager.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace interpreterCore;
using namespace interpreterBase;
using namespace robotModel;
using namespace qReal;

DevicesConfigurationManager::DevicesConfigurationManager(
		qReal::GraphicalModelAssistInterface &graphicalModelAssistInterface
		, qReal::LogicalModelAssistInterface &logicalModelAssistInterface
		, qReal::gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface
		, qReal::SystemEventsInterface &systemEvents
		)
	: DevicesConfigurationProvider("DevicesConfigurationManager")
	, mGraphicalModelAssistInterface(graphicalModelAssistInterface)
	, mLogicalModelAssistInterface(logicalModelAssistInterface)
	, mMainWindowInterpretersInterface(mainWindowInterpretersInterface)
{
	QObject::connect(&systemEvents, &qReal::SystemEventsInterface::activeTabChanged
			, [&] (Id const &diagramRootId) { this->onActiveTabChanged(diagramRootId); });
}

QString DevicesConfigurationManager::save() const
{
	QDomDocument result;
	QDomElement rootElement = result.createElement("devicesConfiguration");
	result.appendChild(rootElement);
	for (QString const &robotModel : configuredModels()) {
		QDomElement robotModelElement = result.createElement("robotModel");
		robotModelElement.setAttribute("name", robotModel);
		rootElement.appendChild(robotModelElement);
		for (PortInfo const &port : configuredPorts(robotModel)) {
			DeviceInfo const device = currentConfiguration(robotModel, port);
			QDomElement configurationElement = result.createElement("configuration");
			configurationElement.setAttribute("port", port.toString());
			configurationElement.setAttribute("device", device.toString());
			robotModelElement.appendChild(configurationElement);
		}
	}

	return result.toString();
}

void DevicesConfigurationManager::load(QString const &configuration)
{
	clearConfiguration(Reason::loading);

	QMap<QString, QMap<PortInfo, DeviceInfo>> const parsed = RobotModelUtils::deserialize(configuration);
	for (QString const &robotModel : parsed.keys()) {
		for (PortInfo const &port : parsed[robotModel].keys()) {
			deviceConfigurationChanged(robotModel, port, parsed[robotModel][port], Reason::loading);
		}
	}
}

void DevicesConfigurationManager::onDeviceConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &sensor, Reason reason)
{
	Q_UNUSED(robotModel)
	Q_UNUSED(port)
	Q_UNUSED(sensor)
	Q_UNUSED(reason)

	qReal::Id const activeDiagramGraphicalId = mMainWindowInterpretersInterface.activeDiagram();
	if (activeDiagramGraphicalId.isNull()) {
		return;
	}

	qReal::Id const logicalRootId = mGraphicalModelAssistInterface.logicalId(activeDiagramGraphicalId);
	mLogicalModelAssistInterface.setPropertyByRoleName(logicalRootId, save(), "devicesConfiguration");
}

void DevicesConfigurationManager::onActiveTabChanged(Id const &graphicalRootId)
{
	if (graphicalRootId.isNull()) {
		return;
	}

	qReal::Id const logicalRootId = mGraphicalModelAssistInterface.logicalId(graphicalRootId);
	load(mLogicalModelAssistInterface.propertyByRoleName(logicalRootId, "devicesConfiguration").toString());
}
