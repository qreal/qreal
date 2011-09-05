#pragma once

#include "../../../qrgui/dialogs/preferencesPages/page.h"

#include "sensorConstants.h"

using namespace qReal::interpreters::robots;

namespace Ui {
	class PreferencesRobotSettingsPage;
}

namespace qReal {
namespace interpreters {
namespace robots {

class PreferencesRobotSettingsPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesRobotSettingsPage(QWidget *parent = 0);
	~PreferencesRobotSettingsPage();

	void save();
	QString selectedCommunication() const;

protected:
	void changeEvent(QEvent *e);

private slots:
	void activatedUnrealModel(bool checked);
	void manualComPortCheckboxChecked(bool state);

private:
	void initRobotModelType(robotModelType::robotModelTypeEnum type);
	void initTypeOfCommunication(QString type);

	QString selectedPortName() const;
	sensorType::SensorTypeEnum selectedPort1Sensor() const;
	sensorType::SensorTypeEnum selectedPort2Sensor() const;
	sensorType::SensorTypeEnum selectedPort3Sensor() const;
	sensorType::SensorTypeEnum selectedPort4Sensor() const;
	robotModelType::robotModelTypeEnum selectedRobotModel() const;

	Ui::PreferencesRobotSettingsPage *mUi;
};

}
}
}
