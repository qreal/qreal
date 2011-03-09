#pragma once

#include <QtGui/QDialog>

#include "../../interpreters/robots/sensorConstants.h"

namespace Ui {
	class RobotSettingsDialog;
}

namespace qReal {
namespace gui {

class RobotSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit RobotSettingsDialog(QWidget *parent = 0);
	~RobotSettingsDialog();

	QString selectedPortName() const;
	interpreters::robots::sensorType::SensorTypeEnum selectedPort1Sensor() const;
	interpreters::robots::sensorType::SensorTypeEnum selectedPort2Sensor() const;
	interpreters::robots::sensorType::SensorTypeEnum selectedPort3Sensor() const;
	interpreters::robots::sensorType::SensorTypeEnum selectedPort4Sensor() const;
	interpreters::robots::robotModelType::robotModelTypeEnum selectedRobotModel() const;

private slots:
	void ok();
	void cancel();
	void activatedNullModel(bool checked);

private:
	Ui::RobotSettingsDialog *mUi;
	void initRobotModelType(interpreters::robots::robotModelType::robotModelTypeEnum type);
};

}
}
