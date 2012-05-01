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
	void generateRefactoringMetamodel();
	void openRefactoringWindow();
	void saveRefactoring();
	void arrangeElementsBT();
	void arrangeElementsLR();
	void arrangeElementsTB();
	void arrangeElementsRL();
	void findRefactoring(QString const &refactoringName);

private:
	void insertRefactoringID(QDomDocument metamodel, QDomNodeList list, bool isNode);
	void addRefactoringLanguageElements(QString diagramName, QDomDocument metamodel, QDomElement &graphics, QString const &pathToRefactoringMetamodel);
	QDomElement createPaletteElement(QString const &elementType, QDomDocument metamodel, const QString &displayedName);
	QDomElement metamodelPaletteGroup(QDomDocument metamodel, QDomNodeList nodeList, QDomNodeList edgeList);
	void addPalette(QDomDocument metamodel, QDomElement diagram, QDomElement metamodelPaletteGroup);
	void addPaletteGroup(QDomDocument metamodel, QDomElement palette, QString const &groupName, QStringList const &elementNameList);
	void addElementsToMetamodelGroup(QDomDocument metamodel, QDomNodeList list, QDomElement metamodelGroup);
	void arrangeElements(QString const &algorithm);

	qReal::ErrorReporterInterface *mErrorReporter;

	QMenu *mRefactoringMenu;
	QMenu *mPlaceMenu;
	QAction *mGenerateAndLoadRefactoringEditorAction;
	QAction *mOpenRefactoringWindowAction;
	QAction *mSaveRefactoringAction;
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

	qrRepo::RepoApi *mRefactoringRepoApi;
	RefactoringWindow *mRefactoringWindow;

	QTranslator mAppTranslator;
};
}
}
