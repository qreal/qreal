#include "transparentMode.h"
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QString>

using namespace qReal;

qReal::TransparentMode::TransparentMode(QList<VersioningPluginInterface *> mPlugins
		, ProjectManager *projectManager):
	mPlugin(NULL), mProjectManager(projectManager)
{
	foreach (VersioningPluginInterface *plugin, mPlugins){
		qDebug() << plugin->friendlyName();
		if (plugin->friendlyName() == "Git Plugin"){
			mPlugin = plugin;
		}
	}
}

void TransparentMode::isInit(QString directory)
{
	if (!mPlugin->isMyWorkingCopy(directory)){
		mPlugin->beginWorkingCopyDownloading(QString(),QString());
	}
}

void TransparentMode::listLog()
{
	isInit("unsaved");
	QStringList format;
	format << "--pretty=format:" << "%H - %an, %ad : %s";
	QString log = mPlugin->getLog(format);
	QList<QPair<QString , QString> > listlog = parseLog(log);
	emit listLogIsReady(listlog);
}

void TransparentMode::setVersion(QString hash)
{
	isInit("unsaved");
	qDebug() << hash;
	mPlugin->setVersion(hash);
}

void TransparentMode::saveVersion()
{
	isInit("unsaved");
	mProjectManager->save();
	mPlugin->beginChangesSubmitting("version was saved in a transparent mode",QString());
	listLog();
}

QList<QPair<QString , QString> >TransparentMode::parseLog(QString log) //hash & mainPart
{
	if (log != ""){
		QStringList listLog = log.split(QChar('\n'));
		QRegExp parseHash (".*[^-]-");
		int version = listLog.size();
		int number = 0;
		QList<QPair <QString, QString> > listOfParseLog;

		while (version != 0)
		{
			version--;
			QString tmpList = listLog.at(number);
			number++;
			parseHash.indexIn(tmpList);
			tmpList.remove(parseHash.cap());
			QString hash = parseHash.cap();
			hash.remove(" -");
			listOfParseLog.append(qMakePair(hash, QString("Version " + QString::number(version) + ":" + tmpList)));
		}
		return listOfParseLog;
	} else {
		return QList<QPair <QString, QString> >();
	}
}
