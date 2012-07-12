#include "../mainWindow.h"
#include "ui_mainWindow.h"
#include "../models/models.h"
#include "projectManager.h"

using namespace qReal;

ProjectManager::ProjectManager(MainWindow *mainWindow)
	: QObject(mainWindow)
	, mMainWindow(mainWindow)
{
}

bool ProjectManager::open(QString const &fileName)
{
	if (!QFile::exists(fileName) && fileName != "") {
		QMessageBox fileNotFoundMessage(QMessageBox::Information, tr("File not found")
				, tr("File ") + fileName + tr(" not found. Try again")
				, QMessageBox::Ok, mMainWindow);
		fileNotFoundMessage.exec();
		return false;
	}

	mMainWindow->closeProject();

	mMainWindow->mModels->repoControlApi().open(fileName);
	mMainWindow->mModels->reinit();

	if (!mMainWindow->missingPluginNames().isEmpty()) {
		QMessageBox thereAreMissingPluginsMessage(
				QMessageBox::Information, tr("There are missing plugins"),
				tr("These plugins are not present, but needed to load the save:\n") +
						mMainWindow->missingPluginNames(),
				QMessageBox::Ok, mMainWindow);
		thereAreMissingPluginsMessage.exec();
		open(mMainWindow->mSaveFile);
		return false;
	}

	mMainWindow->mPropertyModel.setSourceModels(
			mMainWindow->mModels->logicalModel(), mMainWindow->mModels->graphicalModel());
	mMainWindow->mUi->graphicalModelExplorer->setModel(mMainWindow->mModels->graphicalModel());
	mMainWindow->mUi->logicalModelExplorer->setModel(mMainWindow->mModels->logicalModel());

	mMainWindow->connectWindowTitle();

	QString windowTitle = mMainWindow->mToolManager.customizer()->windowTitle();
	if (fileName.isEmpty()) {
		mMainWindow->setWindowTitle(windowTitle + " - unsaved project");
	} else {
		mMainWindow->setWindowTitle(windowTitle + " - " + fileName);
		mMainWindow->refreshRecentProjectsList(fileName);
	}
	mMainWindow->mSaveFile = fileName;

	return true;
}
