#pragma once

#include <QtWidgets/QWidget>

#include "../sensorConstants.h"

namespace Ui
{
	class SensorsConfigurationWidget;
}

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{

/// A number of combo boxes for sensors selection
class SensorsConfigurationWidget : public QWidget
{
	Q_OBJECT

public:
	/// @param autosaveMode Should save happed on each modification or would
	/// be called manually
	SensorsConfigurationWidget(bool autosaveMode = true, QWidget *parent = 0);
	~SensorsConfigurationWidget();

	void reinitValues();
	void retranslateUi();

signals:
	void saved();

public slots:
	void refresh();
	void save();

protected:
	void changeEvent(QEvent *e);

private:
	void startChangesListening();
	sensorType::SensorTypeEnum selectedPort1Sensor() const;
	sensorType::SensorTypeEnum selectedPort2Sensor() const;
	sensorType::SensorTypeEnum selectedPort3Sensor() const;
	sensorType::SensorTypeEnum selectedPort4Sensor() const;

	Ui::SensorsConfigurationWidget *mUi;
};

}
}
}
}
