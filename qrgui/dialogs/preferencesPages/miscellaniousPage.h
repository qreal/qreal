#pragma once

#include "dialogs/preferencesPages/preferencesPage.h"
#include "configurationNetworkManager/client.h"

namespace Ui {
class PreferencesMiscellaniousPage;
}

class PreferencesMiscellaniousPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesMiscellaniousPage(QWidget *parent = 0, bool isServer = false);
	~PreferencesMiscellaniousPage();

	void save();
	virtual void restoreSettings();

signals:
	void iconsetChanged();
	void toolbarSizeChanged(int size);
	void needUdpateSettings();

protected:
	void changeEvent(QEvent *e);

private slots:
	void browseImagesPath();
	void getSettings();

private:
	Ui::PreferencesMiscellaniousPage *mUi;

	Client *mClient;
	QString mLastIconsetPath;
};
