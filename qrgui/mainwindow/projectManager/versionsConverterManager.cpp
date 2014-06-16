#include "versionsConverterManager.h"

#include <QtWidgets/QMessageBox>

#include "mainwindow/mainWindow.h"

using namespace qReal;

VersionsConverterManager::VersionsConverterManager(MainWindow &mainWindow)
	: mMainWindow(mainWindow)
{
}

bool VersionsConverterManager::validateCurrentProject()
{
	QSet<QString> editorsToCheck;
	IdList const allElements = mMainWindow.models()->logicalModelAssistApi().children(Id::rootId());
	for (Id const &element : allElements) {
		editorsToCheck << element.editor();
	}

	QMap<Id, Version> const savedVersions = mMainWindow.models()->logicalModelAssistApi().editorVersions();
	QMultiMap<QString, ProjectConverter> const converters = mMainWindow.toolManager().projectConverters();

	for (QString const &editor : editorsToCheck) {
		Version const currentVersion = mMainWindow.editorManager().version(Id(editor));
		Version const savedVersion = savedVersions[Id(editor)];

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

bool VersionsConverterManager::convertProject(Version const &enviromentVersion
		, Version const &saveVersion
		, QList<ProjectConverter> const &converters)
{
	// Stage I: Sorting converters by versions
	QList<ProjectConverter> sortedConverters = converters;
	qSort(sortedConverters.begin(), sortedConverters.end()
		, [=](ProjectConverter const &converter1, ProjectConverter const &converter2)
	{
		return converter1.fromVersion() < converter2.fromVersion();
	});

	// Stage II: Checking that versions are not overlapped
	for (int index = 0; index < sortedConverters.count() - 1; ++index) {
		if (sortedConverters[index].toVersion() > sortedConverters[index + 1].fromVersion()) {
			qDebug() << "Converter versions are overlapped!";
			return false;
		}
	}

	bool converterApplied = false;

	// Stage III: Sequentially applying converters
	for (ProjectConverter &converter : sortedConverters) {
		if (converter.fromVersion() >= saveVersion && converter.toVersion() <= enviromentVersion) {
			ProjectConverter::ConvertionResult const result = converter.convert(
					mMainWindow.models()->graphicalModelAssistApi()
					, mMainWindow.models()->logicalModelAssistApi());
			switch (result) {
			case ProjectConverter::Success:
				converterApplied = true;
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
		mMainWindow.errorReporter()->addInformation(QObject::tr("Project was automaticly converted to version %1")
				.arg(enviromentVersion.toString()));
	}

	return true;
}

void VersionsConverterManager::displayCannotConvertError()
{
	QString const errorMessage = QObject::tr("The attempt to automaticly convert this project "\
			"to the current enviroment version failed and thus save file can`t be opened. ");

	showError(errorMessage);
}

void VersionsConverterManager::displayTooOldSaveError(Version const &saveVersion)
{
	bool const showVersionDetails = saveVersion.isValid();
	QString const reason = showVersionDetails
			? QObject::tr("This project was created by version %1 of the editor.").arg(saveVersion.toString())
			: QObject::tr("This project was created by too old version of the editor.");

	QString const errorMessage = reason + QObject::tr(" It is now considered outdated and cannot be opened.");

	showError(errorMessage);
}

void VersionsConverterManager::displayTooOldEnviromentError(Version const &saveVersion)
{
	QString const errorMessage = QObject::tr("The save you are trying to open is made by version %1 of editor, "\
			"whitch is newer than currently installed enviroment. "\
			"Update your version before opening this save.").arg(saveVersion.toString());

	showError(errorMessage);
}

void VersionsConverterManager::showError(QString const &errorMessage)
{
	QMessageBox::information(&mMainWindow, QObject::tr("Can`t open project file"), errorMessage);
}
