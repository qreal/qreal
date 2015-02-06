#include "interpreterCore/managers/graphicsWatcherManager.h"

using namespace interpreterCore;
using namespace utils;
using namespace interpreterBase::robotModel;

GraphicsWatcherManager::GraphicsWatcherManager(qrtext::DebuggerInterface const &parser, QObject *parent)
	: QObject(parent)
	, mWatcher(new sensorsGraph::SensorsGraph(parser))
{
	mWatcher->setStartStopButtonsVisible(false);
}

QWidget *GraphicsWatcherManager::widget()
{
	return mWatcher;
}

void GraphicsWatcherManager::forceStart()
{
	mWatcher->startJob();
}

void GraphicsWatcherManager::forceStop()
{
	mWatcher->stopJob();
}

void GraphicsWatcherManager::onDeviceConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &sensor, Reason reason)
{
	Q_UNUSED(port)
	Q_UNUSED(sensor)
	Q_UNUSED(reason)

	updateSensorsList(robotModel);
}

void GraphicsWatcherManager::updateSensorsList(QString const &currentRobotModel)
{
	mWatcher->clearTrackingObjects();
	int index = 0;
	for (PortInfo const &port : configuredPorts(currentRobotModel)) {
		DeviceInfo const device = currentConfiguration(currentRobotModel, port);
		/// @todo It must depend on port, port must return its variable
		QString const variableName = port.reservedVariable();
		if (!device.isNull() && !variableName.isEmpty()) {
			mWatcher->addTrackingObject(index, variableName, QString("%1: %2").arg(port.name(), device.friendlyName()));
			++index;
		}
	}
}
