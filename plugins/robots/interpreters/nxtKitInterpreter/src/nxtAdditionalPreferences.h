#pragma once

#include <interpreterBase/additionalPreferences.h>

namespace Ui {
class NxtAdditionalPreferences;
}

namespace nxtKitInterpreter {

class NxtAdditionalPreferences : public interpreterBase::AdditionalPreferences
{
	Q_OBJECT

public:
	explicit NxtAdditionalPreferences(const QString &realRobotName, QWidget *parent = 0);
	~NxtAdditionalPreferences();

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

	Ui::NxtAdditionalPreferences *mUi;
	const QString mRealRobotName;
};

}
