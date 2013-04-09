#pragma once

#include "../../../../qrgui/dialogs/preferencesPages/preferencesPage.h"

#include "sensorConstants.h"
#include "details/sensorsConfigurationWidget.h"

namespace Ui {
	class PreferencesRobotSettingsPage;
}

namespace qReal {
namespace interpreters {
namespace robots {

/// Preferences page for robots interpreter plugin
class PreferencesRobotSettingsPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesRobotSettingsPage(QWidget *parent = 0);
	~PreferencesRobotSettingsPage();

	void save();

	QString selectedPortName() const;
	QString selectedCommunication() const;

signals:
	void saved();

protected:
	void changeEvent(QEvent *e);

private slots:
	void activatedUnrealModel(bool checked);
	void bluetoothCommunucationToggled();
	void manualComPortCheckboxChecked(bool state);
	void refreshPorts();
	void refreshCommunicationGroup();

private:
	void initRobotModelType(robotModelType::robotModelTypeEnum type);
	void initTypeOfCommunication(QString const &type);

	sensorType::SensorTypeEnum selectedPort1Sensor() const;
	sensorType::SensorTypeEnum selectedPort2Sensor() const;
	sensorType::SensorTypeEnum selectedPort3Sensor() const;
	sensorType::SensorTypeEnum selectedPort4Sensor() const;
	robotModelType::robotModelTypeEnum selectedRobotModel() const;
	bool textVisible() const;

	Ui::PreferencesRobotSettingsPage *mUi;
	details::SensorsConfigurationWidget *mSensorsWidget;
};

}
}
}
