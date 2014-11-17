#pragma once

#include "../../../qrgui/dialogs/preferencesPages/preferencesPage.h"
#include "../../../qrutils/versioningUtils/authenticationSettingsWidget.h"
#include "../../../qrutils/versioningUtils/networkSettingsWidget.h"
#include <qrgui/preferencesDialog/preferencesPage.h>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>

namespace Ui {
	class PreferencesVersioningPage;
}

namespace git
{
namespace ui
{

class PreferencesVersioningPage : public qReal::gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesVersioningPage(QWidget *parent = 0);
	virtual ~PreferencesVersioningPage();

	virtual void save();
	virtual void restoreSettings();

signals:
	void checkClientExisting();
	void compactMode(bool modeOn);

protected:
	virtual void changeEvent(QEvent *e);

private slots:
	void on_browseGitClientButton_clicked();

private:
	QComboBox *createComboBox(const QString &text = QString());

	Ui::PreferencesVersioningPage *mUi;
	qReal::versioning::ui::AuthenticationSettingsWidget *mAuthenticationSettings;

};

}
}
