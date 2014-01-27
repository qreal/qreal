#pragma once

#include <QtWidgets/QWidget>

namespace interpreterCore {
namespace interpreter {
namespace details {

/// A number of combo boxes for sensors selection
class SensorsConfigurationWidget : public QWidget
{
	Q_OBJECT

public:
	/// @param autosaveMode Should save happed on each modification or would
	/// be called manually
	explicit SensorsConfigurationWidget(bool autosaveMode = true, QWidget *parent = 0);

	void loadKit(QStringList const &ports, QStringList const &sensors);

signals:
	void saved();

public slots:
	void refresh();
	void save();

private:
	QLayout *initPort(QString const &port, QStringList const &sensors);

//	robots::enums::sensorType::SensorTypeEnum selectedPort1Sensor() const;
//	robots::enums::sensorType::SensorTypeEnum selectedPort2Sensor() const;
//	robots::enums::sensorType::SensorTypeEnum selectedPort3Sensor() const;
//	robots::enums::sensorType::SensorTypeEnum selectedPort4Sensor() const;

	bool mAutosaveMode;
};

}
}
}
