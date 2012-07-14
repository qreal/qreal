#include "../mainWindow.h"
#include "ui_mainWindow.h"
#include "../models/models.h"
#include "../view/editorViewScene.h"
#include "../view/editorView.h"
#include "../dialogs/suggestToCreateDiagramDialog.h"
#include "projectManager.h"

using namespace qReal;

ProjectManager::ProjectManager(MainWindow *mainWindow)
	: QObject(mainWindow)
	, mMainWindow(mainWindow)
{
}

bool ProjectManager::openExisting(QString const &fileName)
{
	if (!suggestToSaveChangesOrCancel()) {
		return false;
	}
	return open(fileName);
}

bool ProjectManager::suggestToOpenExisting()
{
	if (!suggestToSaveChangesOrCancel()) {
		return false;
	}
	QString fileName = getOpenFileName(tr("Open existing project"));
	if (fileName.isEmpty()) {
		return false;
	}
	return open(fileName);
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
	// QMessageBox::AcceptRole
	return saveAll();
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
		// restoring the session
		open(mMainWindow->mSaveFile);
		return false;
	}

	mMainWindow->mPropertyModel.setSourceModels(mMainWindow->mModels->logicalModel()
			, mMainWindow->mModels->graphicalModel());
	mMainWindow->mUi->graphicalModelExplorer->setModel(mMainWindow->mModels->graphicalModel());
	mMainWindow->mUi->logicalModelExplorer->setModel(mMainWindow->mModels->logicalModel());

	mMainWindow->mSaveFile = fileName;
	refreshApplicationStateAfterOpen();

	return true;
}

bool ProjectManager::suggestToimport()
{
	return import(getOpenFileName(tr("Select file with a save to import")));
}

bool ProjectManager::import(QString const &fileName)
{
	if (!QFile(fileName).exists()) {
		return false;
	}
	mMainWindow->mModels->repoControlApi().importFromDisk(fileName);
	mMainWindow->mModels->reinit();
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
						missingPluginNames(),
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

void ProjectManager::refreshApplicationStateAfterSave()
{
	mMainWindow->mUnsavedProjectIndicator = false;

	refreshWindowTitleAccordingToSaveFile();
	mMainWindow->refreshRecentProjectsList(mMainWindow->mSaveFile);
	SettingsManager::setValue("saveFile", mMainWindow->mSaveFile);
}

void ProjectManager::refreshApplicationStateAfterOpen()
{
	refreshWindowTitleAccordingToSaveFile();
	mMainWindow->refreshRecentProjectsList(mMainWindow->mSaveFile);
	SettingsManager::setValue("saveFile", mMainWindow->mSaveFile);
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

bool ProjectManager::openNewWithDiagram()
{
	if(!openEmptyWithSuggestToSaveChanges()) {
		return false;
	}
	suggestToCreateDiagram(true);
	return true;
}

bool ProjectManager::openEmptyWithSuggestToSaveChanges()
{
	if (!suggestToSaveChangesOrCancel()) {
		return false;
	}
	return open();
}

void ProjectManager::suggestToCreateDiagram(bool isNonClosable)
{
	SuggestToCreateDiagramDialog suggestDialog(mMainWindow, isNonClosable);
	suggestDialog.exec();
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

bool ProjectManager::saveAll()
{
	if (mMainWindow->mSaveFile.isEmpty()) {
		return suggestToSaveAs();
	}
	mMainWindow->mModels->repoControlApi().saveAll();
	refreshApplicationStateAfterSave();
	return true;
}

bool ProjectManager::suggestToSaveAs()
{
	return saveAs(getSaveFileName(tr("Select file to save current model to")));
}

bool ProjectManager::saveAs(QString const &fileName)
{
	QString workingFileName = fileName;

	if (workingFileName.isEmpty()) {
		return false;
	}
	mMainWindow->mModels->repoControlApi().saveTo(workingFileName);
	mMainWindow->mSaveFile = workingFileName;
	refreshApplicationStateAfterSave();
	return true;
}

QString ProjectManager::getOpenFileName(QString const &dialogWindowTitle)
{
	QString fileName = QFileDialog::getOpenFileName(mMainWindow, dialogWindowTitle
			, QFileInfo(mMainWindow->mSaveFile).absoluteDir().absolutePath(), tr("QReal Save File(*.qrs)"));

	if (!fileName.isEmpty() && !QFile::exists(fileName)) {
		QMessageBox fileNotFoundMessage(QMessageBox::Information, tr("File not found"),
				tr("File ") + fileName + tr(" not found. Try again"), QMessageBox::Ok, mMainWindow);
		fileNotFoundMessage.exec();

		fileName = getOpenFileName(dialogWindowTitle);
	}
	return fileName;
}

QString ProjectManager::getSaveFileName(QString const &dialogWindowTitle)
{
	QString fileName = QFileDialog::getSaveFileName(mMainWindow, dialogWindowTitle
			, QFileInfo(mMainWindow->mSaveFile).absoluteDir().absolutePath(), tr("QReal Save File(*.qrs)"));

	if (!fileName.isEmpty() && !fileName.endsWith(".qrs", Qt::CaseInsensitive)) {
		fileName += ".qrs";
	}
	return fileName;
}
