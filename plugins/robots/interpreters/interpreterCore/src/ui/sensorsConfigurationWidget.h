#pragma once

#include <QtCore/QMap>
#include <QtWidgets/QWidget>

#include <interpreterBase/robotModel/portInfo.h>
#include <interpreterBase/robotModel/pluggableDeviceInfo.h>

class QComboBox;

namespace interpreterBase {
namespace robotModel {
class RobotModelInterface;
}
}

namespace interpreterCore {
namespace ui {

/// A number of combo boxes for sensors selection
class SensorsConfigurationWidget : public QWidget
{
	Q_OBJECT

public:
	/// @param autosaveMode Should save happed on each modification or would
	/// be called manually
	explicit SensorsConfigurationWidget(QWidget *parent = 0, bool autosaveMode = false);

	/// Reinits current widget for the given robot model
	void loadRobotModel(interpreterBase::robotModel::RobotModelInterface * const robotModel);

signals:
	void saved();

public slots:
	void refresh();
	void save();

private:
	QLayout *initPort(interpreterBase::robotModel::PortInfo const &port
			, QList<interpreterBase::robotModel::PluggableDeviceInfo> const &sensors);

	QString settingsKey(interpreterBase::robotModel::PortInfo const &port) const;

//	robots::enums::sensorType::SensorTypeEnum selectedPort1Sensor() const;
//	robots::enums::sensorType::SensorTypeEnum selectedPort2Sensor() const;
//	robots::enums::sensorType::SensorTypeEnum selectedPort3Sensor() const;
//	robots::enums::sensorType::SensorTypeEnum selectedPort4Sensor() const;

	bool mAutosaveMode;
	QString mRobotModelId;
	QMap<QComboBox *, interpreterBase::robotModel::PortInfo> mBoxesToPorts;
};

}
}
