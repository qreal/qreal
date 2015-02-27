#pragma once

#include <kitBase/additionalPreferences.h>

namespace Ui {
class TrikAdditionalPreferences;
}

namespace trik {

class TrikAdditionalPreferences : public kitBase::AdditionalPreferences
{
	Q_OBJECT

public:
	explicit TrikAdditionalPreferences(const QStringList &realRobotNames, QWidget *parent = 0);
	~TrikAdditionalPreferences();

	void save() override;
	void restoreSettings() override;
	void onRobotModelChanged(kitBase::robotModel::RobotModelInterface * const robotModel) override;

signals:
	void settingsChanged();

private:
	Ui::TrikAdditionalPreferences *mUi;
	const QStringList mRealRobotNames;
};

}
