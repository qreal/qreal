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
	mLogicalModelApi = &configurator.logicalModelApi();
	mGraphicalModelApi = &configurator.graphicalModelApi();

	mMetamodelGeneratorSupport = new MetamodelGeneratorSupport(
				configurator.mainWindowInterpretersInterface().errorReporter(),
				&configurator.mainWindowInterpretersInterface());
	mRepoControlIFace = &configurator.repoControlInterface();
	mMainWindowIFace = &configurator.mainWindowInterpretersInterface();
	mQRealSourceFilesPath = SettingsManager::value("qrealSourcesLocation", "").toString();
	mPathToRefactoringExamples = mQRealSourceFilesPath + "/plugins/refactoring/refactoringExamples/";
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

	mSaveRefactoringAction = new QAction(tr("Save Refactoring"), NULL);
	connect(mSaveRefactoringAction, SIGNAL(triggered()), this, SLOT(saveRefactoring()));
	mRefactoringMenu->addAction(mSaveRefactoringAction);

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
		if (editorMetamodelFilePath == "" || mQRealSourceFilesPath == "") {
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
	addPalette(metamodel, diagram, metamodelPaletteGroup(metamodel, metamodel.elementsByTagName("node"), metamodel.elementsByTagName("edge")));

	addRefactoringLanguageElements(diagramName, metamodel, graphics, mQRealSourceFilesPath + "/plugins/refactoring/editor/refactoringEditor.xml");
		mEditorElementNames.clear();

	QString metamodelName = diagramName + "RefactoringsMetamodel";
	QString relativeEditorPath = diagramName + "RefactoringsEditor";
	QString editorPath = mQRealSourceFilesPath + "/plugins/" + relativeEditorPath;

	mMetamodelGeneratorSupport->generateProFile(metamodel,
												editorMetamodelFilePath, mQRealSourceFilesPath, metamodelName,
												editorPath, relativeEditorPath
												);

	mMetamodelGeneratorSupport->saveMetamodelInFile(metamodel, editorPath + "/" + metamodelName + ".xml");

	mMetamodelGeneratorSupport->loadPlugin(editorPath, metamodelName
										   , SettingsManager::value("pathToQmake", "").toString()
										   , SettingsManager::value("pathToMake", "").toString()
										   , SettingsManager::value("pluginExtension", "").toString()
										   , SettingsManager::value("prefix", "").toString());

}

void RefactoringPlugin::insertRefactoringID(QDomDocument metamodel, QDomNodeList list, bool isNode)
{
	for (int i = 0; i < list.size(); ++i) {
		QDomElement element = list.at(i).toElement();
		mEditorElementNames.append(element.attribute("name", ""));

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
void RefactoringPlugin::addRefactoringLanguageElements(QString diagramName, QDomDocument metamodel, QDomElement &graphics, const QString &pathToRefactoringMetamodel)
{
	QDomDocument refactoringMetamodel = mMetamodelGeneratorSupport->loadMetamodelFromFile(pathToRefactoringMetamodel);
	QDomNodeList nodes = refactoringMetamodel.elementsByTagName("node");
	while (!nodes.isEmpty()) {
		QDomNode node = nodes.at(0);
		QDomElement nodeElement = node.toElement();
		QString const name = nodeElement.attribute("name", "");
		if (name == "beforeBlock" || name == "afterBlock") {
			QDomElement logic = nodeElement.elementsByTagName("logic").at(0).toElement();
			QDomElement container = logic.elementsByTagName("container").at(0).toElement();
			foreach (QString elementName, mEditorElementNames) {
				QDomElement contains = metamodel.createElement("contains");
				contains.setAttribute("type", diagramName + "::" + elementName);
				container.appendChild(contains);
			}
		}
		graphics.appendChild(node);
	}
	QDomNodeList edges = refactoringMetamodel.elementsByTagName("edge");
	while (!edges.isEmpty()) {
		QDomNode edge = edges.at(0);
		graphics.appendChild(edge);
	}
}

void RefactoringPlugin::openRefactoringWindow()
{
	RefactoringWindow *window = new RefactoringWindow(NULL);
	window->updateRefactorings(mPathToRefactoringExamples);
	window->show();
}

void RefactoringPlugin::saveRefactoring()
{
	IdList const childrenIDs = mLogicalModelApi->children(Id::rootId());
	QHash<Id, QString> diagramIds;
	foreach (Id const &childId, childrenIDs) {
		if (childId.element() == "RefactoringDiagramNode" && childId == mMainWindowIFace->activeDiagram()) {
			QString elementName = mGraphicalModelApi->name(childId).replace(" ", "")
					.replace("(", "").replace(")", "");
			if (elementName != "") {
				QString fileName = mPathToRefactoringExamples + elementName;
				diagramIds.insert(childId, fileName + ".qrs");
				mMainWindowIFace->saveDiagramAsAPictureToFile(fileName + ".png");
			}
		}
	}
	mRepoControlIFace->saveDiagramsById(diagramIds);
}

QDomElement RefactoringPlugin::createPaletteElement(QString const &elementType, QDomDocument metamodel, QString const &displayedName)
{
	QDomElement element = metamodel.createElement(elementType);
	element.setAttribute("name", displayedName);
	return element;
}

QDomElement RefactoringPlugin::metamodelPaletteGroup(QDomDocument metamodel, QDomNodeList nodeList, QDomNodeList edgeList)
{
	QDomElement metamodelGroup = createPaletteElement("group", metamodel, "Source Metamodel Elements");
	for (int i = 0; i < nodeList.size(); ++i) {
		QDomElement element = nodeList.at(i).toElement();
		QString const displayedName = element.attribute("displayedName", "");
		QDomElement paletteElement = createPaletteElement("element", metamodel, displayedName);
		metamodelGroup.appendChild(paletteElement);
	}
	for (int i = 0; i < edgeList.size(); ++i) {
		QDomElement element = edgeList.at(i).toElement();
		QString const displayedName = element.attribute("displayedName", "");
		QDomElement paletteElement = createPaletteElement("element", metamodel, displayedName);
		metamodelGroup.appendChild(paletteElement);
	}
	return metamodelGroup;
}

void RefactoringPlugin::addPalette(QDomDocument metamodel, QDomElement diagram, QDomElement metamodelPaletteGroup)
{
	QDomElement palette = metamodel.createElement("palette");
	QDomElement patternGroup = createPaletteElement("group", metamodel, "Refactoring Rule Elements");
	QDomElement refactoringDiagramNode = createPaletteElement("element", metamodel, "Refactoring Diagram");
	patternGroup.appendChild(refactoringDiagramNode);
	QDomElement fromBeforeToAter = createPaletteElement("element", metamodel, "From Before To After");
	patternGroup.appendChild(fromBeforeToAter);
	QDomElement afterBlock = createPaletteElement("element", metamodel, "After Block");
	patternGroup.appendChild(afterBlock);
	QDomElement beforeBlock = createPaletteElement("element", metamodel, "Before Block");
	patternGroup.appendChild(beforeBlock);
	palette.appendChild(patternGroup);
	QDomElement basicGroup = createPaletteElement("group", metamodel, "Basic Elements");
	QDomElement element = createPaletteElement("element", metamodel, "Element");
	basicGroup.appendChild(element);
	QDomElement link = createPaletteElement("element", metamodel, "Link");
	basicGroup.appendChild(link);
	QDomElement selectedSegment = createPaletteElement("element", metamodel, "Selected segment");
	basicGroup.appendChild(selectedSegment);
	palette.appendChild(basicGroup);
	palette.appendChild(metamodelPaletteGroup);
	diagram.appendChild(palette);
}
