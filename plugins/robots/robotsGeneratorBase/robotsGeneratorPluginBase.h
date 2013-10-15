#pragma once

#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>

#include <qrgui/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/toolPluginInterface/pluginConfigurator.h>
#include <qrrepo/repoApi.h>
#include "robotsGeneratorDeclSpec.h"
#include "masterGeneratorBase.h"

namespace qReal {
namespace robots {
namespace generators {

class ROBOTS_GENERATOR_EXPORT RobotsGeneratorPluginBase : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	RobotsGeneratorPluginBase();

	virtual void init(qReal::PluginConfigurator const &configurator);

protected slots:
	/// Calls code generator. Returns true if operation was successful.
	virtual bool generateCode();

protected:
	/// Override must return a link to concrete master generator instance for
	/// developped plugin. Caller takes ownership so override may forget about it.
	virtual MasterGeneratorBase *masterGenerator() = 0;

	/// Interface of MainWindow
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership

	/// Interface of project manager (allows to perform open/save activities)
	qReal::ProjectManagementInterface *mProjectManager; // Does not have ownership

	/// Control interface of the repository
	qrRepo::RepoApi const *mRepo;  // Does not have ownership

	/// Translator object for this plugin
	QTranslator mAppTranslator;
};

}
}
}
