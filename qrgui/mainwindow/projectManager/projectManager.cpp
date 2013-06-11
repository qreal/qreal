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
		: mMainWindow(mainWindow)
		, mAutosaver(new Autosaver(this))
		, mUnsavedIndicator(false)
{
	setSaveFilePath();
}

void ProjectManager::setSaveFilePath(QString const &filePath /* = "" */)
{
	if (filePath.isEmpty()) {
		mSaveFilePath = mAutosaver->filePath();
	} else {
		mSaveFilePath = filePath;
	}
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
	// 1. If Autosaver have time to save the state repository at the time of testing the sufficiency of plugins to open
	// the project, the autosave file may become incompatible with the application. This will lead to a fail on the
	// next start. 2. autosavePauser was first starts a timer of Autosaver
	Autosaver::Pauser autosavePauser = mAutosaver->pauser();

	if (!fileName.isEmpty() && !saveFileExists(fileName)) {
		return false;
	}

	emit beforeOpen(fileName);
	// There is no way to verify sufficiency plugins without initializing repository
	// that is stored in the save file. Initializing is impossible without closing current project.
	close();
	mMainWindow->models()->repoControlApi().open(fileName);
	mMainWindow->models()->reinit();
	saveTemp();

	if (!pluginsEnough()) {
		// restoring the session
		open(mSaveFilePath);
		return false;
	}
	mMainWindow->propertyModel().setSourceModels(mMainWindow->models()->logicalModel()
			, mMainWindow->models()->graphicalModel());
	mMainWindow->graphicalModelExplorer()->setModel(mMainWindow->models()->graphicalModel());
	mMainWindow->logicalModelExplorer()->setModel(mMainWindow->models()->logicalModel());
	mMainWindow->openFirstDiagram();

	setSaveFilePath(fileName);
	refreshApplicationStateAfterOpen();

	emit afterOpen(fileName);

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
	QString currentSaveFilePath = saveFilePath();
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

QString ProjectManager::missingPluginNames() const
{
	IdList missingPlugins = mMainWindow->editorManager().checkNeededPlugins(
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
	if (mSaveFilePath != mAutosaver->filePath()) {
		mMainWindow->controller()->projectSaved();
	}
}

void ProjectManager::refreshApplicationStateAfterOpen()
{
	refreshWindowTitleAccordingToSaveFile();
	mMainWindow->refreshRecentProjectsList(mSaveFilePath);
}

void ProjectManager::refreshWindowTitleAccordingToSaveFile()
{
	QString const windowTitle = mMainWindow->toolManager().customizer()->windowTitle();
	mMainWindow->setWindowTitle(windowTitle + " " + mSaveFilePath);
	refreshTitleModifiedSuffix();
}

void ProjectManager::refreshTitleModifiedSuffix()
{
	QString modifiedSuffix = tr(" [modified]");
	if (mUnsavedIndicator && !mMainWindow->windowTitle().endsWith(modifiedSuffix)) {
		mMainWindow->setWindowTitle(mMainWindow->windowTitle() + modifiedSuffix);
	}
}

bool ProjectManager::openNewWithDiagram()
{
	clearAutosaveFile();
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

void ProjectManager::save()
{
	// Do not change the method to saveAll - in the current implementation, an empty project in the repository is
	// created to initialize the file name with an empty string, which allows the internal state of the file
	// name = "" Attempt to save the project in this case result in
	// qDebug() << "start save";
	mMainWindow->editorManagerProxy().saveMetamodel("");
	mMainWindow->models()->repoControlApi().saveTo(mSaveFilePath);
	refreshApplicationStateAfterSave();
}

void ProjectManager::saveTemp()
{
	mSaveFilePathTemp=SettingsManager::value("AutosaveTempFile").toString();
	mMainWindow->models()->repoControlApi().saveTo(mSaveFilePathTemp);
}

void ProjectManager::saveGenCode(QString const &text)
{
	utils::OutFile out("nxt-tools/example0/example0.c");
	out() << text;
}

bool ProjectManager::saveOrSuggestToSaveAs()
{
	if (mSaveFilePath.contains(SettingsManager::value("AutosaveFileName").toString(), Qt::CaseSensitive)
		|| mSaveFilePath == mMainWindow->editorManagerProxy().saveMetamodelFilePath()) {
		return suggestToSaveAs();
	}
	save();
	return true;
}

bool ProjectManager::suggestToSaveAs()
{
	if (mMainWindow->editorManagerProxy().isInterpretationMode()) {
		QString newMetamodelFileName = getSaveFileName(tr("Select file to save current metamodel to"));
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
	mMainWindow->models()->repoControlApi().saveTo(workingFileName);
	mSaveFilePath = workingFileName;
	refreshApplicationStateAfterSave();
	return true;
}

QString ProjectManager::openFileName(QString const &dialogWindowTitle) const
{
	QString fileName = QFileDialog::getOpenFileName(mMainWindow, dialogWindowTitle
			, QFileInfo(mSaveFilePath).absoluteDir().absolutePath(), tr("QReal Save File(*.qrs)"));

	if (!fileName.isEmpty() && !QFile::exists(fileName)) {
		QMessageBox fileNotFoundMessage(QMessageBox::Information, tr("File not found")
				, tr("File ") + fileName + tr(" not found. Try again"), QMessageBox::Ok, mMainWindow);
		fileNotFoundMessage.exec();

		fileName = openFileName(dialogWindowTitle);
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
	mUnsavedIndicator = isUnsaved;
	refreshWindowTitleAccordingToSaveFile();
}

void ProjectManager::clearAutosaveFile()
{
	QFile tempFile(SettingsManager::value("AutosaveFileName").toString());
	if (tempFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		tempFile.close();
	}
}
