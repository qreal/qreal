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
	explicit TrikFSharpAdditionalPreferences(QWidget *parent = 0);
	~TrikFSharpAdditionalPreferences();

	void save() override;
	void restoreSettings() override;
	QString defaultSettingsFile() const;

signals:
	void settingsChanged();

private:
	Ui::TrikFSharpAdditionalPreferences *mUi;
};

}
