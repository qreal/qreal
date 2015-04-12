#pragma once

#include <kitBase/additionalPreferences.h>

namespace Ui {
class Ev3AdditionalPreferences;
}

namespace ev3 {

class Ev3AdditionalPreferences : public kitBase::AdditionalPreferences
{
	Q_OBJECT

public:
	explicit Ev3AdditionalPreferences(const QString &realRobotName, QWidget *parent = 0);
	~Ev3AdditionalPreferences();

	void save() override;
	void restoreSettings() override;
	void onRobotModelChanged(kitBase::robotModel::RobotModelInterface * const robotModel) override;

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
