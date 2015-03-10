#pragma once

#include <kitBase/additionalPreferences.h>

namespace Ui {
class NxtAdditionalPreferences;
}

namespace nxt {

class NxtAdditionalPreferences : public kitBase::AdditionalPreferences
{
	Q_OBJECT

public:
	explicit NxtAdditionalPreferences(const QString &realRobotName, QWidget *parent = 0);
	~NxtAdditionalPreferences();

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

	Ui::NxtAdditionalPreferences *mUi;
	const QString mRealRobotName;
};

}
