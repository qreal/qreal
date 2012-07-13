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

/// Try to open save file with name fileName, show messages is file non exist or plugins are missing and
/// return false, or return true otherwise. fileName == "" will be create an empty project.
bool ProjectManager::open(QString const &fileName)
{
	if (fileName != "" && !saveFileExists(fileName)) {
		return false;
	}
	// There is no way to verify sufficiency plugins without initializing repository
	// that is stored in the save file. Iinitializing is impossible without closing current project.
	mMainWindow->closeProject();
	mMainWindow->mModels->repoControlApi().open(fileName);
	mMainWindow->mModels->reinit();

	if (!pluginsEnough()) {
		open(mMainWindow->mSaveFile);
		return false;
	}
	mMainWindow->refreshRecentProjectsList(fileName);

	mMainWindow->mPropertyModel.setSourceModels(mMainWindow->mModels->logicalModel()
			, mMainWindow->mModels->graphicalModel());
	mMainWindow->mUi->graphicalModelExplorer->setModel(mMainWindow->mModels->graphicalModel());
	mMainWindow->mUi->logicalModelExplorer->setModel(mMainWindow->mModels->logicalModel());

	mMainWindow->mSaveFile = fileName;
	refreshWindowTitleAccordingToSaveFile();

	return true;
}

void ProjectManager::refreshWindowTitleAccordingToSaveFile()
{
	mMainWindow->connectWindowTitle();
	QString windowTitle = mMainWindow->mToolManager.customizer()->windowTitle();
	if (mMainWindow->mSaveFile.isEmpty()) {
		mMainWindow->setWindowTitle(windowTitle + " - unsaved project");
	} else {
		mMainWindow->setWindowTitle(windowTitle + " - " + mMainWindow->mSaveFile);
	}
}

bool ProjectManager::saveFileExists(QString const &fileName)
{
	if (!QFile::exists(fileName)) {
		QMessageBox fileNotFoundMessage(QMessageBox::Information, tr("File not found")
				, tr("File ") + fileName + tr(" not found. Try again"), QMessageBox::Ok, mMainWindow);
		fileNotFoundMessage.exec();
		return false;
	}
	return true;
}

bool ProjectManager::pluginsEnough()
{
	if (!missingPluginNames().isEmpty()) {
		QMessageBox thereAreMissingPluginsMessage(QMessageBox::Information, tr("There are missing plugins"),
				tr("These plugins are not present, but needed to load the save:\n") +
						mMainWindow->missingPluginNames(),
				QMessageBox::Ok, mMainWindow);
		thereAreMissingPluginsMessage.exec();
		return false;
	}
	return true;
}

QString ProjectManager::missingPluginNames()
{
	IdList missingPlugins = mMainWindow->mEditorManager.checkNeededPlugins(
			mMainWindow->mModels->logicalRepoApi(), mMainWindow->mModels->graphicalRepoApi());
	QString result;
	foreach (Id const &id, missingPlugins) {
		result += id.editor() + "\n";
	}
	return result;
}
