#pragma once

#include <QtCore/QTranslator>
#include <QtGui/QMenu>

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

#include "../../../qrgui/mainwindow/errorReporter.h"

#include "../../../qrkernel/ids.h"

#include "../../../qrutils/metamodelGeneratorSupport.h"
#include "refactoringPreferencePage.h"

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

private:
	void insertRefactoringID(QDomDocument metamodel, QDomNodeList list, bool isNode);
	void addRefactoringLanguageElements(QDomElement &diagram, QString const &pathToRefactoringMetamodel);

	qReal::ErrorReporterInterface *mErrorReporter;

	QMenu *mRefactoringMenu;
	QAction *mGenerateAndLoadRefactoringEditorAction;
	QAction *mOpenRefactoringWindowAction;

	QList<qReal::ActionInfo> mActionInfos;

	qReal::RefactoringPreferencesPage *mPreferencesPage;

	utils::MetamodelGeneratorSupport *mMetamodelGeneratorSupport;

	QTranslator mAppTranslator;
};

}
}
