#include "sensorsConfigurationManager.h"

#include <QtXml/QDomDocument>

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;
using namespace interpreterBase;
using namespace robotModel;
using namespace qReal;

SensorsConfigurationManager::SensorsConfigurationManager(
		qReal::GraphicalModelAssistInterface &graphicalModelAssistInterface
		, qReal::LogicalModelAssistInterface &logicalModelAssistInterface
		, qReal::gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface
		, qReal::SystemEventsInterface &systemEvents
		)
	: SensorsConfigurationProvider("SensorsConfigurationManager")
	, mGraphicalModelAssistInterface(graphicalModelAssistInterface)
	, mLogicalModelAssistInterface(logicalModelAssistInterface)
	, mMainWindowInterpretersInterface(mainWindowInterpretersInterface)
{
	QObject::connect(&systemEvents, &qReal::SystemEventsInterface::activeTabChanged
			, [&] (Id const &diagramRootId) { this->onActiveTabChanged(diagramRootId); });
}

QString SensorsConfigurationManager::save() const
{
	QDomDocument result;
	for (QString const &robotModel : mCurrentConfiguration.keys()) {
		QDomElement robotModelElement = result.createElement("robotModel");
		robotModelElement.setAttribute("name", robotModel);
		result.appendChild(robotModelElement);
		for (PortInfo const &port : mCurrentConfiguration[robotModel].keys()) {
			PluggableDeviceInfo const device = mCurrentConfiguration[robotModel][port];
			QDomElement configurationElement = result.createElement("configuration");
			configurationElement.setAttribute("port", port.toString());
			configurationElement.setAttribute("device", device.toString());
			robotModelElement.appendChild(configurationElement);
		}
	}

	return result.toString();
}

void SensorsConfigurationManager::load(QString const &configuration)
{
	mCurrentConfiguration.clear();
	QDomDocument parsedConfiguration;
	parsedConfiguration.setContent(configuration);
	for (QDomElement robotModelElement = parsedConfiguration.firstChildElement(); !robotModelElement.isNull()
			; robotModelElement = robotModelElement.nextSiblingElement())
	{
		QString const robotModel = robotModelElement.attribute("name");
		for (QDomElement configurationElement = parsedConfiguration.firstChildElement(); !configurationElement.isNull()
				; configurationElement = configurationElement.nextSiblingElement())
		{
			PortInfo const port = PortInfo::fromString(robotModelElement.attribute("port"));
			PluggableDeviceInfo const device = PluggableDeviceInfo::fromString(robotModelElement.attribute("device"));
			if (port.isValid()) {
				sensorConfigurationChanged(robotModel, port, device);
			}
		}
	}

	refreshSensorsConfiguration();
}

void SensorsConfigurationManager::onSensorConfigurationChanged(QString const &robotModel
		, PortInfo const &port, PluggableDeviceInfo const &sensor)
{
	Q_UNUSED(robotModel)
	Q_UNUSED(port)
	Q_UNUSED(sensor)

	/// @todo On refreshSensorsConfiguration() call it will save each device a number of times
	/// equal to overall number of devices, at least (since there can be notifications from neighbouring
	/// ConfigurationProvider which may trigger onSensorConfigurationChanged() here.
	/// It is needed to be optimized somehow.

	qReal::Id const activeDiagramGraphicalId = mMainWindowInterpretersInterface.activeDiagram();
	if (activeDiagramGraphicalId.isNull()) {
		return;
	}

	qReal::Id const logicalRootId = mGraphicalModelAssistInterface.logicalId(activeDiagramGraphicalId);

	mLogicalModelAssistInterface.setPropertyByRoleName(logicalRootId, save(), "devicesConfiguration");
}

void SensorsConfigurationManager::onActiveTabChanged(Id const &graphicalRootId)
{
	if (graphicalRootId.isNull()) {
		return;
	}

	qReal::Id const logicalRootId = mGraphicalModelAssistInterface.logicalId(graphicalRootId);
	load(mLogicalModelAssistInterface.propertyByRoleName(logicalRootId, "devicesConfiguration").toString());
}
