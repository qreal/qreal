#include "transparentMode.h"

#include <QtCore/QRegExp>
#include <QtCore/QString>

#include "gitPlugin.h"

using namespace git;

TransparentMode::TransparentMode(
		GitPlugin *plugin
		, qReal::ProjectManagementInterface *projectManager
		, qReal::gui::MainWindowInterpretersInterface *mainWindowIface
		, qReal::SystemEvents *systemInterface
		)
	: mPlugin(plugin)
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
	if (!tabIsReady) {
		init();
	}

	isFullScreen = mMainWindowIface->isFullScreen();
	if (!isFullScreen) {
		mMainWindowIface->makeFullScreen(true);
	}

	getAndUpdateLog();
	mCompactWidget->setVisible(true);
	mMainWindowIface->openTab(mCompactWidget, "Change Version");
	mCompactWidget->setMaximumSize(mCompactWidget->size());
}

void TransparentMode::isInit(QString const &directory)
{
	if (!mPlugin->isMyWorkingCopy(directory, true, true)) {
		mPlugin->doInit(directory, true, true);
	}
	if (!tabIsReady) {
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
	if (widget == mCompactWidget) {
		tabIsReady = false;
		mMainWindowIface->makeFullScreen(isFullScreen);
	}
}

void TransparentMode::showDiff(const QString &newHash, const QString &oldHash, QWidget *widget)
{
	mPlugin->showDiff(oldHash, newHash, mProjectManager->saveFilePath(), widget, true);
}

void TransparentMode::showDiffAsync(const QString &newHash, const QString &oldHash, QWidget *widget)
{
	invocation::details::FunctorInterface<void> *functor =
		new invocation::details::VoidFunctor<void, TransparentMode
			, void(TransparentMode::*)(const QString&, const QString&, QWidget *)
			, const QString&, const QString&, QWidget *>(
				this
				, &TransparentMode::showDiff
				, oldHash
				, newHash
				, widget
	);

	invocation::FunctorOperation<void> *operation = new invocation::FunctorOperation<void>(functor);
	mMainWindowIface->reportOperation(operation);
	operation->invokeAsync();
}

void TransparentMode::init()
{
	mCompactWidget = new ChangeVersionWidget;
	mCompactWidget->setVisible(false);
	connect(
		mCompactWidget
		, SIGNAL(showDiff(QString, QString, QWidget *))
		, this
		, SLOT(showDiffAsync(QString,QString,QWidget*))
	);
	connect(mCompactWidget, SIGNAL(loadVersion(QString)),SLOT(setVersionAsync(QString)));
	connect(mSystemInterface, SIGNAL(otherTabClosed(QWidget *)), this, SLOT(removeBrokenPointers(QWidget *)));
	connect(mCompactWidget, SIGNAL(updateLogs()), SLOT(getAndUpdateLog()));
	tabIsReady = true;
}

void TransparentMode::listLog()
{
	if (mPlugin->isMyWorkingCopy(QString(), true, true))
	{
		if (mProjectManager->getUnsavedIndicator()){
			mProjectManager->save();
		} else {
			getAndUpdateLog();
		}
	} else {
		emit listLogIsReady(QList<QPair<QString, QString> >());
	}
}

void TransparentMode::setVersionAsync(const QString &hash)
{
	invocation::details::FunctorInterface<void> *functor =
		new invocation::details::VoidFunctor<void, TransparentMode
			, void(TransparentMode::*)(const QString&)
			, const QString&>(
				this
				, &TransparentMode::setVersion
				, hash
	);

	invocation::FunctorOperation<void> *operation = new invocation::FunctorOperation<void>(functor);
	mMainWindowIface->reportOperation(operation);
	operation->invokeAsync();
	QString currentAdress = mProjectManager->saveFilePath();
	mProjectManager->close();
	mProjectManager->open(currentAdress);
}

void TransparentMode::setVersion(const QString& hash)
{
	mPlugin->setVersion(hash, true);
}

void TransparentMode::saveVersion()
{
	isInit(mPlugin->mTempDir);
	mPlugin->beginChangesSubmitting("version was saved in a transparent mode", mProjectManager->saveFilePath(), true);
	//getAndUpdateLog();
}

QList<QPair<QString , QString> >TransparentMode::parseLog(QString log) //hash & mainPart
{
	if (log != "") {
		QStringList listLog = log.split(QChar('\n'));
		QRegExp parseHash (".*[^-]-");
		int version = listLog.size();
		int number = 0;
		QList<QPair <QString, QString> > listOfParseLog;

		while (version != 0) {
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
