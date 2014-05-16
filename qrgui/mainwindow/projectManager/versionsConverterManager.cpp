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
	for (QString const &editor : editorsToCheck) {
		Version const currentVersion = mMainWindow.editorManager().version(Id(editor));

		// This is a temporal solution that will disable too old saves in editors.
		if (currentVersion.isValid() != savedVersions[Id(editor)].isValid()) {
		/// @todo: Compare versions intellectually: plugins must provide compability and convertion info.
		/// if (savedVersions[editor] < currentVersion) {
			/// @todo: convertion process must be started here.
			displayCannotConvertError(savedVersions[Id(editor)]);
			return false;
		}
	}

	return true;
}

void VersionsConverterManager::displayCannotConvertError(Version const &oldVersion)
{
	bool const showVersionDetails = oldVersion.isValid();
	QString const reason = showVersionDetails
			? QObject::tr("This project was created by version %1 of the editor.").arg(oldVersion.toString())
			: QObject::tr("This project was created by too old version of the editor.");

	QString const errorMessage = reason + QObject::tr(" To our great regret this project "\
			"can`t be automaticly converted up to the current enviroment version and thus can`t be opened.");

	QMessageBox::information(&mMainWindow, QObject::tr("Can`t open project file"), errorMessage);
}
