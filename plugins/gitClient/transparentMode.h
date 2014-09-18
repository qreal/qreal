#pragma once
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QPair>
#include "../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../qrgui/toolPluginInterface/pluginConfigurator.h"
#include "gui/changeVersionWidget.h"

namespace git {
class GitPlugin;

class TransparentMode : public QObject
{
	Q_OBJECT
public:
	explicit TransparentMode(GitPlugin *plugin
							 , qReal::ProjectManagementInterface *projectManager
							 , qReal::gui::MainWindowInterpretersInterface *mainWindowIface
							 , qReal::SystemEventsInterface *systemInterface);
	void openChangeVersionTab();

signals:
	void listLogIsReady(QList<QPair<QString , QString> > string);

public slots:
	void saveVersion();

private slots:
	void listLog();
	void setVersion(QString hash);
	void getAndUpdateLog();
	void removeBrokenPointers(QWidget *widget);

private:
	ChangeVersionWidget *mCompactWidget;
	qReal::gui::MainWindowInterpretersInterface *mMainWindowIface;
	qReal::ProjectManagementInterface *mProjectManager;
	GitPlugin *mPlugin;
	SystemEventsInterface *mSystemInterface;

	//TODO allow open several tabs
	bool tabIsReady;

	void init();
	void isInit(const QString &directory = "", bool const &prepareAndProcess = true);
	QList<QPair<QString, QString> > parseLog(QString log); //hash & mainPart
};
}
