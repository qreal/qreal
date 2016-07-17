/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtWidgets/QMenu>

#include <qrkernel/ids.h>
#include <qrutils/metamodelGeneratorSupport.h>
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrgui/mainWindow/errorReporter.h>

#include "refactoringPreferencePage.h"
#include "refactoringWindow.h"
#include "refactoringFinder.h"
#include "refactoringApplier.h"

namespace qReal {
namespace refactoring {

/// Refactoring plugin allows you to automatically arrange elements,
/// create refactorings editor based on specified metamodel or use default,
/// save created refactorings and apply 3 predefined refactorings
/// (change element names, direction of edges, cut the selected block of elements in the procedure)
class RefactoringPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.refactoring.RefactoringPlugin")

public:
	RefactoringPlugin();
	virtual ~RefactoringPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

	virtual QPair<QString, gui::PreferencesPage *> preferencesPage();

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
	void insertRefactoringID(QDomDocument metamodel, const QDomNodeList &list, bool isNode);

	/// add to chosen metamodel refactoring elements and make all elements source metamodel
	/// children of containers "Before" and "After"
	/// @param diagramName name of the source diagram
	/// @param metamodel metamodel for integranion with refactoring language
	/// @param graphics xml-element, refactoring elements will be its children
	/// @param pathToRefactoringMetamodel absolute path to Refactoring Metmaodel
	void addRefactoringLanguageElements(const QString &diagramName, QDomDocument metamodel
			, QDomElement &graphics, QString const &pathToRefactoringMetamodel);


	QDomElement createPaletteElement(QString const &elementType, QDomDocument metamodel, const QString &name);
	QDomElement metamodelPaletteGroup(QDomDocument metamodel
			, const QDomNodeList &nodeList, const QDomNodeList &edgeList);

	void addPalette(QDomDocument metamodel, QDomElement diagram
			, const QDomElement &metamodelPaletteGroup);
	void addPaletteGroup(QDomDocument metamodel, QDomElement palette
			, QString const &groupName, QStringList const &elementNameList);
	void addElementsToMetamodelGroup(QDomDocument metamodel, const QDomNodeList &list
			, QDomElement &metamodelGroup);
	const QString nameForPaletteGroup(const QString &name);
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
};
}
}
