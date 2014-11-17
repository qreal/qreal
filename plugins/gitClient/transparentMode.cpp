#include "transparentMode.h"
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include "gitPlugin.h"

using namespace git;

TransparentMode::TransparentMode(GitPlugin *plugin
								 , qReal::ProjectManagementInterface *projectManager
								 , qReal::gui::MainWindowInterpretersInterface *mainWindowIface
								 , qReal::SystemEvents *systemInterface):
	mPlugin(plugin)
	, mProjectManager(projectManager)
	, mMainWindowIface(mainWindowIface)
	, mSystemInterface(systemInterface)
	, tabIsReady(false)
	, isFullScreen(false)
{
	init();
}

void TransparentMode::openChangeVersionTab()
{
	if (!tabIsReady){
		init();
	}
//	isFullScreen = mMainWindowIface->isFullScreen();
//	if (!isFullScreen){
//		mMainWindowIface->makeFullScreen(true);
//	}
	getAndUpdateLog();
	mCompactWidget->setVisible(true);
	mMainWindowIface->openTab(mCompactWidget, "Change Version");
	mCompactWidget->setMaximumSize(mCompactWidget->size());


}

void TransparentMode::isInit(QString const &directory, const bool &prepareAndProcess)
{
	if (!mPlugin->isMyWorkingCopy(directory, true, prepareAndProcess)){
		mPlugin->doInit(directory, true);
	}
	if (!tabIsReady){
		init();
	}
}

void TransparentMode::getAndUpdateLog()
{
	QString format = "--pretty=format:\"%H\ -\ %aD\"";
	QString log = mPlugin->getLog(format, true);
	QList<QPair<QString, QString> > listlog = parseLog(log);
	mCompactWidget->updateLog(listlog);
}

void TransparentMode::removeBrokenPointers(QWidget *widget)
{
	if (widget == mCompactWidget)
	{
		tabIsReady = false;
//		mMainWindowIface->makeFullScreen(isFullScreen);
	}
}

void TransparentMode::showDiff(QString newHash, QString oldHash, QWidget *widget)
{
	mMainWindowIface->windowWidget()->setEnabled(false);
	mPlugin->showDiff(oldHash, newHash, mProjectManager->saveFilePath(), widget, true);
	mMainWindowIface->windowWidget()->setEnabled(true);
}

void TransparentMode::init()
{
	mCompactWidget = new ChangeVersionWidget;
	mCompactWidget->setVisible(false);
	connect(mCompactWidget, SIGNAL(showDiff(QString, QString, QWidget*)), this
			, SLOT(showDiff(QString, QString, QWidget*)));
	connect(mCompactWidget, SIGNAL(loadVersion(QString)),SLOT(setVersion(QString)));
	connect(mSystemInterface, SIGNAL(indefiniteTabClosed(QWidget*)), this, SLOT(removeBrokenPointers(QWidget*)));
	tabIsReady = true;
}

void TransparentMode::listLog()
{
	if (mPlugin->isMyWorkingCopy(QString(), true, true))
	{
		if (/*mProjectManager->getUnsavedIndicator()*/true){
			mProjectManager->save();
		} else {
			getAndUpdateLog();
		}
	} else {
		emit listLogIsReady(QList<QPair<QString, QString> >());
	}
}

void TransparentMode::setVersion(QString hash)
{
	mPlugin->setVersion(hash, true);
//	mProjectManager->reload();
}

void TransparentMode::saveVersion()
{
	isInit();
	mPlugin->beginChangesSubmitting("version was saved in a transparent mode", mProjectManager->saveFilePath(), true);
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
