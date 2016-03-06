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

#include "versionsConverterManager.h"

#include <QtWidgets/QMessageBox>

#include <qrgui/models/models.h>
#include <qrgui/plugins/pluginManager/toolPluginManager.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "mainWindow/mainWindow.h"

using namespace qReal;

VersionsConverterManager::VersionsConverterManager(MainWindow &mainWindow)
	: mMainWindow(mainWindow)
{
}

bool VersionsConverterManager::validateCurrentProject()
{
	QSet<QString> editorsToCheck;
	const IdList allElements = mMainWindow.models().logicalModelAssistApi().children(Id::rootId());
	for (const Id &element : allElements) {
		editorsToCheck << element.editor();
	}

	const QMap<Id, Version> savedVersions = mMainWindow.models().logicalModelAssistApi().editorVersions();
	const QMultiMap<QString, ProjectConverter> converters = mMainWindow.toolManager().projectConverters();

	for (const QString &editor : editorsToCheck) {
		const Version currentVersion = mMainWindow.editorManager().version(Id(editor));
		const Version savedVersion = savedVersions[Id(editor)];

		if (currentVersion == savedVersion) {
			continue;
		}

		if (currentVersion < savedVersion) {
			displayTooOldEnviromentError(savedVersion);
			return false;
		}

		if (!convertProject(currentVersion, savedVersion, converters.values(editor))) {
			return false;
		}
	}

	return true;
}

bool VersionsConverterManager::convertProject(const Version &enviromentVersion
		, const Version &saveVersion
		, QList<ProjectConverter> const &converters)
{
	// Stage I: Sorting converters by versions
	QList<ProjectConverter> sortedConverters = converters;
	qSort(sortedConverters.begin(), sortedConverters.end()
		, [=](const ProjectConverter &converter1, const ProjectConverter &converter2)
	{
		return converter1.fromVersion() < converter2.fromVersion();
	});

	// Stage II: Checking that versions are not overlapped
	for (int index = 0; index < sortedConverters.count() - 1; ++index) {
		if (sortedConverters[index].toVersion() > sortedConverters[index + 1].fromVersion()) {
			qWarning() << "Converter versions are overlapped!";
			return false;
		}
	}

	bool converterApplied = false;

	// Stage III: Sequentially applying converters
	for (ProjectConverter &converter : sortedConverters) {
		if (converter.fromVersion() >= saveVersion && converter.toVersion() <= enviromentVersion) {
			const ProjectConverter::ConvertionResult result = converter.convert(
					mMainWindow.models().graphicalModelAssistApi()
					, mMainWindow.models().logicalModelAssistApi());
			switch (result) {
			case ProjectConverter::Success:
				converterApplied = true;
				break;
			case ProjectConverter::NoModificationsMade:
				break;
			case ProjectConverter::SaveInvalid:
				displayCannotConvertError();
				return false;
			case ProjectConverter::VersionTooOld:
				displayTooOldSaveError(saveVersion);
				return false;
			}
		}
	}

	// Stage IV: Notifying user
	if (converterApplied) {
		mMainWindow.errorReporter()->addInformation(QObject::tr("Project was automaticly converted to version %1."\
				" Please check its contents.").arg(enviromentVersion.toString()));
		mMainWindow.models().mutableLogicalRepoApi().setMetaInformation(
				converters.first().editor() + "Version", enviromentVersion.toString());
	}

	return true;
}

void VersionsConverterManager::displayCannotConvertError()
{
	const QString errorMessage = QObject::tr("The attempt to automaticly convert this project "\
			"to the current enviroment version failed and thus save file can`t be opened. ");

	showError(errorMessage);
}

void VersionsConverterManager::displayTooOldSaveError(const Version &saveVersion)
{
	const bool showVersionDetails = saveVersion.isValid();
	const QString reason = showVersionDetails
			? QObject::tr("This project was created by version %1 of the editor.").arg(saveVersion.toString())
			: QObject::tr("This project was created by too old version of the editor.");

	const QString errorMessage = reason + QObject::tr(" It is now considered outdated and cannot be opened.");

	showError(errorMessage);
}

void VersionsConverterManager::displayTooOldEnviromentError(const Version &saveVersion)
{
	const QString errorMessage = QObject::tr("The save you are trying to open is made by version %1 of editor, "\
			"whitch is newer than currently installed enviroment. "\
			"Update your version before opening this save.").arg(saveVersion.toString());

	showError(errorMessage);
}

void VersionsConverterManager::showError(const QString &errorMessage)
{
	QMessageBox::information(&mMainWindow, QObject::tr("Can`t open project file"), errorMessage);
}
