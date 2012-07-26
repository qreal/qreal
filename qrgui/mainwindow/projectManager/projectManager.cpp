#include "../mainWindow.h"
#include "ui_mainWindow.h"
#include "../models/models.h"
#include "../view/editorViewScene.h"
#include "../view/editorView.h"
#include "../dialogs/suggestToCreateDiagramDialog.h"
#include "projectManager.h"
#include "../../../../qrutils/outFile.h"

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
	QString const fileName = getOpenFileName(tr("Open existing project"));
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

bool ProjectManager::open(QString const &fileName)
{
	if (!fileName.isEmpty() && !saveFileExists(fileName)) {
		return false;
	}
	// There is no way to verify sufficiency plugins without initializing repository
	// that is stored in the save file. Iinitializing is impossible without closing current project.
	close();
	mMainWindow->models()->repoControlApi().open(fileName);
	mMainWindow->models()->reinit();

	if (!pluginsEnough()) {
		// restoring the session
		open(mSaveFilePath);
		return false;
	}

	mMainWindow->propertyModel().setSourceModels(mMainWindow->models()->logicalModel()
			, mMainWindow->models()->graphicalModel());
	mMainWindow->graphicalModelExplorer()->setModel(mMainWindow->models()->graphicalModel());
	mMainWindow->logicalModelExplorer()->setModel(mMainWindow->models()->logicalModel());

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
	mMainWindow->models()->repoControlApi().importFromDisk(fileName);
	mMainWindow->models()->reinit();
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
		QMessageBox thereAreMissingPluginsMessage(QMessageBox::Information, tr("There are missing plugins")
				, tr("These plugins are not present, but needed to load the save:\n") + missingPluginNames()
				, QMessageBox::Ok, mMainWindow);
		thereAreMissingPluginsMessage.exec();
		return false;
	}
	return true;
}

QString ProjectManager::missingPluginNames()
{
	IdList missingPlugins = mMainWindow->manager()->checkNeededPlugins(
			mMainWindow->models()->logicalRepoApi(), mMainWindow->models()->graphicalRepoApi());
	QString result;
	foreach (Id const &id, missingPlugins) {
		result += id.editor() + "\n";
	}
	return result;
}

void ProjectManager::refreshApplicationStateAfterSave()
{
	refreshApplicationStateAfterOpen();
	setUnsavedIndicator(false);
}

void ProjectManager::refreshApplicationStateAfterOpen()
{
	refreshWindowTitleAccordingToSaveFile();
	mMainWindow->refreshRecentProjectsList(mSaveFilePath);
}

void ProjectManager::refreshWindowTitleAccordingToSaveFile()
{
	mMainWindow->connectWindowTitle();
	QString const windowTitle = mMainWindow->toolManager().customizer()->windowTitle();
	if (mSaveFilePath.isEmpty()) {
		mMainWindow->setWindowTitle(windowTitle + " unsaved project");
	} else {
		mMainWindow->setWindowTitle(windowTitle + " " + mSaveFilePath);
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
	if (mMainWindow->propertyEditor()->model() != NULL) {
		static_cast<PropertyEditorModel *>(mMainWindow->propertyEditor()->model())->clearModelIndexes();
	}
	mMainWindow->graphicalModelExplorer()->setModel(NULL);
	mMainWindow->logicalModelExplorer()->setModel(NULL);

	if (mMainWindow->getCurrentTab()) {
		static_cast<EditorViewScene *>(mMainWindow->getCurrentTab()->scene())->clearScene();
	}
	mMainWindow->closeAllTabs();
	mMainWindow->setWindowTitle(mMainWindow->toolManager().customizer()->windowTitle());
}

bool ProjectManager::save()
{
	if (mSaveFilePath.isEmpty()) {
		return false;
	}
	mMainWindow->models()->repoControlApi().saveAll();
	refreshApplicationStateAfterSave();
	return true;
}

void ProjectManager::saveGenCode(QString const &text)
{
	utils::OutFile out("nxt-tools/example0/example0.c");
	out() << text;
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
	QString const workingFileName = fileName;
	if (workingFileName.isEmpty()) {
		return false;
	}
	mMainWindow->models()->repoControlApi().saveTo(workingFileName);
	mSaveFilePath = workingFileName;
	refreshApplicationStateAfterSave();
	return true;
}

QString ProjectManager::getOpenFileName(QString const &dialogWindowTitle)
{
	QString fileName = QFileDialog::getOpenFileName(mMainWindow, dialogWindowTitle
			, QFileInfo(mSaveFilePath).absoluteDir().absolutePath(), tr("QReal Save File(*.qrs)"));

	if (!fileName.isEmpty() && !QFile::exists(fileName)) {
		QMessageBox fileNotFoundMessage(QMessageBox::Information, tr("File not found")
				, tr("File ") + fileName + tr(" not found. Try again"), QMessageBox::Ok, mMainWindow);
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
		mMainWindow->setWindowTitle(mMainWindow->windowTitle() + tr(" [modified]"));
	}
	mUnsavedIndicator = isUnsaved;
}

void ProjectManager::reinitAutosaver()
{
	mAutosaver->reinit();
}
