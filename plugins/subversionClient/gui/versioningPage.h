#pragma once

#include "../../../qrgui/dialogs/preferencesPages/preferencesPage.h"
#include "../../../qrutils/versioningUtils/authenticationSettingsWidget.h"
#include "../../../qrutils/versioningUtils/networkSettingsWidget.h"

namespace Ui {
	class PreferencesVersioningPage;
}

namespace versioning
{
namespace ui
{

class PreferencesVersioningPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesVersioningPage(QWidget *parent = 0);
	virtual ~PreferencesVersioningPage();

	virtual void save();
	virtual void restoreSettings();

protected:
	virtual void changeEvent(QEvent *e);

private slots:
	void on_browseSvnClientButton_clicked();

private:
	Ui::PreferencesVersioningPage *mUi;
	AuthenticationSettingsWidget *mAuthenticationSettings;

};

}
}
