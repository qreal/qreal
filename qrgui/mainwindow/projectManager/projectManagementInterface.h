#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>

#include "autosaver.h"

namespace qReal
{

/// Interfase that provides all of the work load project from a file, stored in the project file, and so on
class ProjectManagementInterface : public QObject
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
	/// Creating a new diagramm
	virtual void suggestToCreateDiagram(bool isNonClosable = false) = 0;
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
	/// Create an empty project
	virtual bool openEmptyWithSuggestToSaveChanges() = 0;
	/// Try to open save file with name fileName, show messages is file non exist or plugins are missing and
	/// return false, or return true otherwise. fileName == "" (default value) will be create an empty project.
	virtual bool open(QString const &fileName = "") = 0;
	virtual bool suggestToSaveChangesOrCancel() = 0;
	virtual void setUnsavedIndicator(bool isUnsaved) = 0;
	virtual void reinitAutosaver() = 0;
};

}
