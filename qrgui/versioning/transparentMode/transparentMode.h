#pragma once
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QPair>
#include "../../mainwindow/projectManager/projectManager.h"
#include "../versioningPluginInterface.h"

namespace qReal {


class TransparentMode : public QObject
{
	Q_OBJECT
public:
	TransparentMode(QList<VersioningPluginInterface *> mPlugins, ProjectManager *projectManager);

signals:
	void listLogIsReady(QList<QPair<QString , QString> > string);

public slots:
	void listLog();
	void setVersion(QString hash);
	void saveVersion();

private:
	ProjectManager *mProjectManager;
	BriefVersioningInterface *mPlugin;
	void isInit();
	QList<QPair<QString, QString> > parseLog(QString log); //hash & mainPart
};
}
