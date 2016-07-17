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

#pragma once

#include <QtCore/QFileInfo>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>
#include <qrgui/models/models.h>
#include "qrgui/systemFacade/components/autosaver.h"

namespace qReal {

class MainWindow;

/// ProjectManagementInterface implementation
class QRGUI_SYSTEM_FACADE_EXPORT ProjectManager : public ProjectManagementInterface
{
	Q_OBJECT

public:
	explicit ProjectManager(models::Models &models);

public slots:
	bool openExisting(const QString &fileName) override;
	bool suggestToOpenExisting() override;
	bool openNewWithDiagram() override;
	bool suggestToImport() override;

	void close() override;

	void save() override;
	bool saveAs(const QString &fileName) override;
	bool suggestToSaveAs() override;
	bool saveOrSuggestToSaveAs() override;

	void setUnsavedIndicator(bool isUnsaved) override;

	/// Saves current project into given file without refreshing application state after it
	void saveTo(const QString &fileName);

public:
	bool openEmptyWithSuggestToSaveChanges() override;
	bool open(const QString &fileName = QString()) override;
	bool suggestToSaveChangesOrCancel() override;
	void reinitAutosaver();
	bool somethingOpened() const override;
	QString saveFilePath() const override;
	void setSaveFilePath(const QString &filePath = QString());

	/// Prompts user to restore last session if it was incorrectly terminated
	/// and returns yes if he agrees. Otherwise returns false
	bool restoreIncorrectlyTerminated();

	/// May be overrided to ask user such questions as should we open autosave or not, etc.
	/// The answer will be given in yes/no form, true or false will be returned correspondingly.
	virtual bool askQuestion(const QString &title, const QString &question) const;

	/// May be overrided to prompt user a path to file to open.
	virtual QString openFileName(const QString &promptPhrase) const;

protected:
	bool openProject(const QString &fileName);
	bool import(const QString &fileName);
	bool saveFileExists(const QString &fileName) const;

	bool pluginsEnough() const;
	QString missingPluginNames() const;
	void checkNeededPluginsRecursive(const details::ModelsAssistInterface &api, const Id &id
			, QStringList &result) const;

	virtual bool checkVersions();
	bool checkForUnknownElements();

	virtual void refreshApplicationStateAfterSave();
	virtual void refreshApplicationStateAfterOpen();

	/// Can be overrided to prompt user a path to file where to save project.
	virtual QString saveFileName(const QString &promptPhrase) const;

	/// Can be overrided to report user a some information and errors occured during some operation.
	virtual void showMessage(const QString &title, const QString &message) const;

	void fileNotFoundMessage(const QString &fileName) const;

	models::Models &mModels;
	Autosaver mAutosaver;
	bool mUnsavedIndicator;
	QString mSaveFilePath;
	bool mSomeProjectOpened;
};

}
