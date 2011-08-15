#include "diffmanager.h"
#include "diffview/diffform.h"
#include "../view/editorview.h"
#include "../mainwindow/mainwindow.h"
#include "details/diffprovider.h"

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

	DiffForm diffForm(mMainWindow, new DiffProvider(mRepoModel, mWorkingCopyModel), NULL);
	diffForm.exec();


	delete mWorkingCopyModel;
	delete mRepoModel;
	return true;
}

QString DiffManager::createRepoModel(const QString &workingCopy)
{
	QString repoUrl = mWorkingCopyModel->repoControlApi().repoUrl(workingCopy);
	mErrors.append(mWorkingCopyModel->repoControlApi().newErrors());
	if (repoUrl == "")
		return "";
	QSettings settings("SPbSU", "QReal");
	QString checkoutDir = settings.value("diffCheckoutPath", QDir::currentPath()).toString()+"/CheckoutForDiff";
	bool ok = mWorkingCopyModel->repoControlApi().doCheckout(repoUrl, checkoutDir);
	mErrors.append(mWorkingCopyModel->repoControlApi().newErrors());
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
