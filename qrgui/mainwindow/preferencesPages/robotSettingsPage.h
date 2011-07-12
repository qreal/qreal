#pragma once

#include "page.h"

#include "../../interpreters/robots/sensorConstants.h"
#include "../../../plugins/robots/thirdparty/qextserialport/src/qextserialenumerator.h"

using namespace qReal::interpreters::robots;

namespace Ui {
	class PreferencesRobotSettingsPage;
}

class PreferencesRobotSettingsPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesRobotSettingsPage(QWidget *parent = 0);
	~PreferencesRobotSettingsPage();

	void save();

protected:
	void changeEvent(QEvent *e);

private slots:
	void activatedUnrealModel(bool checked);
	void manualComPortCheckboxChecked(bool state);

private:
	void initRobotModelType(robotModelType::robotModelTypeEnum type);

	QString selectedPortName() const;
	sensorType::SensorTypeEnum selectedPort1Sensor() const;
	sensorType::SensorTypeEnum selectedPort2Sensor() const;
	sensorType::SensorTypeEnum selectedPort3Sensor() const;
	sensorType::SensorTypeEnum selectedPort4Sensor() const;
	robotModelType::robotModelTypeEnum selectedRobotModel() const;

	Ui::PreferencesRobotSettingsPage *mUi;
};
