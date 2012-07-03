#pragma once

#include <QtCore/QTranslator>

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

#include "compilerPage.h"

namespace metaEditor {

/// Main plugin class for metaeditor support code. Provides generation of editor
/// by metamodel specified in .xml file or in QReal save file, compilation of
/// generated editor and loading it to QReal, and parsing of existing .xml metamodel.
class MetaEditorSupportPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	MetaEditorSupportPlugin();
	virtual ~MetaEditorSupportPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();
	virtual QPair<QString, PreferencesPage *> preferencesPage();

private slots:
	/// Generates .xml file with metamodel for later compilation with qrxc tool
	void generateEditorForQrxc();

	/// Generates .cpp/.h code ready to be compiled into editor plugin
	void generateEditorWithQrmc();

	/// Parses existing .xml with metamodel and shows it in QReal as logical model
	/// in metaeditor
	void parseEditorXml();

private:
	/// Compiles and loads to QReal new editor plugin
	void loadNewEditor(
			QString const &directoryName  ///< Directory where editor was generated (containing .pro file of that editor)
			, QPair<QString, QString> const &metamodelNames  ///< Metamodel name as set in metamodel
			, QString const &commandFirst  ///< qmake command
			, QString const &commandSecond  ///< make command
			, QString const &extension  ///< Extension of the resulting binary file with editor plugin (.dll/.so)
			, QString const &prefix  ///< Optional OS-dependent prefix for resulting binary file (lib for linux, for example)
			);

	/// Action that starts generation of .xml file with metamodel for qrxc
	QAction mGenerateEditorForQrxcAction;

	/// Action that starts generation of C++ sources with qrmc
	QAction mGenerateEditorWithQrmcAction;

	/// Action that starts parsing of existing .xml metamodel
	QAction mParseEditorXmlAction;

	/// Thanslator object for this plugin, provides localisation
	QTranslator mAppTranslator;

	/// Interface of MainWindow, used, for example, to reinit models when finished parsing
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership

	/// API of logical part of a repository, mutable to allow parser build model
	qrRepo::LogicalRepoApi *mLogicalRepoApi;  // Does not have ownership

	/// Control API of a repository, used for requests such as current save file name
	qrRepo::RepoControlInterface *mRepoControlApi;  // Does not have ownership

	/// Page with plugin settings. Created here, but then ownership is passed to
	/// a caller of preferencesPage().
	PreferencesCompilerPage *mCompilerSettingsPage;  // Does not have ownership
};

}
