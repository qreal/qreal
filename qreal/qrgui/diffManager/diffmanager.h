#pragma once

#include "../editorManager/editorManager.h"
#include "../models/models.h"
#include "../mainwindow/mainwindow.h"

namespace qReal
{
namespace diffManager
{

class DiffManager
{
public:
	DiffManager(MainWindow* mainWindow);
	bool showDiff(QString const &workingDir);
	QStringList newErrors();

private:
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
