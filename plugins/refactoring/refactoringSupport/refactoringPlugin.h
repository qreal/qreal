#pragma once

#include <QtCore/QTranslator>
#include <QtGui/QMenu>

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

#include "../../../qrgui/mainwindow/errorReporter.h"

#include "../../../qrkernel/ids.h"

#include "../../../qrutils/metamodelGeneratorSupport.h"
#include "refactoringPreferencePage.h"
#include "refactoringWindow.h"
#include "refactoringFinder.h"
#include "refactoringApplier.h"

namespace qReal {
namespace refactoring {

class RefactoringPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	RefactoringPlugin();
	virtual ~RefactoringPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

	virtual QPair<QString, PreferencesPage *> preferencesPage();

private slots:
	/// generate and plugged refactoring metamodel, integrated with chosen metamodel (by user)
	void generateRefactoringMetamodel();

	/// open dialog with list of refactoring rules, which can be applyed
	void openRefactoringWindow();

	/// save .qrs file with the refactoring rule on active diagram,
	/// only if this diagram has refactoring type (RefactoringDiagramNode)
	/// and picture (.png) with this rule.
	/// names of .qrs and .png are the same as name on the diagram
	void saveRefactoring();

	/// automatically arrange elements Bottom-Top by dotRunner
	void arrangeElementsBT();

	/// automatically arrange elements Left-Right by dotRunner
	void arrangeElementsLR();

	/// automatically arrange elements Top-Bottom by dotRunner
	void arrangeElementsTB();

	/// automatically arrange elements Right-Left by dotRunner
	void arrangeElementsRL();

	/// find first place for applying refactoring on the active diagram
	/// found place is highlighted
	/// @param refactoringName name of .qrs with refactoring rule
	void findRefactoring(QString const &refactoringName);

	/// find another place for applying refactoring on the active diagram
	/// found place is highlighted
	/// is enabled only after find first refactoring place
	void findNextRefactoring();

	/// clear all highlighted place on the diagram and clear all matchings
	void discardRefactoring();

	/// create the pattern of refactoring rule by means of common refactoring language
	/// (without specific editor elements), i.e. on this way of starting creation
	/// refactoring rule only basic elements can be used (other elements are not
	/// children of container "Before" and "After")
	void createRefactoring();

	/// apply refactoring rule on the highlighted place after it has been found
	void applyRefactoring();

private:
	/// insert property "ID" in chosen by user metamodel in the chosen elements
	/// @param metamodel metamodel for integranion with refactoring language
	/// @param list list of the elements for adding property "ID"
	/// @param isNode true, if all elements in list are Nodes, false, if
	/// all elements in list are Edges
	void insertRefactoringID(QDomDocument metamodel, QDomNodeList list, bool isNode);

	/// add to chosen metamodel refactoring elements and make all elements source metamodel
	/// children of containers "Before" and "After"
	/// @param diagramName name of the source diagram
	/// @param metamodel metamodel for integranion with refactoring language
	/// @param graphics xml-element, refactoring elements will be its children
	/// @param pathToRefactoringMetamodel absolute path to Refactoring Metmaodel
	void addRefactoringLanguageElements(QString diagramName, QDomDocument metamodel, QDomElement &graphics, QString const &pathToRefactoringMetamodel);


	QDomElement createPaletteElement(QString const &elementType, QDomDocument metamodel, const QString &displayedName);
	QDomElement metamodelPaletteGroup(QDomDocument metamodel, QDomNodeList nodeList, QDomNodeList edgeList);
	void addPalette(QDomDocument metamodel, QDomElement diagram, QDomElement metamodelPaletteGroup);
	void addPaletteGroup(QDomDocument metamodel, QDomElement palette, QString const &groupName, QStringList const &elementNameList);
	void addElementsToMetamodelGroup(QDomDocument metamodel, QDomNodeList list, QDomElement metamodelGroup);
	void arrangeElements(QString const &algorithm);
	QList<QPair<Id, QPair<Id, bool> > > findOutsideSelectionLinks();
	void removeUnnecessaryLinksFromSelected();
	void makeSubprogramHARDCODE();

	qReal::ErrorReporterInterface *mErrorReporter;

	QMenu *mRefactoringMenu;
	QMenu *mPlaceMenu;
	QAction *mGenerateAndLoadRefactoringEditorAction;
	QAction *mOpenRefactoringWindowAction;
	QAction *mSaveRefactoringAction;
	QAction *mCreateRefactoringAction;
	QAction *mPlaceLRAction;
	QAction *mPlaceTBAction;
	QAction *mPlaceRLAction;
	QAction *mPlaceBTAction;

	LogicalModelAssistInterface *mLogicalModelApi;
	GraphicalModelAssistInterface *mGraphicalModelApi;
	qrRepo::RepoControlInterface *mRepoControlIFace;
	gui::MainWindowInterpretersInterface *mMainWindowIFace;

	QString mQRealSourceFilesPath;
	QString mPathToRefactoringExamples;

	QList<qReal::ActionInfo> mActionInfos;
	QStringList mEditorElementNames;

	RefactoringPreferencesPage *mPreferencesPage;

	utils::MetamodelGeneratorSupport *mMetamodelGeneratorSupport;
	RefactoringFinder *mRefactoringFinder;
	RefactoringApplier *mRefactoringApplier;

	qrRepo::RepoApi *mRefactoringRepoApi;
	RefactoringWindow *mRefactoringWindow;
	QList<QHash<Id, Id> > mMatches;
	QHash <Id, Id> mCurrentMatch;
	IdList mSelectedElementsOnActiveDiagram;

	QTranslator mAppTranslator;
};
}
}
