#pragma once

#include <QtWidgets/QWidget>

//#include "../sensorConstants.h"

namespace utils {

/// A number of combo boxes for sensors selection
class SensorsConfigurationWidget : public QWidget
{
	Q_OBJECT

public:
	/// @param autosaveMode Should save happed on each modification or would
	/// be called manually
	explicit SensorsConfigurationWidget(bool autosaveMode = true, QWidget *parent = 0);

	void reinitValues();

signals:
	void saved();

public slots:
	void refresh();
	void save();

private:
	void startChangesListening();
//	robots::enums::sensorType::SensorTypeEnum selectedPort1Sensor() const;
//	robots::enums::sensorType::SensorTypeEnum selectedPort2Sensor() const;
//	robots::enums::sensorType::SensorTypeEnum selectedPort3Sensor() const;
//	robots::enums::sensorType::SensorTypeEnum selectedPort4Sensor() const;
};

}
