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
#include <QtCore/QTimer>

#include "qrgui/systemFacade/systemFacadeDeclSpec.h"

namespace qReal {

class ProjectManager;

/// @brief class provides automatic saving of the project at equal time intervals.
/// All options for working are retrieved from the settings manager.
/// Autosaver provides an interface that allows you to make it reload parameters
class QRGUI_SYSTEM_FACADE_EXPORT Autosaver : public QObject
{
	Q_OBJECT

public:
	class Pauser
	{
	public:
		Pauser(Autosaver &autosaver);
		~Pauser();

	private:
		Autosaver &mAutosaver;
	};

	explicit Autosaver(ProjectManager &projectManager);
	void reinit();

	QString tempFilePath() const;
	bool isAutosave(const QString &fileName) const;
	bool isTempFile(const QString &fileName) const;
	QString originalFile(const QString &fileName) const;

	bool openTemp();
	bool openAutosave(const QString &fileName);

	bool removeTemp();
	bool removeAutoSave();
	bool removeAutoSave(const QString &fileName);

	/// Performs search in project`s folder and tries to find autosaved version
	/// of specified project. If such version was not found returns false.
	/// Otherwise prompts user to recover the newest version.
	/// Returns true if user agrees and autosave was opened successfully,
	/// otherwise returns false.
	bool checkAutoSavedVersion(const QString &originalProjectPath);

	/// Similar to @see checkAutoSavedVersion, but searches for temp save
	bool checkTempFile();

private slots:
	void saveAutoSave();
	void saveTemp();

private:
	QString autosaveFilePath() const;
	QString autosaveFilePath(const QString &currentFilePath) const;
	void resume();
	void suspend();

	uint interval() const;
	QString openAutosavePrompt() const;
	QString openTempFilePrompt() const;
	bool removeFile(const QString &fileName);

	// if option AutosaveInterval in not specified
	static const uint defaultInterval = 600;

	ProjectManager &mProjectManager;
	QTimer mTimer;
};

}
