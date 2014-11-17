#pragma once

#include <QtCore/QObject>

#include "../../../qrgui/plugins/toolPluginInterface/pluginInterface.h"
#include "../../../qrgui/plugins/toolPluginInterface/pluginConfigurator.h"
#include "gui/pushDialog.h"
#include "gui/pullDialog.h"
#include "gui/commitDialog.h"
#include "gui/versioningPage.h"
#include "gui/cloneDialog.h"
#include "gui/remoteDialog.h"
#include "gui/resetDialog.h"
#include "gui/statusDialog.h"
#include "gui/logDialog.h"
#include "gui/remoteListDialog.h"
#include "gui/diffBetweenDialog.h"
#include "transparentMode.h"

namespace git {

class GitPlugin;

namespace details {

class ViewInteraction : public QObject
{
	Q_OBJECT

public:
	explicit ViewInteraction(GitPlugin *pluginInstance);

	QList<qReal::ActionInfo> actions() const;
	QPair<QString, qReal::gui::PreferencesPage *> preferencesPage() const;
	void init(qReal::PluginConfigurator const &configurator);

public slots:
	void initClicked();
	void cloneClicked();
	void commitClicked();
	void remoteClicked();
	void pushClicked();
	void pullClicked();
	void resetClicked();
	void cleanClicked();
	void statusClicked();
	void logClicked();
	void remoteListClicked();
	void versionsClicked();
	void diffClicked();
	void diffBetweenClicked();

private slots:
	void onInitComplete(bool const success);
	void onCloneComplete(bool const success);
	void onCommitComplete(bool const success);
	void onRemoteComplete(bool const success);
	void onPushComplete(bool const success);
	void onPullComplete(bool const success);
	void onResetComplete(bool const success);
	void onStatusComplete(QString answer, bool const success);
	void onLogComplete(QString answer, bool const success);
	void onRemoteListComplete(QString answer, bool const success);
	void modeChanged(bool compactMode);
	void removeClosedTab(QWidget *widget);

private:
	void initActions();
	void showMessage(QString const &message);
	void reopenWithoutSavings();
	QWidget *makeDiffTab();

	bool isFullScreen;
	QList<qReal::ActionInfo> mMenu;
	GitPlugin *mPlugin;
	qReal::gui::MainWindowInterpretersInterface *mMainWindowIface;
	qReal::ProjectManagementInterface *mProjectManager;
	qrRepo::RepoControlInterface *mRepoApi;
	ui::PreferencesVersioningPage *mPreferencesPage;
	QList<QWidget *> mDiffWidgets;
	TransparentMode *mCompactMode;
	qReal::SystemEvents *mSystemEvents;
};
}
}
