/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "projectManagerWrapper.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTreeView>

#include <qrkernel/platformInfo.h>
#include <qrutils/qRealFileDialog.h>

#include "mainWindow/mainWindow.h"

#include <qrgui/dialogs/projectManagement/suggestToCreateDiagramDialog.h>
#include <qrgui/editor/editorView.h>
#include <qrgui/editor/editorViewScene.h>
#include <qrgui/editor/propertyEditorView.h>
#include <qrgui/models/models.h>
#include <qrgui/models/propertyEditorModel.h>
#include <qrgui/plugins/pluginManager/toolPluginManager.h>
#include <qrgui/plugins/pluginManager/proxyEditorManager.h>

using namespace qReal;
using namespace utils;

ProjectManagerWrapper::ProjectManagerWrapper(MainWindow *mainWindow, TextManagerInterface *textManager)
	: ProjectManager(mainWindow->models())
	, mMainWindow(mainWindow)
	, mTextManager(textManager)
	, mVersionsConverter(*mMainWindow)
{
	setSaveFilePath();
}

bool ProjectManagerWrapper::suggestToSaveChangesOrCancel()
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

int ProjectManagerWrapper::suggestToSaveOrCancelMessage()
{
	QMessageBox offerSave(mMainWindow);
	offerSave.setWindowTitle(tr("Save"));
	offerSave.addButton(tr("&Save"), QMessageBox::AcceptRole);
	offerSave.addButton(tr("&Cancel"), QMessageBox::RejectRole);
	offerSave.addButton(tr("&Discard"), QMessageBox::DestructiveRole);
	offerSave.setText(tr("Do you want to save current project?"));
	return offerSave.exec();
}

void ProjectManagerWrapper::showMessage(const QString &title, const QString &message) const
{
	QMessageBox::information(mMainWindow, title, message);
}

bool ProjectManagerWrapper::askQuestion(const QString &title, const QString &question) const
{
	return QMessageBox::question(mMainWindow, title, question) == QMessageBox::Yes;
}

bool ProjectManagerWrapper::open(const QString &fileName)
{
	const QString dequotedFileName = (fileName.startsWith("'") && fileName.endsWith("'"))
			|| (fileName.startsWith("\"") && fileName.endsWith("\""))
					? fileName.mid(1, fileName.length() - 2)
					: fileName;

	const QFileInfo fileInfo(dequotedFileName);

	if (fileInfo.suffix() == "qrs" || fileInfo.completeBaseName().isEmpty()) {
		if (!dequotedFileName.isEmpty() && !saveFileExists(dequotedFileName)) {
			return false;
		}

		return openProject(dequotedFileName);
	} else if (fileInfo.exists()) {
		mMainWindow->closeStartTab();
		mTextManager->showInTextEditor(fileInfo, text::Languages::pickByExtension(fileInfo.suffix()));
	}

	return true;
}

QString ProjectManagerWrapper::textFileFilters() const
{
	QStringList result;
	for (const text::LanguageInfo &language : text::Languages::knownLanguages()) {
		result << QString("%1 (*.%2)").arg(language.extensionDescription, language.extension);
	}

	return result.join(";;");
}

bool ProjectManagerWrapper::checkVersions()
{
	return mVersionsConverter.validateCurrentProject();
}

void ProjectManagerWrapper::refreshApplicationStateAfterSave()
{
	if (!mAutosaver.isAutosave(mSaveFilePath)) {
		refreshApplicationStateAfterSaveOrOpen();
		mMainWindow->controller()->projectSaved();
		setUnsavedIndicator(false);
	}

	mAutosaver.removeTemp();
}

void ProjectManagerWrapper::refreshApplicationStateAfterOpen()
{
	mMainWindow->closeStartTab();
	mMainWindow->propertyModel().setSourceModels(mMainWindow->models().logicalModel()
			, mMainWindow->models().graphicalModel());
	mMainWindow->graphicalModelExplorer()->setModel(mMainWindow->models().graphicalModel());
	mMainWindow->logicalModelExplorer()->setModel(mMainWindow->models().logicalModel());

	/// @todo Crashes metamodeling on fly.
	mMainWindow->openFirstDiagram();

	refreshApplicationStateAfterSaveOrOpen();
}

void ProjectManagerWrapper::refreshApplicationStateAfterSaveOrOpen()
{
	refreshWindowTitleAccordingToSaveFile();
	if (!mAutosaver.isTempFile(mSaveFilePath)) {
		mMainWindow->refreshRecentProjectsList(mAutosaver.originalFile(mSaveFilePath));
	}
}

void ProjectManagerWrapper::refreshWindowTitleAccordingToSaveFile()
{
	const QString windowTitle = mMainWindow->toolManager().customizer()->windowTitle();
	const QString saveFile = mAutosaver.isTempFile(mSaveFilePath) ? tr("Unsaved project") : mSaveFilePath;
	mMainWindow->setWindowTitle(windowTitle + " " + saveFile);
	refreshTitleModifiedSuffix();
}

void ProjectManagerWrapper::refreshTitleModifiedSuffix()
{
	const QString modifiedSuffix = tr(" [modified]");
	if (mUnsavedIndicator && !mMainWindow->windowTitle().endsWith(modifiedSuffix)) {
		mMainWindow->setWindowTitle(mMainWindow->windowTitle() + modifiedSuffix);
	}
}

bool ProjectManagerWrapper::openNewWithDiagram()
{
	if (!openEmptyWithSuggestToSaveChanges()) {
		return false;
	}

	suggestToCreateDiagram(false);
	return true;
}

void ProjectManagerWrapper::suggestToCreateDiagram(bool isClosable)
{
	const Id theOnlyDiagram = mMainWindow->editorManager().theOnlyDiagram();
	if (theOnlyDiagram != Id()) {
		const Id editor = mMainWindow->editorManager().editors()[0];
		mMainWindow->createDiagram(mMainWindow->editorManager().diagramNodeNameString(editor, theOnlyDiagram));
	} else {
		SuggestToCreateDiagramDialog suggestDialog(mMainWindow->editorManager(), mMainWindow, isClosable);
		connect(&suggestDialog, &SuggestToCreateDiagramDialog::diagramSelected
				, mMainWindow, &MainWindow::createDiagram);
		suggestDialog.exec();
	}
}

void ProjectManagerWrapper::close()
{
	if (mMainWindow->propertyEditor()->model()) {
		static_cast<PropertyEditorModel *>(mMainWindow->propertyEditor()->model())->clearModelIndexes();
	}

	mMainWindow->graphicalModelExplorer()->setModel(nullptr);
	mMainWindow->logicalModelExplorer()->setModel(nullptr);

	if (mMainWindow->getCurrentTab()) {
		static_cast<qReal::gui::editor::EditorViewScene *>(mMainWindow->getCurrentTab()->scene())->clearScene();
	}

	mMainWindow->closeAllTabs();
	mMainWindow->setWindowTitle(mMainWindow->toolManager().customizer()->windowTitle());

	ProjectManager::close();
}

void ProjectManagerWrapper::save()
{
	mMainWindow->editorManagerProxy().saveMetamodel("");
	ProjectManager::save();
}

bool ProjectManagerWrapper::saveOrSuggestToSaveAs()
{
	if (mTextManager->saveText(false)) {
		return true;
	}

	if (mSaveFilePath == mAutosaver.tempFilePath()
			|| mSaveFilePath == mMainWindow->editorManagerProxy().saveMetamodelFilePath()) {
		return suggestToSaveAs();
	}

	return ProjectManager::saveOrSuggestToSaveAs();
}

void ProjectManagerWrapper::setUnsavedIndicator(bool isUnsaved)
{
	ProjectManager::setUnsavedIndicator(isUnsaved);
	refreshWindowTitleAccordingToSaveFile();
}

bool ProjectManagerWrapper::suggestToSaveAs()
{
	if (mTextManager->saveText(true)) {
		return true;
	}

	if (mMainWindow->editorManagerProxy().isInterpretationMode()) {
		const QString newMetamodelFileName = saveFileName(tr("Select file to save current metamodel to"));
		if (newMetamodelFileName.isEmpty()) {
			return false;
		}
		mMainWindow->editorManagerProxy().saveMetamodel(newMetamodelFileName);
	}

	return ProjectManager::suggestToSaveAs();
}

QString ProjectManagerWrapper::openFileName(const QString &dialogWindowTitle) const
{
	const QString pathToExamples = mMainWindow->toolManager().customizer()->examplesDirectory();
	const QString defaultDirectory = pathToExamples.isEmpty()
			? QFileInfo(mSaveFilePath).absoluteDir().absolutePath()
			: pathToExamples;
	QString filter = tr("QReal Save File (*.qrs)") + ";;";
	const QString extensions = textFileFilters();

	filter += (extensions.isEmpty() ? "" : extensions + ";;") + tr("All files (*.*)");

	QString fileName = QRealFileDialog::getOpenFileName("OpenQRSProject", mMainWindow, dialogWindowTitle
			, defaultDirectory, filter);

	if (!fileName.isEmpty() && !QFile::exists(fileName)) {
		fileNotFoundMessage(fileName);
		fileName = openFileName(dialogWindowTitle);
	}

	return fileName;
}

QString ProjectManagerWrapper::saveFileName(const QString &dialogWindowTitle) const
{
	const QString oldFileName = QFileInfo(saveFilePath()).fileName();
	const QString defaultSaveFilePath = mAutosaver.isTempFile(mSaveFilePath)
			? PlatformInfo::invariantSettingsPath("pathToDefaultSaves")
			: QFileInfo(mSaveFilePath).absoluteDir().absolutePath();
	QString fileName = QRealFileDialog::getSaveFileName("SaveQRSProject", mMainWindow, dialogWindowTitle
			, defaultSaveFilePath, tr("QReal Save File(*.qrs)"), oldFileName);

	if (!fileName.isEmpty() && !fileName.endsWith(".qrs", Qt::CaseInsensitive)) {
		fileName += ".qrs";
	}

	return fileName;
}
