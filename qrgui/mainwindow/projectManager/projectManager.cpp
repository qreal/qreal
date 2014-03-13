#include "projectManager.h"

#include <qrutils/outFile.h>
#include <qrutils/qRealFileDialog.h>

#include "mainwindow/mainWindow.h"

// TODO: lolwut?
//#include "ui_mainWindow.h"

#include "models/models.h"
#include "view/editorViewScene.h"
#include "view/editorView.h"
#include "dialogs/suggestToCreateDiagramDialog.h"

using namespace qReal;
using namespace utils;

ProjectManager::ProjectManager(MainWindow *mainWindow, TextManagerInterface *textManager)
	: mMainWindow(mainWindow)
	, mTextManager(textManager)
	, mAutosaver(new Autosaver(this))
	, mUnsavedIndicator(false)
	, mSomeProjectOpened(false)
{
	setSaveFilePath();
}

void ProjectManager::setSaveFilePath(QString const &filePath /* = "" */)
{
	mSaveFilePath = filePath.isEmpty()
			? mAutosaver->tempFilePath()
			: mAutosaver->originalFile(filePath);
}

QString ProjectManager::saveFilePath() const
{
	return mSaveFilePath;
}

void ProjectManager::reinitAutosaver()
{
	mAutosaver->reinit();
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
	QString const fileName = openFileName(tr("Open existing project"));
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
	} // QMessageBox::AcceptRole

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

bool ProjectManager::open(QString const &fileName)
{
	// 1. If Autosaver have time to save the state repository at the time of
	// testing the sufficiency of plugins to open the project, the autosave
	// file may become incompatible with the application. This will lead to
	// a fail on the next start.
	// 2. autosavePauser was first starts a timer of Autosaver
	Autosaver::Pauser const autosavePauser(*mAutosaver);
	Q_UNUSED(autosavePauser)

	if (!fileName.isEmpty() && !saveFileExists(fileName)) {
		return false;
	}

	emit beforeOpen(fileName);
	// There is no way to verify sufficiency plugins without initializing repository
	// that is stored in the save file. Initializing is impossible without closing current project.
	if (mSomeProjectOpened) {
		close();
	}
	if (mAutosaver->checkAutoSavedVersion(fileName)) {
		setUnsavedIndicator(true);
		mSomeProjectOpened = true;
		return true;
	}
	mMainWindow->models()->repoControlApi().open(fileName);
	mMainWindow->models()->reinit();

	if (!pluginsEnough()) {
		// restoring the session
		mSomeProjectOpened = open(mSaveFilePath);
		return false;
	}

	mMainWindow->closeStartTab();
	mMainWindow->propertyModel().setSourceModels(mMainWindow->models()->logicalModel()
			, mMainWindow->models()->graphicalModel());
	mMainWindow->graphicalModelExplorer()->setModel(mMainWindow->models()->graphicalModel());
	mMainWindow->logicalModelExplorer()->setModel(mMainWindow->models()->logicalModel());

	/// @todo Crashes metamodeling on fly.
	mMainWindow->openFirstDiagram();

	setSaveFilePath(fileName);
	refreshApplicationStateAfterOpen();

	emit afterOpen(fileName);

	mSomeProjectOpened = true;

	return true;
}

bool ProjectManager::suggestToImport()
{
	return import(openFileName(tr("Select file with a save to import")));
}

bool ProjectManager::import(QString const &fileName)
{
	if (fileName.isEmpty()) {
		return false;
	}
	QString const currentSaveFilePath = saveFilePath();
	if (!open(fileName)) {
		return open(currentSaveFilePath);
	}
	// In the hope that while the user selects a file nobody substitute for the current project with project, which
	// has diagrams for which there are no plugins
	mMainWindow->models()->repoControlApi().importFromDisk(currentSaveFilePath);
	mMainWindow->models()->reinit();
	return true;
}

bool ProjectManager::saveFileExists(QString const &fileName)
{
	if (!QFile::exists(fileName)) {
		fileNotFoundMessage(fileName);
		return false;
	}
	return true;
}

bool ProjectManager::pluginsEnough() const
{
	if (!missingPluginNames().isEmpty()) {
		QMessageBox::information(mMainWindow, tr("There are missing plugins")
				, tr("These plugins are not present, but needed to load the save:\n") + missingPluginNames());
		return false;
	}
	return true;
}

QString ProjectManager::missingPluginNames() const
{
	IdList const missingPlugins = mMainWindow->editorManager().checkNeededPlugins(
			mMainWindow->models()->logicalRepoApi()
			, mMainWindow->models()->graphicalRepoApi());
	QString result;
	foreach (Id const &id, missingPlugins) {
		result += id.editor() + "\n";
	}
	return result;
}

void ProjectManager::refreshApplicationStateAfterSave()
{
	if (!mAutosaver->isAutosave(mSaveFilePath)) {
		refreshApplicationStateAfterOpen();
		mMainWindow->controller()->projectSaved();
		setUnsavedIndicator(false);
	}
	mAutosaver->removeTemp();
}

void ProjectManager::refreshApplicationStateAfterOpen()
{
	refreshWindowTitleAccordingToSaveFile();
	if (!mAutosaver->isTempFile(mSaveFilePath)) {
		mMainWindow->refreshRecentProjectsList(mAutosaver->originalFile(mSaveFilePath));
	}
}

void ProjectManager::refreshWindowTitleAccordingToSaveFile()
{
	QString const windowTitle = mMainWindow->toolManager().customizer()->windowTitle();
	QString const saveFile = mAutosaver->isTempFile(mSaveFilePath) ? tr("Unsaved project") : mSaveFilePath;
	mMainWindow->setWindowTitle(windowTitle + " " + saveFile);
	refreshTitleModifiedSuffix();
}

void ProjectManager::refreshTitleModifiedSuffix()
{
	QString const modifiedSuffix = tr(" [modified]");
	if (mUnsavedIndicator && !mMainWindow->windowTitle().endsWith(modifiedSuffix)) {
		mMainWindow->setWindowTitle(mMainWindow->windowTitle() + modifiedSuffix);
	}
}

bool ProjectManager::openNewWithDiagram()
{
	if(!openEmptyWithSuggestToSaveChanges()) {
		return false;
	}
	suggestToCreateDiagram(false);
	return true;
}

bool ProjectManager::openEmptyWithSuggestToSaveChanges()
{
	if (!suggestToSaveChangesOrCancel()) {
		return false;
	}
	return open();
}

void ProjectManager::suggestToCreateDiagram(bool isClosable)
{
	Id const theOnlyDiagram = mMainWindow->editorManager().theOnlyDiagram();
	if (theOnlyDiagram != Id()) {
		Id const editor = mMainWindow->editorManager().editors()[0];
		mMainWindow->createDiagram(mMainWindow->editorManager().diagramNodeNameString(editor, theOnlyDiagram));
	} else {
		SuggestToCreateDiagramDialog suggestDialog(mMainWindow, isClosable);
		suggestDialog.exec();
	}
}

void ProjectManager::close()
{
	if (mMainWindow->propertyEditor()->model()) {
		static_cast<PropertyEditorModel *>(mMainWindow->propertyEditor()->model())->clearModelIndexes();
	}
	mMainWindow->graphicalModelExplorer()->setModel(NULL);
	mMainWindow->logicalModelExplorer()->setModel(NULL);

	if (mMainWindow->getCurrentTab()) {
		static_cast<EditorViewScene *>(mMainWindow->getCurrentTab()->scene())->clearScene();
	}
	mMainWindow->closeAllTabs();
	mMainWindow->setWindowTitle(mMainWindow->toolManager().customizer()->windowTitle());

	mAutosaver->removeAutoSave();
	mAutosaver->removeTemp();
	mSomeProjectOpened = false;

	emit closed();
}

void ProjectManager::saveTo(QString const &fileName)
{
	mMainWindow->models()->repoControlApi().saveTo(fileName);
}

void ProjectManager::save()
{
	// Do not change the method to saveAll - in the current implementation, an empty project in the repository is
	// created to initialize the file name with an empty string, which allows the internal state of the file
	// name = "" Attempt to save the project in this case result in
	mMainWindow->editorManagerProxy().saveMetamodel("");
	saveTo(mSaveFilePath);
	mAutosaver->removeAutoSave();
	refreshApplicationStateAfterSave();
}

bool ProjectManager::restoreIncorrectlyTerminated()
{
	return mAutosaver->checkTempFile();
}

bool ProjectManager::saveOrSuggestToSaveAs()
{
	if (mTextManager->saveText(false)) {
		return true;
	}

	if (mSaveFilePath == mAutosaver->tempFilePath()
			|| mSaveFilePath == mMainWindow->editorManagerProxy().saveMetamodelFilePath()) {
		return suggestToSaveAs();
	}
	save();
	return true;
}

bool ProjectManager::suggestToSaveAs()
{
	if (mTextManager->saveText(true)) {
		return true;
	}

	if (mMainWindow->editorManagerProxy().isInterpretationMode()) {
		QString const newMetamodelFileName = getSaveFileName(tr("Select file to save current metamodel to"));
		if (newMetamodelFileName.isEmpty()) {
			return false;
		}
		mMainWindow->editorManagerProxy().saveMetamodel(newMetamodelFileName);
	}
	return saveAs(getSaveFileName(tr("Select file to save current model to")));
}

bool ProjectManager::saveAs(QString const &fileName)
{
	QString const workingFileName = fileName;
	if (workingFileName.isEmpty()) {
		return false;
	}
	mAutosaver->removeAutoSave();
	mMainWindow->models()->repoControlApi().saveTo(workingFileName);
	setSaveFilePath(workingFileName);
	refreshApplicationStateAfterSave();
	return true;
}

QString ProjectManager::openFileName(QString const &dialogWindowTitle) const
{
	QString const pathToExamples = mMainWindow->toolManager().customizer()->examplesDirectory();
	QString const defaultDirectory = pathToExamples.isEmpty()
			? QFileInfo(mSaveFilePath).absoluteDir().absolutePath()
			: pathToExamples;
	QString fileName = QRealFileDialog::getOpenFileName("OpenQRSProject", mMainWindow, dialogWindowTitle
			, defaultDirectory, tr("QReal Save File(*.qrs)"));

	if (!fileName.isEmpty() && !QFile::exists(fileName)) {
		fileNotFoundMessage(fileName);
		fileName = openFileName(dialogWindowTitle);
	}
	return fileName;
}

QString ProjectManager::getSaveFileName(QString const &dialogWindowTitle)
{
	QString fileName = QRealFileDialog::getSaveFileName("SaveQRSProject", mMainWindow, dialogWindowTitle
			, QFileInfo(mSaveFilePath).absoluteDir().absolutePath(), tr("QReal Save File(*.qrs)"));

	if (!fileName.isEmpty() && !fileName.endsWith(".qrs", Qt::CaseInsensitive)) {
		fileName += ".qrs";
	}
	return fileName;
}

void ProjectManager::setUnsavedIndicator(bool isUnsaved)
{
	mUnsavedIndicator = isUnsaved;
	refreshWindowTitleAccordingToSaveFile();
}

void ProjectManager::fileNotFoundMessage(QString const &fileName) const
{
	QMessageBox::information(mMainWindow, tr("File not found")
			, tr("File ") + fileName + tr(" not found. Try again"));
}
