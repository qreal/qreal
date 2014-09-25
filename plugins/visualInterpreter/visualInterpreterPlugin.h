#pragma once

#include "../../qrkernel/ids.h"
#include "../../qrutils/metamodelGeneratorSupport.h"
#include "../../qrutils/watchListWindow.h"
#include "../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../qrgui/toolPluginInterface/pluginConfigurator.h"
#include "../../qrgui/mainwindow/errorReporter.h"

#include "visualInterpreterPreferencesPage.h"
#include "visualInterpreterUnit.h"

namespace qReal {
namespace visualInterpreter {

/// Visual interpreter plugin for any editor. Allows to create semantics for any
/// visual language and interpret diagrams with it.
class VisualInterpreterPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.visualInterpreter.VisualInterpreterPlugin")

public:
	VisualInterpreterPlugin();
	virtual ~VisualInterpreterPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

	virtual QPair<QString, PreferencesPage *> preferencesPage();

private slots:
	void generateMetamodels() const;
	
	/// Generate, compile and load semantics editor for specified metamodel
	void generateSemanticsMetamodel(QString const &editorMetamodelFilePath, QString const &qrealSourceFilesPath) const;

	/// Generate, compile and load editor for specified metamodel with new 'id' attribute
	void generateEditorMetamodel(QString const &editorMetamodelFilePath, QString const &qrealSourceFilesPath) const;

	/// Load semantics model from current opened diagram
	void loadSemantics();

	/// Make one step according to semantics (find match, delete, create and replace elements)
	void interpret();

	/// Stops interpretation
	void stopInterpretation();

	/// Show watch list with all declared variables and its values. List updates
	/// dynamically
	void showWatchList();

private:
	/// Insert some semantics enums into semantics metamodel
	void insertSemanticsEnums(QDomDocument metamodel, QString const &name, QStringList const &values) const;

	/// Add special semantics state property to all existing elements
	void insertSematicsStateProperty(QDomDocument metamodel) const;

	/// Add to specific elements semantics state property (different approach for nodes and edges)
	void insertSemanticsStatePropertiesInSpecificElemType(QDomDocument metamodel
			, QDomNodeList const &nodes, bool isNode) const;

	/// Insert in graphic types of semantics metamodel specific elements
	void insertSpecialSemanticsElements(QDomDocument metamodel, QString const &diagramName) const;

	/// Groups elements in semantics editor
	void insertPaletteGroups(QDomDocument metamodel, QString const &diagramDisplayedName) const;

	/// Add 'id' property to all elements in metamodel
	void insertIdPropertyToBasicElements(QDomDocument metamodel) const;

	/// Add to specific elements 'id' property
	void insertIdPropertyInSpecificElemType(QDomDocument metamodel, QDomNodeList const &nodes) const;

	/// Remove default values of all properties
	void removePropertyDefaultValues(QDomDocument metamodel) const;

	/// Delete directory (which was used for generate and compile semantics editor)
	void removeDirectory(QString const &dirName);

	qReal::ErrorReporterInterface *mErrorReporter;

	QAction *mGenerateAndLoadSemanticsEditorAction;
	QAction *mLoadSemanticsAction;
	QAction *mInterpretAction;
	QAction *mStopInterpretationAction;
	QAction *mWatchListAction;

	QMenu *mVisualInterpreterMenu;

	QList<qReal::ActionInfo> mActionInfos;

	qReal::VisualInterpreterPreferencesPage *mPreferencesPage;

	qReal::VisualInterpreterUnit *mVisualInterpreterUnit;
	utils::MetamodelGeneratorSupport *mMetamodelGeneratorSupport;
	utils::WatchListWindow *mWatchListWindow;
};

}
}
