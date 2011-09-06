#include "diffManager.h"
#include "diffView/diffForm.h"
#include "../view/editorView.h"
#include "../mainwindow/mainwindow.h"
#include "details/diffProvider.h"

#include <QSettings>

using namespace qReal;
using namespace qReal::diffManager;
using namespace qReal::diffManager::diffView;
using namespace qReal::diffManager::details;
using namespace qReal::models;

DiffManager::DiffManager(MainWindow *mainWindow)
	: mMainWindow(mainWindow)
{
}

bool DiffManager::showDiff(QString const &workingDir)
{
	mWorkingCopyModel = new Models(workingDir, *(mMainWindow->manager()));
	mWorkingCopyModel->repoControlApi().open(workingDir);
	mWorkingCopyModel->reinit();

	QString repoModelDir = createRepoModel(workingDir);
	if (repoModelDir == "")
	{
		delete mWorkingCopyModel;
		return false;
	}

	DiffForm diffForm(mMainWindow, new DiffProvider(mRepoModel, mWorkingCopyModel), mMainWindow);
	diffForm.exec();

	delete mWorkingCopyModel;
	delete mRepoModel;
	removeDir(repoModelDir);
	return true;
}

QString DiffManager::createRepoModel(const QString &workingCopy)
{
	QString repoUrl = mWorkingCopyModel->versionControlSystemApi().repoUrl(workingCopy);
	mErrors.append(mWorkingCopyModel->versionControlSystemApi().newErrors());
	if (repoUrl == "")
		return "";
	QSettings settings("SPbSU", "QReal");
	QString checkoutDir = settings.value("diffCheckoutPath", QDir::currentPath()).toString()+"/CheckoutForDiff";
	// Checkout operation doesn`t work if checkout directory is not empty
	if (!removeDir(checkoutDir))
	{
		mErrors.append("Can`t remove directory " + checkoutDir);
		return "";
	}
	QDir dir;
	if (!dir.mkpath(checkoutDir))
	{
		mErrors.append("Can`t create directory " + checkoutDir);
		return "";
	}
	bool ok = mWorkingCopyModel->versionControlSystemApi().doCheckout(repoUrl, checkoutDir);
	mErrors.append(QStringList(mWorkingCopyModel->versionControlSystemApi().newErrors()));
	if (!ok)
		return "";
	mRepoModel = new Models(checkoutDir, *(mMainWindow->manager()));
	mRepoModel->repoControlApi().open(checkoutDir);
	mRepoModel->reinit();
	return checkoutDir;
}

QStringList DiffManager::newErrors()
{
	QStringList result(mErrors);
	mErrors.clear();
	return result;
}

bool DiffManager::clearDir(QString const &path)
{
	QDir dir(path);
	if (!dir.exists())
	{
		return true;
	}
	foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden))
	{
		if (fileInfo.isDir())
		{
			if (!clearDir(fileInfo.filePath()) || !dir.rmdir(fileInfo.fileName()))
			{
				return false;
			}
		}
		else
		{
			QFile file(fileInfo.filePath());
			// For removing readonly files (in .svn directory)
			if (!file.setPermissions(QFile::ReadOther | QFile::WriteOther | QFile::ExeOther))
			{
				return false;
			}
			if (!file.remove())
			{
				return false;
			}
		}
	}
	return true;
}

bool DiffManager::removeDir(QString const &path)
{
	QDir dir(path);
	if (!dir.exists())
	{
		return true;
	}
	if (!clearDir(path))
	{
		return false;
	}
	return dir.rmdir(path);
}
