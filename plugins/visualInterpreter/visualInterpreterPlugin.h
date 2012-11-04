#pragma once

#include <QtCore/QTranslator>

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

public:
	VisualInterpreterPlugin();
	virtual ~VisualInterpreterPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

	virtual QPair<QString, PreferencesPage *> preferencesPage();

private slots:
	/// Generate, compile and load semantics editor for specified metamodel
	void generateSemanticsMetamodel() const;

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
	/// Insert possible semantics state of elements into semantics metamodel
	void insertSemanticsStatesEnum(QDomDocument metamodel) const;

	/// Add special semantics state property to all existing elements
	void insertSematicsStateProperty(QDomDocument metamodel) const;

	/// Add to specific elements semantics state property (different approach for nodes and edges)
	void insertSematicsStatePropertyInSpecificElemType(QDomDocument metamodel
			, QDomNodeList const &nodes, bool isNode) const;

	/// Insert in graphic types of semantics metamodel specific elements
	void insertSpecialSemanticsElements(QDomDocument metamodel, QString const &diagramName) const;

	/// Groups elements in semantics editor
	void insertPaletteGroups(QDomDocument metamodel, QString const &diagramDisplayedName) const;

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
	utils::watchListWindow *mWatchListWindow;

	QTranslator mAppTranslator;
};

}
}
