#pragma once

#include <QtCore/QTranslator>

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

#include "visualInterpreterPreferencesPage.h"

#include "../../../qrgui/mainwindow/errorReporter.h"

#include "../../../qrkernel/ids.h"

#include "../../../qrutils/metamodelGeneratorSupport.h"
#include "graphTransformationUnit.h"

namespace qReal {
namespace visualInterpreter {

/// Visual interpreter plugin for any editor. Allows create semantics for any
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
	void generateSemanticsMetamodel();
	
	/// Load semantics model from current open diagram
	void loadSemantics();
	
	/// Make one step according to semantics (find match, delete, create and replace elements)
	void interpret();

private:
	/// Insert in semantics metamodel possible semantics state of elements
	void insertSemanticsStatesEnum(QDomDocument metamodel);
	
	/// Add to all existing elements special semantics state property
	void insertSematicsStateProperty(QDomDocument metamodel);
	
	/// Add to specific elements semantics state property (different approach for nodes and edges)
	void insertSematicsStatePropertyInSpecificElemType(QDomDocument metamodel,
			QDomNodeList nodes, bool isNode);
	
	/// Insert in graphic types of semantics metamodel specific elements
	void insertSpecialSemanticsElements(QDomDocument metamodel,
			QString const &diagramName);
	
	/// Delete directory (which was used for generate and compile semantics editor)
	void removeDirectory(QString dirName);
	
	
	qReal::ErrorReporterInterface *mErrorReporter;

	QAction *mGenerateAndLoadSemanticsEditorAction;
	QAction *mLoadSemanticsAction;
	QAction *mInterpretAction;
	
	QList<qReal::ActionInfo> mActionInfos;
	
	qReal::VisualInterpreterPreferencesPage *mPreferencesPage;
	
	qReal::GraphTransformationUnit *mGraphTransformationUnit;
	utils::MetamodelGeneratorSupport *mMetamodelGeneratorSupport;
	
	QTranslator mAppTranslator;
};

}
}
