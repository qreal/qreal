#pragma once

#include <QtCore/QTranslator>

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

namespace hascol {

/// Main plugin class for Hascol tools. Provides generator and parser for Hascol
/// sources. Parser requires installed CoolKit (http://oops.math.spbu.ru/projects/coolkit)
/// and that COOL_ROOT environment variable is set to root of CoolKit installation.
class HascolSupportPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	HascolSupportPlugin();
	virtual ~HascolSupportPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

private slots:
	/// Slot that calls generator
	void generateHascolSourceCode();

	/// Slot that asks user about files that need to be parsed and calls hascol parser
	void parseHascolSources();

private:
	/// Action that launches code generator
	QAction mGenerateCodeAction;

	/// Action that launches hascol parser
	QAction mParseSourcesAction;

	/// Thanslator object for this plugin, provides localisation
	QTranslator mAppTranslator;

	/// Interface of MainWindow, used, for example, to reinit models when finished parsing
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership

	/// API of logical part of a repository, mutable to allow parser build model
	qrRepo::LogicalRepoApi *mLogicalRepoApi;  // Does not have ownership
};

}
