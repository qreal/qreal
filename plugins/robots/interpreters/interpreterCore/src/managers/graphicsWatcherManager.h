#pragma once

#include <QtCore/QObject>

#include <qrutils/graphicsWatcher/sensorsGraph.h>
#include <interpreterBase/devicesConfigurationProvider.h>

#include <qrtext/debuggerInterface.h>

namespace interpreterCore {

/// Incapsulates inner operations on managing graphics watcher in the dock window.
class GraphicsWatcherManager : public QObject, public interpreterBase::DevicesConfigurationProvider
{
public:
	/// Constructor.
	/// @param parser - blocks parser, used to show values of variables on graphs.
	/// @param parent - parent of this widget in Qt widget hierarchy.
	explicit GraphicsWatcherManager(qrtext::DebuggerInterface const &parser, QObject *parent = 0);

	/// Returns the graphics watcher widget itself for placing it into dock.
	/// Note that if this method will not be called, nobody will delete a widget, which will result in memleak.
	QWidget *widget();  // Transfers ownership

public slots:
	/// Starts graphics watcher`s job even if user stopped it himself.
	void forceStart();

	/// Stops graphics watcher`s job even if user started it himself.
	void forceStop();

private:
	void onDeviceConfigurationChanged(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &sensor
			, Reason reason) override;

	void updateSensorsList(QString const &currentRobotModel);

	utils::sensorsGraph::SensorsGraph *mWatcher;  // Doesn`t have ownership
};

}
