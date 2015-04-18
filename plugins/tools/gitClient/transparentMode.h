#pragma once

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QPair>

#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrutils/invocationUtils/functorOperation.h>

#include "gui/changeVersionWidget.h"


namespace git {
class GitPlugin;

class TransparentMode : public QObject
{
	Q_OBJECT
public:
	explicit TransparentMode(
		GitPlugin *plugin
		, qReal::ProjectManagementInterface *projectManager
		, qReal::gui::MainWindowInterpretersInterface *mainWindowIface
		, qReal::SystemEvents *systemInterface
		);
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
	void showDiffAsync(const QString &newHash, const QString &oldHash, QWidget *widget);

private:
	void init();
	void isInit(const QString &directory = QString());
	QList<QPair<QString, QString> > parseLog(QString log); //hash & mainPart
	void showDiff(const QString &newHash, const QString &oldHash, QWidget *widget);

	GitPlugin *mPlugin;
	ChangeVersionWidget *mCompactWidget;
	qReal::ProjectManagementInterface *mProjectManager;
	qReal::gui::MainWindowInterpretersInterface *mMainWindowIface;
	qReal::SystemEvents *mSystemInterface;

	bool tabIsReady;
	bool isFullScreen;
};
}
