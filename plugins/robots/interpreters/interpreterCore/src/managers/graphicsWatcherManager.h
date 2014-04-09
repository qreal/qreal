#pragma once

#include <QtCore/QObject>

#include <qrutils/graphicsWatcher/sensorsGraph.h>
#include <interpreterBase/devicesConfigurationProvider.h>

namespace interpreterCore {

/// Incapsulates inner operations on managing graphics watcher on the dock
class GraphicsWatcherManager : public QObject, public interpreterBase::DevicesConfigurationProvider
{
public:
	explicit GraphicsWatcherManager(utils::ExpressionsParser * const parser, QObject *parent = 0);

	/// Returns the graphcis watcher widget itself for placing it into dock
	QWidget *widget();  // Transfers ownership

public slots:
	/// Starts graphics watcher`s job even if user stopped it himself
	void forceStart();

	/// Stops graphics watcher`s job even if user started it himself
	void forceStop();

private:
	void onDeviceConfigurationChanged(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &sensor) override;

	void updateSensorsList(QString const &currentRobotModel);

	utils::sensorsGraph::SensorsGraph *mWatcher;  // Doesn`t have ownership
};

}
