#include "transparentMode.h"
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QString>

//all versioning plugins must contain method "friendly name"
static const QStringList requiredPlugins = QStringList("Git Plugin");

using namespace qReal;

// to do: if we need several working plugins: to change mPlugin to list of Plugins and change methods
qReal::TransparentMode::TransparentMode(QList<VersioningPluginInterface *> mPlugins
		, ProjectManager *projectManager):
	mPlugin(NULL), mProjectManager(projectManager)
{
	foreach (VersioningPluginInterface *plugin, mPlugins){
		if (requiredPlugins.contains(plugin->friendlyName())){
			mPlugin = plugin;
		}
	}
}

void TransparentMode::isInit(QString const &directory, const bool &prepareAndProcess)
{
	if (!mPlugin->isMyWorkingCopy(directory, false, prepareAndProcess)){
		mPlugin->beginWorkingCopyDownloading(QString(), QString(), QString(), true);
	}
}

void TransparentMode::getAndUpdateLog()
{
	QString format = "--pretty=format:\"%H\ -\ %aD\"";
	QString log = mPlugin->getLog(format, true);
	QList<QPair<QString, QString> > listlog = parseLog(log);
	emit listLogIsReady(listlog);
}

void TransparentMode::listLog()
{
	if (mPlugin->isMyWorkingCopy(QString(), false, true))
	{
		mProjectManager->save();
		saveVersion();
	} else {
		emit listLogIsReady(QList<QPair<QString, QString> >());
	}
}

void TransparentMode::setVersion(QString hash)
{
	isInit();
	mPlugin->setVersion(hash, true);
	mProjectManager->reload();
}

bool TransparentMode::suggestToSaveAs()
{
	bool tmp = mProjectManager->suggestToSaveAs();
	if (tmp) {
		saveVersion();
	}
	return tmp;
}

bool TransparentMode::saveOrSuggestToSaveAs()
{
	bool tmp = mProjectManager->saveOrSuggestToSaveAs();
	if (tmp) {
		saveVersion();
	}
	return tmp;
}

void TransparentMode::saveVersion()
{
	isInit();
	mPlugin->beginChangesSubmitting("version was saved in a transparent mode", QString(), true);
	getAndUpdateLog();
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
			tmpList.remove("\"");
			QString hash = parseHash.cap();
			hash.remove(" -");
			hash.remove("\"");
			listOfParseLog.append(qMakePair(hash, tmpList));
		}
		return listOfParseLog;
	} else {
		return QList<QPair <QString, QString> >();
	}
}
