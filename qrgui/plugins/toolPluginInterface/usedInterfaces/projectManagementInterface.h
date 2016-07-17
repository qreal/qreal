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

#include <QtCore/QObject>

#include "qrgui/systemFacade/components/autosaver.h"
#include "qrgui/plugins/toolPluginInterface/toolPluginInterfaceDeclSpec.h"

namespace qReal {

/// Interface that provides all of the work load project from a file, stored in the project file, and so on
class QRGUI_TOOL_PLUGIN_INTERFACE_EXPORT ProjectManagementInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~ProjectManagementInterface() {}

public slots:
	/// Tries to open an existing file, prompts you to save changes, if any. Checks for the presence of a file and the
	/// sufficiency of the plugins. In the event of any failure, leaving the project as is and returns false, o/w true.
	virtual bool openExisting(const QString &fileName) = 0;

	/// Similarly @see openExisting(), but offers the user select a file (by a dialog)
	virtual bool suggestToOpenExisting() = 0;

	/// Creating a new project
	virtual bool openNewWithDiagram() = 0;

	/// Offers the user select a file (by a dialog) and import all diagrams from it into the current project.
	virtual bool suggestToImport() = 0;

	/// Closes current project without suggestion to save it
	virtual void close() = 0;

	/// Save the project in the user file, if it specified. Otherwise save to autosave file
	virtual void save() = 0;

	/// Save the project in the file with the name fileName, if it not empty (fileName). Otherwise return false only.
	virtual bool saveAs(const QString &fileName) = 0;

	/// Similarly @see saveAs(const QString &fileName), but offers the user specified file location (by a dialog)
	virtual bool suggestToSaveAs() = 0;

	/// Similarly @see save(), if specified project-file, similarly @see suggestToSaveAs() o/w
	virtual bool saveOrSuggestToSaveAs() = 0;

public:
	/// Returns true if some .qrs project is currently opened in system. Otherwise returns false.
	virtual bool somethingOpened() const = 0;

	/// Create an empty project
	virtual bool openEmptyWithSuggestToSaveChanges() = 0;

	/// Try to open save file with name fileName, show messages is file non exist or plugins are missing and
	/// return false, or return true otherwise. fileName == "" (default value) will be create an empty project.
	virtual bool open(const QString &fileName = QString()) = 0;

	virtual bool suggestToSaveChangesOrCancel() = 0;

	virtual void setUnsavedIndicator(bool isUnsaved) = 0;

	/// Returns path to save file. If temp file it will return empty string.
	virtual QString saveFilePath() const = 0;

signals:
	/// Emitted each time when project manager is going to open new project
	/// @param fileName Project location
	void beforeOpen(const QString &fileName);

	/// Emitted each time when project manager has opened new project
	/// @param fileName Opened project location
	void afterOpen(const QString &fileName);

	/// Emitted each time when project manager has closed current project
	void closed();
};

}
