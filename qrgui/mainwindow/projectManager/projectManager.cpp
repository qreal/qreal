#include "../mainWindow.h"
#include "ui_mainWindow.h"
#include "../models/models.h"
#include "../view/editorViewScene.h"
#include "../view/editorView.h"
#include "projectManager.h"

using namespace qReal;

ProjectManager::ProjectManager(MainWindow *mainWindow)
	: QObject(mainWindow)
	, mMainWindow(mainWindow)
{
}

bool ProjectManager::openEmptyProject()
{
	if (!suggestToSaveChangesOrCancel()) {
		return false;
	}
	return open();
}

bool ProjectManager::suggestToSaveChangesOrCancel()
{
	if (!mMainWindow->mUnsavedProjectIndicator) {
		return true;
	}
	switch (mMainWindow->suggestToSaveProject()) {
	case QMessageBox::DestructiveRole:
		return true;
	case QMessageBox::RejectRole:
		return false;
	}
	saveAll();
	return true;  // QMessageBox::AcceptRole
}

/// Try to open save file with name fileName, show messages is file non exist or plugins are missing and
/// return false, or return true otherwise. fileName == "" (default value) will be create an empty project.
bool ProjectManager::open(QString const &fileName)
{
	if (fileName != "" && !saveFileExists(fileName)) {
		return false;
	}
	// There is no way to verify sufficiency plugins without initializing repository
	// that is stored in the save file. Iinitializing is impossible without closing current project.
	close();
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

void ProjectManager::close()
{
	if (mMainWindow->mUi->propertyEditor->model() != NULL) {
		static_cast<PropertyEditorModel *>(mMainWindow->mUi->propertyEditor->model())->clearModelIndexes();
	}
	mMainWindow->mUi->graphicalModelExplorer->setModel(NULL);
	mMainWindow->mUi->logicalModelExplorer->setModel(NULL);

	if (mMainWindow->getCurrentTab()) {
		static_cast<EditorViewScene *>(mMainWindow->getCurrentTab()->scene())->clearScene();
	}
	mMainWindow->closeAllTabs();
	mMainWindow->setWindowTitle(mMainWindow->mToolManager.customizer()->windowTitle());
}

void ProjectManager::saveAll()
{
	if (mMainWindow->mSaveFile.isEmpty()) {
		suggestToSaveAs();
		return;
	}
	mMainWindow->mModels->repoControlApi().saveAll();
	refreshApplicationStateAfterSave();
}

void ProjectManager::suggestToSaveAs()
{
	saveAs(mMainWindow->getWorkingFile(tr("Select file to save current model to"), true));
}

void ProjectManager::saveAs(QString const &fileName)
{
	QString workingFileName = fileName;

	if (workingFileName.isEmpty()) {
		return;
	}
	if (!workingFileName.endsWith(".qrs", Qt::CaseInsensitive)) {
		workingFileName += ".qrs";
	}
	mMainWindow->mModels->repoControlApi().saveTo(workingFileName);
	mMainWindow->mSaveFile = workingFileName;
	refreshApplicationStateAfterSave();
}

void ProjectManager::refreshApplicationStateAfterSave()
{
	mMainWindow->mUnsavedProjectIndicator = false;
	refreshWindowTitleAccordingToSaveFile();
	SettingsManager::setValue("saveFile", mMainWindow->mSaveFile);
}
