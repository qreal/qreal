#pragma once

#include <interpreterBase/additionalPreferences.h>

namespace Ui {
class Ev3AdditionalPreferences;
}

namespace ev3KitInterpreter {

class Ev3AdditionalPreferences : public interpreterBase::AdditionalPreferences
{
	Q_OBJECT

public:
	explicit Ev3AdditionalPreferences(const QString &realRobotName, QWidget *parent = 0);
	~Ev3AdditionalPreferences();

	void save() override;
	void restoreSettings() override;
	void onRobotModelChanged(interpreterBase::robotModel::RobotModelInterface * const robotModel) override;

signals:
	void settingsChanged();

private slots:
	void manualComPortCheckboxChecked(bool state);

private:

	QString selectedCommunication() const;
	QString selectedPortName() const;

	Ui::Ev3AdditionalPreferences *mUi;
	const QString mRealRobotName;
};

}
