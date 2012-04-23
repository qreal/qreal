#include "refactoringPlugin.h"

#include <QFileDialog>
#include <QProcess>
#include <QApplication>
#include <QProgressBar>
#include <QDesktopWidget>
#include <QtGui/QApplication>

#include "../../../qrutils/xmlUtils.h"
#include "../../../qrutils/outFile.h"
#include "../../../qrkernel/settingsManager.h"
#include "refactoringWindow.h"

using namespace qReal;
using namespace refactoring;
using namespace utils;

Q_EXPORT_PLUGIN2(refactoring, qReal::refactoring::RefactoringPlugin)

RefactoringPlugin::RefactoringPlugin()
	: mPreferencesPage(new RefactoringPreferencesPage())
{
	mAppTranslator.load(":/refactoringSupport_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

RefactoringPlugin::~RefactoringPlugin()
{
}

void RefactoringPlugin::init(PluginConfigurator const &configurator)
{
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();

	mMetamodelGeneratorSupport = new MetamodelGeneratorSupport(
			configurator.mainWindowInterpretersInterface().errorReporter(),
			&configurator.mainWindowInterpretersInterface());
}

QPair<QString, PreferencesPage *> RefactoringPlugin::preferencesPage()
{
	return qMakePair(tr("Refactorings"), static_cast<PreferencesPage*>(mPreferencesPage));
}

QList<qReal::ActionInfo> RefactoringPlugin::actions()
{
	mRefactoringMenu = new QMenu(tr("Refactoring"));
	ActionInfo refactoringMenuInfo(mRefactoringMenu, "tools");

	mGenerateAndLoadRefactoringEditorAction = new QAction(tr("Generate and load refactoring editor"), NULL);
	connect(mGenerateAndLoadRefactoringEditorAction, SIGNAL(triggered()), this, SLOT(generateRefactoringMetamodel()));
	mRefactoringMenu->addAction(mGenerateAndLoadRefactoringEditorAction);

	mOpenRefactoringWindowAction = new QAction(tr("Open Refactoring Window"), NULL);
	connect(mOpenRefactoringWindowAction, SIGNAL(triggered()), this, SLOT(openRefactoringWindow()));
	mRefactoringMenu->addAction(mOpenRefactoringWindowAction);


	mActionInfos << refactoringMenuInfo;

	return mActionInfos;
}

void RefactoringPlugin::generateRefactoringMetamodel()
{
	QDir currentDir(".");

	QString editorMetamodelFilePath =
			QFileDialog::getOpenFileName(
			NULL
			, tr("Select xml file to load refactoring metamodel")
			, currentDir.absolutePath()
			, "XML files (*.xml)"
			);
	QString qrealSourceFilesPath = SettingsManager::value("qrealSourcesLocation", "").toString();;
	if (editorMetamodelFilePath == "" || qrealSourceFilesPath == "") {
		return;
	}

	QDomDocument metamodel = mMetamodelGeneratorSupport->loadMetamodelFromFile(editorMetamodelFilePath);

	QDomElement diagram = mMetamodelGeneratorSupport->getDiagramElement(metamodel);
	QDomElement graphics = metamodel.elementsByTagName("graphicTypes").at(0).toElement();
	QString diagramName = diagram.attribute("name").replace(" ", "_");
	QString displayedName = diagram.attribute("displayedName");
	if (displayedName == "") {
		displayedName = diagramName;
	}

	diagram.setAttribute("displayedName", displayedName + " Refactorings");
	insertRefactoringID(metamodel, metamodel.elementsByTagName("node"), true);
	insertRefactoringID(metamodel, metamodel.elementsByTagName("edge"), false);
	addRefactoringLanguageElements(graphics, qrealSourceFilesPath + "/plugins/refactoring/editor/refactoringEditor.xml");

	QString metamodelName = diagramName + "RefactoringsMetamodel";
	QString relativeEditorPath = diagramName + "RefactoringsEditor";
	QString editorPath = qrealSourceFilesPath + "/plugins/" + relativeEditorPath;

	mMetamodelGeneratorSupport->generateProFile(metamodel,
			editorMetamodelFilePath, qrealSourceFilesPath, metamodelName,
			editorPath, relativeEditorPath
	);

	mMetamodelGeneratorSupport->saveMetamodelInFile(metamodel, editorPath + "/" + metamodelName + ".xml");

	mMetamodelGeneratorSupport->loadPlugin(editorPath, metamodelName
			, SettingsManager::value("pathToQmake", "").toString()
			, SettingsManager::value("pathToMake", "").toString()
			, SettingsManager::value("pluginExtension", "").toString()
			, SettingsManager::value("prefix", "").toString());

	//removeDirectory(editorPath);
}

void RefactoringPlugin::insertRefactoringID(QDomDocument metamodel, QDomNodeList list, bool isNode)
{
	for (int i = 0; i < list.size(); ++i) {
		QDomElement element = list.at(i).toElement();

		QDomElement graphics = element.elementsByTagName("graphics").at(0).toElement();
		QDomNodeList labels = graphics.elementsByTagName("labels");
		QString x = graphics.elementsByTagName("picture").at(0).toElement().attribute("sizex");

		QDomElement label = metamodel.createElement("label");
		if (isNode) {
			label.setAttribute("x", x);
			label.setAttribute("y", "0");
		}
		label.setAttribute("textBinded", "ID");
		if (labels.length() > 0) {
			labels.at(0).insertBefore(label, labels.at(0).firstChild());
		} else {
			QDomElement labelsElem = metamodel.createElement("labels");
			labelsElem.appendChild(label);
			graphics.appendChild(labelsElem);
		}

		QDomElement logic = element.elementsByTagName("logic").at(0).toElement();
		QDomNodeList properties = logic.elementsByTagName("properties");

		QDomElement property = metamodel.createElement("property");
		property.setAttribute("type", "ID");
		property.setAttribute("name", "ID");
		if (properties.length() > 0) {
			properties.at(0).appendChild(property);
		} else {
			QDomElement propertiesElem = metamodel.createElement("properties");
			propertiesElem.appendChild(property);
			logic.appendChild(propertiesElem);
		}
	}
}
void RefactoringPlugin::addRefactoringLanguageElements(QDomElement &diagram, const QString &pathToRefactoringMetamodel)
{
	QDomDocument refactoringMetamodel = mMetamodelGeneratorSupport->loadMetamodelFromFile(pathToRefactoringMetamodel);
	QDomNodeList nodes = refactoringMetamodel.elementsByTagName("node");
	for (int i = 0; i < nodes.size(); ++i) {
		QDomNode element = nodes.at(i);
		diagram.appendChild(element);
	}
	QDomNodeList edges = refactoringMetamodel.elementsByTagName("edge");
	for (int i = 0; i < edges.size(); ++i) {
		QDomNode element = edges.at(i);
		diagram.appendChild(element);
	}
}

void RefactoringPlugin::openRefactoringWindow()
{
	RefactoringWindow *window = new RefactoringWindow(NULL);
	window->show();
}
