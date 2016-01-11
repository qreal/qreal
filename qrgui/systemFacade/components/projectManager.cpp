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

#include "projectManager.h"

#include <qrkernel/logging.h>
#include <qrutils/outFile.h>
#include <qrutils/stringUtils.h>
#include <qrutils/qRealFileDialog.h>
#include <qrgui/models/models.h>
#include <qrrepo/exceptions/qrrepoException.h>

using namespace qReal;

ProjectManager::ProjectManager(models::Models &models)
	: mModels(models)
	, mAutosaver(*this)
	, mUnsavedIndicator(false)
	, mSomeProjectOpened(false)
{
	setSaveFilePath();
}

void ProjectManager::setSaveFilePath(const QString &filePath /* = "" */)
{
	mSaveFilePath = filePath.isEmpty()
			? mAutosaver.tempFilePath()
			: mAutosaver.isTempFile(filePath)
					? filePath
					: mAutosaver.originalFile(filePath);
}

QString ProjectManager::saveFilePath() const
{
	return mAutosaver.isTempFile(mSaveFilePath) ? QString() : mSaveFilePath;
}

bool ProjectManager::somethingOpened() const
{
	return mSomeProjectOpened;
}

void ProjectManager::reinitAutosaver()
{
	mAutosaver.reinit();
}

bool ProjectManager::openExisting(const QString &fileName)
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

	const QString fileName = openFileName(tr("Open existing project"));
	if (fileName.isEmpty()) {
		return false;
	}

	return open(fileName);
}

bool ProjectManager::suggestToSaveChangesOrCancel()
{
	return true;
}

bool ProjectManager::open(const QString &fileName)
{
	const QString dequotedFileName = utils::StringUtils::dequote(fileName);
	const QFileInfo fileInfo(dequotedFileName);

	if (fileInfo.suffix() == "qrs" || fileInfo.completeBaseName().isEmpty()) {
		if (!dequotedFileName.isEmpty() && !saveFileExists(dequotedFileName)) {
			return false;
		}

		return openProject(dequotedFileName);
	}

	return false;
}

bool ProjectManager::openProject(const QString &fileName)
{
	// 1. If Autosaver have time to save the state repository at the time of
	// testing the sufficiency of plugins to open the project, the autosave
	// file may become incompatible with the application. This will lead to
	// a fail on the next start.
	// 2. autosavePauser was first starts a timer of Autosaver
	const Autosaver::Pauser autosavePauser(mAutosaver);
	Q_UNUSED(autosavePauser)

	if (!fileName.isEmpty() && !saveFileExists(fileName)) {
		return false;
	}

	emit beforeOpen(fileName);
	// There is no way to verify sufficiency plugins without initializing repository
	// that is stored in the save file. Initializing is impossible without closing current project.
	const bool someProjectWasOpened = mSomeProjectOpened;
	if (mSomeProjectOpened) {
		close();
	}

	if (mAutosaver.checkAutoSavedVersion(fileName)) {
		setUnsavedIndicator(true);
		mSomeProjectOpened = true;
		return true;
	}

	try {
		mModels.repoControlApi().open(fileName);
	} catch (qrRepo::QrRepoException) {
		return false;
	}

	mModels.reinit();

	if (!pluginsEnough() || !checkVersions() || !checkForUnknownElements()) {
		// restoring the session
		if (someProjectWasOpened) {
			mSomeProjectOpened = open(mSaveFilePath);
		}

		return false;
	}

	setSaveFilePath(fileName);
	refreshApplicationStateAfterOpen();

	mSomeProjectOpened = true;
	QLOG_INFO() << "Opened project" << fileName;
	QLOG_DEBUG() << "Sending after open signal...";

	emit afterOpen(fileName);

	return true;
}

bool ProjectManager::suggestToImport()
{
	return import(openFileName(tr("Select file with a save to import")));
}

bool ProjectManager::import(const QString &fileName)
{
	if (fileName.isEmpty()) {
		return false;
	}

	const QString currentSaveFilePath = saveFilePath();
	if (!open(fileName)) {
		return open(currentSaveFilePath);
	}

	// In the hope that while the user selects a file nobody substitute for the current project with project, which
	// has diagrams for which there are no plugins
	mModels.repoControlApi().importFromDisk(currentSaveFilePath);
	mModels.reinit();
	return true;
}

bool ProjectManager::saveFileExists(const QString &fileName) const
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
		showMessage(tr("There are missing plugins")
				, tr("These plugins are not present, but needed to load the save:\n") + missingPluginNames());
		return false;
	}

	return true;
}

QString ProjectManager::missingPluginNames() const
{
	QStringList missingPlugins;
	checkNeededPluginsRecursive(mModels.logicalModelAssistApi(), Id::rootId(), missingPlugins);
	checkNeededPluginsRecursive(mModels.graphicalModelAssistApi(), Id::rootId(), missingPlugins);
	return missingPlugins.join("\n");
}

void ProjectManager::checkNeededPluginsRecursive(const details::ModelsAssistInterface &api
		, const Id &id, QStringList &result) const
{
	const EditorManagerInterface &editorManager = mModels.logicalModelAssistApi().editorManagerInterface();
	const IdList loadedEditors = editorManager.editors();
	const Id currentEditor = Id(id.editor());
	if (id != Id::rootId() && !loadedEditors.contains(currentEditor)) {
		const QString missingEditor = id.editor();
		if (!result.contains(missingEditor)) {
			result.append(missingEditor);
		}
	}

	for (const Id &child : api.children(id)) {
		checkNeededPluginsRecursive(api, child, result);
	}
}

bool ProjectManager::checkVersions()
{
	/// @todo: Versions validation must occure in console mode too
	return true;
}

bool ProjectManager::checkForUnknownElements()
{
	const IdList allElements = mModels.logicalModelAssistApi().children(Id::rootId());
	for (const Id &element : allElements) {
		const bool isElementKnown = mModels.logicalModelAssistApi().editorManagerInterface().hasElement(element.type());
		if (!isElementKnown) {
			const QString errorMessage = tr("This project contains unknown element %1 and thus can`t be opened. "\
					"Probably it was created by old or incorrectly working version of QReal.").arg(element.toString());
			showMessage(tr("Can`t open project file"), errorMessage);
			return false;
		}
	}

	return true;
}

void ProjectManager::refreshApplicationStateAfterSave()
{
}

void ProjectManager::refreshApplicationStateAfterOpen()
{
}

bool ProjectManager::openNewWithDiagram()
{
	return openEmptyWithSuggestToSaveChanges();
}

bool ProjectManager::openEmptyWithSuggestToSaveChanges()
{
	if (!suggestToSaveChangesOrCancel()) {
		return false;
	}

	return open();
}

void ProjectManager::close()
{
	mAutosaver.removeAutoSave();
	mAutosaver.removeTemp();
	mSomeProjectOpened = false;

	QLOG_INFO() << "Project" << mSaveFilePath << "closed";
	emit closed();
}

void ProjectManager::saveTo(const QString &fileName)
{
	QLOG_INFO() << "Saving project into" << fileName;
	mModels.repoControlApi().saveTo(fileName);
}

void ProjectManager::save()
{
	// Do not change the method to saveAll - in the current implementation, an empty project in the repository is
	// created to initialize the file name with an empty string, which allows the internal state of the file
	// name = "" Attempt to save the project in this case result in
	saveTo(mSaveFilePath);
	mAutosaver.removeAutoSave();
	refreshApplicationStateAfterSave();
}

bool ProjectManager::restoreIncorrectlyTerminated()
{
	return mAutosaver.checkTempFile();
}

bool ProjectManager::askQuestion(const QString &title, const QString &question) const
{
	Q_UNUSED(title)
	Q_UNUSED(question)
	return false;
}

bool ProjectManager::saveOrSuggestToSaveAs()
{
	save();
	return true;
}

bool ProjectManager::suggestToSaveAs()
{
	return saveAs(saveFileName(tr("Select file to save current model to")));
}

bool ProjectManager::saveAs(const QString &fileName)
{
	const QString workingFileName = fileName;
	if (workingFileName.isEmpty()) {
		return false;
	}

	mAutosaver.removeAutoSave();
	mModels.repoControlApi().saveTo(workingFileName);
	setSaveFilePath(workingFileName);
	refreshApplicationStateAfterSave();
	return true;
}

QString ProjectManager::openFileName(const QString &promptPhrase) const
{
	Q_UNUSED(promptPhrase)
	return QString();
}

void ProjectManager::showMessage(const QString &title, const QString &message) const
{
	Q_UNUSED(title);
	qDebug() << qPrintable(message);
}

QString ProjectManager::saveFileName(const QString &promptPhrase) const
{
	Q_UNUSED(promptPhrase)
	return QString();
}

void ProjectManager::setUnsavedIndicator(bool isUnsaved)
{
	mUnsavedIndicator = isUnsaved;
}

void ProjectManager::fileNotFoundMessage(const QString &fileName) const
{
	showMessage(tr("File not found"), tr("File %1 not found. Try again").arg(fileName));
}
