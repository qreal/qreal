#pragma once

#include <kitBase/additionalPreferences.h>

namespace Ui {
class TrikFSharpAdditionalPreferences;
}

namespace trik {

class TrikFSharpAdditionalPreferences : public kitBase::AdditionalPreferences
{
	Q_OBJECT

public:
	explicit TrikFSharpAdditionalPreferences(const QString &robotName, QWidget *parent = 0);
	~TrikFSharpAdditionalPreferences();

	void save() override;
	void restoreSettings() override;
	void onRobotModelChanged(kitBase::robotModel::RobotModelInterface * const robotModel) override;

private:
	const QString mRobotName;
	Ui::TrikFSharpAdditionalPreferences *mUi;
};

}
