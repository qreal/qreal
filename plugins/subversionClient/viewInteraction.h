#pragma once

#include <QtCore/QObject>

#include "../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../qrgui/toolPluginInterface/pluginConfigurator.h"
#include "gui/checkoutDialog.h"
#include "gui/commitDialog.h"
#include "gui/versioningPage.h"

namespace versioning
{

class SubversionPlugin;

namespace details
{

class ViewInteraction : public QObject
{
	Q_OBJECT

public:
	explicit ViewInteraction(SubversionPlugin *pluginInstance);

	QList<qReal::ActionInfo> actions() const;
	QPair<QString, PreferencesPage *> preferencesPage() const;
	void init(qReal::PluginConfigurator const &configurator);

public slots:
	void checkoutClicked();
	void updateClicked();
	void commitClicked();
	void revertClicked();
	void cleanUpClicked();
	void infoClicked();

private:
	void initActions();
	void showMessage(QString const &message);

	QList<qReal::ActionInfo> mMenu;
	SubversionPlugin *mPlugin;
	qReal::gui::MainWindowInterpretersInterface *mMainWindowIface;

	ui::PreferencesVersioningPage *mPreferencesPage;
};

}
}
