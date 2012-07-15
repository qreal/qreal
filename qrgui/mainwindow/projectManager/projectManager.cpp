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
	, mAutosaver(new Autosaver(this))
	, mUnsavedIndicator(false)
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
	if (!mUnsavedIndicator) {
		return true;
	}
	switch (suggestToSaveOrCancelMessage()) {
	case QMessageBox::DestructiveRole:
		return true;
	case QMessageBox::RejectRole:
		return false;
	}
	// QMessageBox::AcceptRole
	return saveOrSuggestToSaveAs();
}

int ProjectManager::suggestToSaveOrCancelMessage()
{
	QMessageBox offerSave(mMainWindow);
	offerSave.setWindowTitle(tr("Save"));
	offerSave.addButton(tr("&Save"), QMessageBox::AcceptRole);
	offerSave.addButton(tr("&Cancel"), QMessageBox::RejectRole);
	offerSave.addButton(tr("&Discard"), QMessageBox::DestructiveRole);
	offerSave.setText(tr("Do you want to save current project?"));
	return offerSave.exec();
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
		open(mSaveFilePath);
		return false;
	}

	mMainWindow->mPropertyModel.setSourceModels(mMainWindow->mModels->logicalModel()
			, mMainWindow->mModels->graphicalModel());
	mMainWindow->mUi->graphicalModelExplorer->setModel(mMainWindow->mModels->graphicalModel());
	mMainWindow->mUi->logicalModelExplorer->setModel(mMainWindow->mModels->logicalModel());

	mSaveFilePath = fileName;
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
	setUnsavedIndicator(false);

	refreshWindowTitleAccordingToSaveFile();
	mMainWindow->refreshRecentProjectsList(mSaveFilePath);
	SettingsManager::setValue("saveFile", mSaveFilePath);
}

void ProjectManager::refreshApplicationStateAfterOpen()
{
	refreshWindowTitleAccordingToSaveFile();
	mMainWindow->refreshRecentProjectsList(mSaveFilePath);
	SettingsManager::setValue("saveFile", mSaveFilePath);
}

void ProjectManager::refreshWindowTitleAccordingToSaveFile()
{
	mMainWindow->connectWindowTitle();
	QString windowTitle = mMainWindow->mToolManager.customizer()->windowTitle();
	if (mSaveFilePath.isEmpty()) {
		mMainWindow->setWindowTitle(windowTitle + " - unsaved project");
	} else {
		mMainWindow->setWindowTitle(windowTitle + " - " + mSaveFilePath);
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

bool ProjectManager::save()
{
	if (mSaveFilePath.isEmpty()) {
		return false;
	}
	mMainWindow->mModels->repoControlApi().saveAll();
	refreshApplicationStateAfterSave();
	return true;
}

bool ProjectManager::saveOrSuggestToSaveAs()
{
	if (!save()) {
		return suggestToSaveAs();
	}
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
	mSaveFilePath = workingFileName;
	refreshApplicationStateAfterSave();
	return true;
}

QString ProjectManager::getOpenFileName(QString const &dialogWindowTitle)
{
	QString fileName = QFileDialog::getOpenFileName(mMainWindow, dialogWindowTitle
			, QFileInfo(mSaveFilePath).absoluteDir().absolutePath(), tr("QReal Save File(*.qrs)"));

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
			, QFileInfo(mSaveFilePath).absoluteDir().absolutePath(), tr("QReal Save File(*.qrs)"));

	if (!fileName.isEmpty() && !fileName.endsWith(".qrs", Qt::CaseInsensitive)) {
		fileName += ".qrs";
	}
	return fileName;
}

void ProjectManager::setUnsavedIndicator(bool isUnsaved)
{
	if (isUnsaved && !mUnsavedIndicator) {
		mMainWindow->setWindowTitle(mMainWindow->windowTitle() + " [modified]");
	}
	mUnsavedIndicator = isUnsaved;
}

void ProjectManager::reinitAutosaver()
{
	mAutosaver->reinit();
}
