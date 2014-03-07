#pragma once

#include <QtWidgets/QWidget>

#include "../sensorConstants.h"
#include "sensorsConfigurationProvider.h"

namespace Ui
{
	class SensorsConfigurationWidget;
}

class QComboBox;

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

/// A number of combo boxes for sensors selection
class SensorsConfigurationWidget : public QWidget, public SensorsConfigurationProvider
{
	Q_OBJECT

public:
	/// @param autosaveMode Should save happed on each modification or would
	/// be called manually
	SensorsConfigurationWidget(bool autosaveMode = true, QWidget *parent = 0);
	~SensorsConfigurationWidget();

	void reinitValues();
	void retranslateUi();

public slots:
	void save();

protected:
	void changeEvent(QEvent *e);

private:
	void onSensorConfigurationChanged(
			robots::enums::inputPort::InputPortEnum port
			, robots::enums::sensorType::SensorTypeEnum type
			) override;

	void startChangesListening();
	robots::enums::sensorType::SensorTypeEnum selectedPortSensor(QComboBox const &comboBox) const;

	Ui::SensorsConfigurationWidget *mUi;  // Has ownership.
};

}
}
}
}
