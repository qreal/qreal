#pragma once

#include <interpreterBase/additionalPreferences.h>

namespace Ui {
class TrikAdditionalPreferences;
}

namespace trikKitInterpreter {

class TrikAdditionalPreferences : public interpreterBase::AdditionalPreferences
{
	Q_OBJECT

public:
	explicit TrikAdditionalPreferences(const QStringList &realRobotNames, QWidget *parent = 0);
	~TrikAdditionalPreferences();

	void save() override;
	void restoreSettings() override;
	void onRobotModelChanged(interpreterBase::robotModel::RobotModelInterface * const robotModel) override;

signals:
	void settingsChanged();

private:
	Ui::TrikAdditionalPreferences *mUi;
	const QStringList mRealRobotNames;
};

}
