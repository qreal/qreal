#include "visualInterpreterPlugin.h"

#include <QFileDialog>
#include <QProcess>
#include <QApplication>
#include <QProgressBar>
#include <QDesktopWidget>
#include <QtGui/QApplication>

#include "../../../qrutils/xmlUtils.h"
#include "../../../qrutils/outFile.h"

#include "../../../qrkernel/settingsManager.h"

Q_EXPORT_PLUGIN2(visualInterpreter, qReal::visualInterpreter::VisualInterpreterPlugin)

using namespace qReal;
using namespace visualInterpreter;
using namespace utils;

VisualInterpreterPlugin::VisualInterpreterPlugin()
	: mPreferencesPage(new VisualInterpreterPreferencesPage())
{
	mAppTranslator.load(":/visualInterpreter_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

VisualInterpreterPlugin::~VisualInterpreterPlugin()
{
}

void VisualInterpreterPlugin::init(PluginConfigurator const &configurator)
{
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
	
	mMetamodelGeneratorSupport = new MetamodelGeneratorSupport(
			configurator.mainWindowInterpretersInterface().errorReporter(),
			&configurator.mainWindowInterpretersInterface());
	
	mGraphTransformationUnit = new GraphTransformationUnit(configurator.logicalModelApi()
			, configurator.graphicalModelApi()
			, configurator.mainWindowInterpretersInterface());
}

QPair<QString, PreferencesPage *> VisualInterpreterPlugin::preferencesPage()
{
	return qMakePair(tr("Visual Interpreter"), static_cast<PreferencesPage*>(mPreferencesPage));
}

QList<qReal::ActionInfo> VisualInterpreterPlugin::actions()
{
	mGenerateAndLoadSemanticsEditorAction = new QAction(tr("Generate and load semantics editor"), NULL);
	ActionInfo generateSemanticsMetamodelActionInfo(mGenerateAndLoadSemanticsEditorAction, "semantics", "tools");
	connect(mGenerateAndLoadSemanticsEditorAction, SIGNAL(triggered()), this, SLOT(generateSemanticsMetamodel()));
	
	mLoadSemanticsAction = new QAction(tr("Load semantics model"), NULL);
	ActionInfo loadSemanticsActionInfo(mLoadSemanticsAction, "semantics", "tools");
	connect(mLoadSemanticsAction, SIGNAL(triggered()), this, SLOT(loadSemantics()));
	
	mInterpretAction = new QAction(tr("Interpret"), NULL);
	ActionInfo interpretActionInfo(mInterpretAction, "semantics", "tools");
	connect(mInterpretAction, SIGNAL(triggered()), this, SLOT(interpret()));
	
	mActionInfos << generateSemanticsMetamodelActionInfo << loadSemanticsActionInfo
			<< interpretActionInfo;
	
	return mActionInfos;
}

void VisualInterpreterPlugin::generateSemanticsMetamodel() {
	QString editorMetamodelFilePath =
			QFileDialog::getOpenFileName(NULL, tr("Specify editor metamodel:"));
	QString qrealSourceFilesPath = SettingsManager::value("qrealSourcesLocation", "").toString();
	
	if (editorMetamodelFilePath == "" || qrealSourceFilesPath == "") {
		return;
	}
	
	QDomDocument metamodel = mMetamodelGeneratorSupport->loadMetamodelFromFile(editorMetamodelFilePath);
	
	QDomElement diagram = mMetamodelGeneratorSupport->getDiagramElement(metamodel);
	QString diagramName = diagram.attribute("name");
	QString displayedName = diagram.attribute("displayedName");
	if (displayedName == "") {
		displayedName = diagramName;
	}
	
	diagram.setAttribute("displayedName", displayedName + " Semantics");
	
	insertSemanticsStatesEnum(metamodel);
	insertSematicsStateProperty(metamodel);
	insertSpecialSemanticsElements(metamodel, diagramName);
	
	QString metamodelName = diagramName + "SemanticsMetamodel";
	QString relativeEditorPath = diagramName + "SemanticsEditor";
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

void VisualInterpreterPlugin::insertSemanticsStatesEnum(QDomDocument metamodel) {
	QDomElement semanticsEnum = metamodel.createElement("enum");
	semanticsEnum.setAttribute("name", "SemanticsStatus");

	QDomElement semanticsStatusNew = metamodel.createElement("value");
	QDomText newText = metamodel.createTextNode("@new@");
	semanticsStatusNew.appendChild(newText);
	
	QDomElement semanticsStatusDeleted = metamodel.createElement("value");
	QDomText deletedText = metamodel.createTextNode("@deleted@");
	semanticsStatusDeleted.appendChild(deletedText);
	
	semanticsEnum.appendChild(semanticsStatusNew);
	semanticsEnum.appendChild(semanticsStatusDeleted);
	
	mMetamodelGeneratorSupport->insertElementInDiagramSublevel(metamodel,
			"nonGraphicTypes", semanticsEnum);
}

void VisualInterpreterPlugin::insertSematicsStateProperty(QDomDocument metamodel) {
	insertSematicsStatePropertyInSpecificElemType(metamodel, 
			metamodel.elementsByTagName("node"), true);
	insertSematicsStatePropertyInSpecificElemType(metamodel, 
			metamodel.elementsByTagName("edge"), false);

}

void VisualInterpreterPlugin::insertSematicsStatePropertyInSpecificElemType(
		QDomDocument metamodel ,QDomNodeList nodes, bool isNode)
{
	for (int i = 0; i < nodes.length(); i++) {
		QDomElement elem = nodes.at(i).toElement();
		
		QDomElement graphics = elem.elementsByTagName("graphics").at(0).toElement();
		QDomNodeList labels = graphics.elementsByTagName("labels");
		QString x = graphics.elementsByTagName("picture").at(0).toElement().attribute("sizex");
		
		QDomElement label = metamodel.createElement("label");
		if (isNode) {
			label.setAttribute("x", x);
			label.setAttribute("y", "0");
		}
		label.setAttribute("textBinded", "semanticsStatus");
		if (labels.length() > 0) {
			labels.at(0).insertBefore(label, labels.at(0).firstChild());
		} else {
			QDomElement labelsElem = metamodel.createElement("labels");
			labelsElem.appendChild(label);
			graphics.appendChild(labelsElem);
		}
		
		QDomElement logic = elem.elementsByTagName("logic").at(0).toElement();
		QDomNodeList properties = logic.elementsByTagName("properties");
		
		QDomElement property = metamodel.createElement("property");
		property.setAttribute("type", "SemanticsStatus");
		property.setAttribute("name", "semanticsStatus");
		if (properties.length() > 0) {
			properties.at(0).appendChild(property);
		} else {
			QDomElement propertiesElem = metamodel.createElement("properties");
			propertiesElem.appendChild(property);
			logic.appendChild(propertiesElem);
		}
	}
}

void VisualInterpreterPlugin::insertSpecialSemanticsElements(
		QDomDocument metamodel, QString const &diagramName)
{
	QString elementsXml = 
	"<semanticElements>"
	"<node displayedName=\"Semantics Rule\" name=\"SemanticsRule\">"
		"<graphics>"
			"<picture sizex=\"100\" sizey=\"100\">"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" y1=\"10\" x1=\"0\" y2=\"10\" stroke-width=\"2\" x2=\"100\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" y1=\"10\" x1=\"100\" y2=\"100\" stroke-width=\"2\" x2=\"100\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" y1=\"100\" x1=\"100\" y2=\"100\" stroke-width=\"2\" x2=\"0\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" y1=\"100\" x1=\"0\" y2=\"0\" stroke-width=\"2\" x2=\"0\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" y1=\"0\" x1=\"0\" y2=\"0\" stroke-width=\"2\" x2=\"50\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" y1=\"0\" x1=\"50\" y2=\"10\" stroke-width=\"2\" x2=\"50\" fill-style=\"solid\" />"
			"</picture>"
			"<labels>"
				"<label x=\"0\" y=\"0\" textBinded=\"ruleName\"/>"
				"<label x=\"0\" y=\"10\" textBinded=\"procedure\"/>"
			"</labels>"
		"</graphics>"
		"<logic>"
			"<properties>"
				"<property type=\"string\" name=\"ruleName\" />"
				"<property type=\"string\" name=\"procedure\" />"
			"</properties>"
			"<container>"
			"</container>"
		"</logic>"
	"</node>"
	"<node displayedName=\"Wildcard\" name=\"Wildcard\">"
		"<graphics>"
			"<picture sizex=\"50\" sizey=\"50\">"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" y1=\"10\" x1=\"10\" y2=\"40\" stroke-width=\"2\" x2=\"40\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" y1=\"10\" x1=\"40\" y2=\"40\" stroke-width=\"2\" x2=\"10\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" y1=\"1\" x1=\"25\" y2=\"49\" stroke-width=\"2\" x2=\"25\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" y1=\"25\" x1=\"1\" y2=\"25\" stroke-width=\"2\" x2=\"49\" fill-style=\"solid\" />"
			"</picture>"
			"<ports>"
				"<pointPort x=\"10\" y=\"10\"/>"
				"<pointPort x=\"1\" y=\"25\"/>"
				"<pointPort x=\"10\" y=\"40\"/>"
				"<pointPort x=\"25\" y=\"1\"/>"
				"<pointPort x=\"25\" y=\"49\"/>"
				"<pointPort x=\"40\" y=\"10\"/>"
				"<pointPort x=\"49\" y=\"25\"/>"
				"<pointPort x=\"40\" y=\"40\"/>"
			"</ports>"
		"</graphics>"
		"<logic>"
		"</logic>"
	"</node>"
	"<node displayedName=\"Control Flow Mark\" name=\"ControlFlowMark\">"
		"<graphics>"
			"<picture sizex=\"10\" sizey=\"10\">"
				"<ellipse fill=\"#FFFFFF\" stroke-style=\"solid\" stroke=\"#000000\" y1=\"0\" x1=\"0\" y2=\"10\" stroke-width=\"1\" x2=\"10\" fill-style=\"solid\"/>"
			"</picture>"
			"<ports>"
				"<pointPort x=\"0\" y=\"0\"/>"
				"<pointPort x=\"0\" y=\"10\"/>"
				"<pointPort x=\"10\" y=\"0\"/>"
				"<pointPort x=\"10\" y=\"10\"/>"
				"<pointPort x=\"5\" y=\"5\"/>"
			"</ports>"
			"<labels>"
				"<label x=\"10\" y=\"0\" textBinded=\"semanticsStatus\"/>"
			"</labels>"
		"</graphics>"
		"<logic>"
			"<properties>"
				"<property type=\"SemanticsStatus\" name=\"semanticsStatus\"/>"
			"</properties>"
		"</logic>"
	"</node>"
	"<edge displayedName=\"Replacement\" name=\"Replacement\">"
		"<graphics>"
			"<lineType type=\"dashLine\"/>"
		"</graphics>"
		"<logic>"
			"<associations endType=\"open_arrow\" beginType=\"no_arrow\">"
			"</associations>"
		"</logic>"
	"</edge>"
	"<edge displayedName=\"Control Flow Location\" name=\"ControlFlowLocation\">"
		"<graphics>"
			"<lineType type=\"dotLine\"/>"
		"</graphics>"
		"<logic>"
			"<associations endType=\"open_arrow\" beginType=\"no_arrow\">"
			"</associations>"
		"</logic>"
	"</edge>"
	"</semanticElements>";
	
	QDomDocument elements = mMetamodelGeneratorSupport->loadElementsFromString(elementsXml);
	QDomElement container = elements.elementsByTagName("container").at(0).toElement();

	QStringList elementNames = mMetamodelGeneratorSupport->collectAllGraphicTypesInMetamodel(metamodel);
	
	elementNames << "Wildcard" << "ControlFlowMark"
			<< "Replacement" << "ControlFlowLocation";
	
	mMetamodelGeneratorSupport->appendTypesToElement(
			elements, container, "contains", diagramName, elementNames);
	
	QDomNodeList semanticsElems = elements.firstChild().childNodes();
	
	mMetamodelGeneratorSupport->insertElementsInDiagramSublevel(
			metamodel, "graphicTypes", semanticsElems);
}

void VisualInterpreterPlugin::removeDirectory(QString dirName) {
	QDir dir(dirName);
	
	foreach (QFileInfo info, dir.entryInfoList(QDir::Hidden |
			QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files, QDir::DirsFirst)) {
		if (info.isDir()) {
			removeDirectory(info.absoluteFilePath());
		} else {
			QFile::remove(info.absoluteFilePath());
		}
	}

	dir.rmdir(dirName);
}

void VisualInterpreterPlugin::loadSemantics()
{
	mGraphTransformationUnit->loadSemantics();
}

void VisualInterpreterPlugin::interpret()
{
	mGraphTransformationUnit->interpret();
}
