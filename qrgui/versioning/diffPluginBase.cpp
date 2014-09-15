#include "diffPluginBase.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;

qReal::ErrorReporterInterface *DiffPluginBase::errorReporter() const
{
	return mErrorReporter;
}

qReal::ProjectManagementInterface *DiffPluginBase::projectManager() const
{
	return mProjectManager;
}

void DiffPluginBase::setHandler(DiffPluginInterface *handler)
{
	mHandler = handler;
}
