#pragma once

#include "../editorManager/editorManager.h"
#include "../models/models.h"
#include "../mainwindow/mainwindow.h"

namespace qReal
{
namespace diffManager
{

/*! \class DiffManager
* \brief Provides visual diff operation.
*
* DiffManager class provides ability to see graphical difference
* between model in repository and working copy model. To use this
* class abilities, working copy model must be under version control.
* Use showDiff() method to see difference and newErrors() method
* to get list of errors occured.
*
*/
class DiffManager
{
public:
	/*!
	* Creates new instance of DiffManager class binded to
	* specified working copy directory.
	*/
	DiffManager(MainWindow* mainWindow);
	/*!
	* This is a main method of DiffManager class. It checks
	* out head revision from repo to specified in prefernces
	* path, loads model in this revision and shows graphical
	* difference between repo and working copy models. All
	* errors occured during this process can be requested
	* using newErrors() method.
	* \return Returns boolean value showing if an error occured or no.
	* \sa newErros()
	*/
	bool showDiff(QString const &workingDir);
	/*!
	* This method provides list of the errors occured during
	* the diff computation process.
	* \attention Calling of this method will clear the current
	*            list of errors.
	*/
	QStringList newErrors();

private:
	/*!
	* Main window instance for getting the editor manager
	* and diff window parentness.
	*/
	MainWindow *mMainWindow;
	models::Models *mWorkingCopyModel;
	models::Models *mRepoModel;
	QStringList mErrors;

	QString createRepoModel(QString const &workingCopy);
	bool clearDir(QString const &path);
	bool removeDir(QString const &path);
};

}

}
