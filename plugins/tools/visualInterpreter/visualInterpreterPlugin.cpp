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

#include <QtCore/QProcess>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>

#include "visualInterpreterPlugin.h"

#include <qrutils/xmlUtils.h>
#include <qrutils/outFile.h>
#include <qrkernel/settingsManager.h>

using namespace qReal;
using namespace visualInterpreter;
using namespace utils;

VisualInterpreterPlugin::VisualInterpreterPlugin()
	: mPreferencesPage(new VisualInterpreterPreferencesPage())
	, mWatchListWindow(nullptr)
{
}

VisualInterpreterPlugin::~VisualInterpreterPlugin()
{
}

void VisualInterpreterPlugin::init(PluginConfigurator const &configurator)
{
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();

	mMetamodelGeneratorSupport = new MetamodelGeneratorSupport(
			configurator.mainWindowInterpretersInterface().errorReporter()
			, &configurator.mainWindowInterpretersInterface());

	mVisualInterpreterUnit = new VisualInterpreterUnit(configurator.logicalModelApi()
			, configurator.graphicalModelApi()
			, configurator.mainWindowInterpretersInterface());
}

QPair<QString, gui::PreferencesPage *> VisualInterpreterPlugin::preferencesPage()
{
	return qMakePair(tr("Visual Interpreter"), static_cast<gui::PreferencesPage *>(mPreferencesPage));
}

QList<qReal::ActionInfo> VisualInterpreterPlugin::actions()
{
	mVisualInterpreterMenu = new QMenu(tr("Visual interpret"));
	ActionInfo visualInterpreterMenu(mVisualInterpreterMenu, "tools");

	mGenerateAndLoadSemanticsEditorAction = new QAction(tr("Generate and load semantics editor"), nullptr);
	connect(mGenerateAndLoadSemanticsEditorAction, SIGNAL(triggered()), this, SLOT(generateMetamodels()));
	mVisualInterpreterMenu->addAction(mGenerateAndLoadSemanticsEditorAction);

	mLoadSemanticsAction = new QAction(tr("Load semantics model"), nullptr);
	mLoadSemanticsAction->setShortcut(QKeySequence(Qt::Key_Q));
	connect(mLoadSemanticsAction, SIGNAL(triggered()), this, SLOT(loadSemantics()));
	mVisualInterpreterMenu->addAction(mLoadSemanticsAction);

	mInterpretAction = new QAction(tr("Interpret"), nullptr);
	mInterpretAction->setShortcut(QKeySequence(Qt::Key_W));
	connect(mInterpretAction, SIGNAL(triggered()), this, SLOT(interpret()));
	mVisualInterpreterMenu->addAction(mInterpretAction);

	mStopInterpretationAction = new QAction(tr("Stop interpretation"), nullptr);
	mStopInterpretationAction->setShortcut(QKeySequence(Qt::Key_E));
	connect(mStopInterpretationAction, SIGNAL(triggered()), this, SLOT(stopInterpretation()));
	mVisualInterpreterMenu->addAction(mStopInterpretationAction);

	mWatchListAction = new QAction(tr("Show watch list"), nullptr);
	connect(mWatchListAction, SIGNAL(triggered()), this, SLOT(showWatchList()));
	mVisualInterpreterMenu->addAction(mWatchListAction);

	mActionInfos << visualInterpreterMenu;

	return mActionInfos;
}

void VisualInterpreterPlugin::generateMetamodels() const
{
	QString editorMetamodelFilePath =
			QFileDialog::getOpenFileName(nullptr, tr("Specify editor metamodel:"), QString(), "xml (*.xml)");
	QString qrealSourceFilesPath = SettingsManager::value("qrealSourcesLocation").toString();

	if (editorMetamodelFilePath.isEmpty() || qrealSourceFilesPath.isEmpty()) {
		return;
	}

	generateSemanticsMetamodel(editorMetamodelFilePath, qrealSourceFilesPath);
	generateEditorMetamodel(editorMetamodelFilePath, qrealSourceFilesPath);
}

void VisualInterpreterPlugin::generateSemanticsMetamodel(QString const &editorMetamodelFilePath
		, QString const &qrealSourceFilesPath) const
{
	QDomDocument const metamodel = mMetamodelGeneratorSupport->loadMetamodelFromFile(editorMetamodelFilePath);

	QDomElement diagram = mMetamodelGeneratorSupport->diagramElement(metamodel);
	QString const diagramName = diagram.attribute("name");
	QString const displayedName = diagram.attribute("displayedName").isEmpty()
			? diagramName
			: diagram.attribute("displayedName");

	diagram.setAttribute("displayedName", displayedName + " Semantics");

	removePropertyDefaultValues(metamodel);
	insertSemanticsEnums(metamodel, "SemanticsStatus", QStringList() << "@new@" << "@deleted@");
	insertSemanticsEnums(metamodel, "LanguageType", QStringList() << "Block Scheme (C-like)" << "Python" << "QtScript");
	insertSemanticsEnums(metamodel, "SemanticsType", QStringList() << "Interpretation" << "Generation");
	insertSematicsStateProperty(metamodel);
	insertPaletteGroups(metamodel, displayedName);
	insertSpecialSemanticsElements(metamodel, diagramName);

	QString const metamodelName = diagramName + "SemanticsMetamodel";
	QString const relativeEditorPath = diagramName + "SemanticsEditor";
	QString const editorPath = qrealSourceFilesPath + "/plugins/" + relativeEditorPath;

	mMetamodelGeneratorSupport->generateProFile(metamodel
			, editorMetamodelFilePath, qrealSourceFilesPath, metamodelName
			, editorPath, relativeEditorPath);

	mMetamodelGeneratorSupport->saveMetamodelInFile(metamodel, editorPath
			+ "/" + metamodelName + ".xml");

	mMetamodelGeneratorSupport->loadPlugin(editorPath, metamodelName
			, SettingsManager::value("pathToQmake").toString()
			, SettingsManager::value("pathToMake").toString()
			, SettingsManager::value("pluginExtension").toString()
			, SettingsManager::value("prefix").toString());
}

void VisualInterpreterPlugin::generateEditorMetamodel(QString const &editorMetamodelFilePath
		, QString const &qrealSourceFilesPath) const
{
	QDomDocument const metamodel = mMetamodelGeneratorSupport->loadMetamodelFromFile(editorMetamodelFilePath);

	QDomElement diagram = mMetamodelGeneratorSupport->diagramElement(metamodel);
	QString const diagramName = diagram.attribute("name");
	QString const displayedName = diagram.attribute("displayedName").isEmpty()
			? diagramName
			: diagram.attribute("displayedName");

	diagram.setAttribute("displayedName", displayedName + " With Semantics Properties");

	insertIdPropertyToBasicElements(metamodel);

	QString const metamodelName = diagramName + "WithSemanticsMetamodel";
	QString const relativeEditorPath = diagramName + "WithSemanticsEditor";
	QString const editorPath = qrealSourceFilesPath + "/plugins/" + relativeEditorPath;

	mMetamodelGeneratorSupport->generateProFile(metamodel
			, editorMetamodelFilePath, qrealSourceFilesPath, metamodelName
			, editorPath, relativeEditorPath);

	mMetamodelGeneratorSupport->saveMetamodelInFile(metamodel, editorPath
			+ "/" + metamodelName + ".xml");

	mMetamodelGeneratorSupport->loadPlugin(editorPath, metamodelName
			, SettingsManager::value("pathToQmake").toString()
			, SettingsManager::value("pathToMake").toString()
			, SettingsManager::value("pluginExtension").toString()
			, SettingsManager::value("prefix").toString());
}

void VisualInterpreterPlugin::insertSemanticsEnums(QDomDocument metamodel, QString const &name
		, QStringList const &values) const
{
	QDomElement semanticsEnum = metamodel.createElement("enum");
	semanticsEnum.setAttribute("name", name);

	foreach (QString const &value, values) {
		QDomElement elem = metamodel.createElement("value");
		QDomText text = metamodel.createTextNode(value);
		elem.appendChild(text);
		semanticsEnum.appendChild(elem);
	}

	mMetamodelGeneratorSupport->insertElementInDiagramSublevel(metamodel, "nonGraphicTypes", semanticsEnum);
}

void VisualInterpreterPlugin::insertSematicsStateProperty(QDomDocument metamodel) const
{
	insertSemanticsStatePropertiesInSpecificElemType(metamodel, metamodel.elementsByTagName("node"), true);
	insertSemanticsStatePropertiesInSpecificElemType(metamodel, metamodel.elementsByTagName("edge"), false);
}

void VisualInterpreterPlugin::insertSemanticsStatePropertiesInSpecificElemType(QDomDocument metamodel
		, QDomNodeList const &nodes, bool isNode) const
{
	for (int i = 0; i < nodes.length(); i++) {
		QDomElement const elem = nodes.at(i).toElement();

		QDomElement graphics = elem.elementsByTagName("graphics").at(0).toElement();
		QDomNodeList labels = graphics.elementsByTagName("labels");
		QString const x = graphics.elementsByTagName("picture").at(0).toElement().attribute("sizex");

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

		QDomElement semStatusProperty = metamodel.createElement("property");
		semStatusProperty.setAttribute("type", "SemanticsStatus");
		semStatusProperty.setAttribute("name", "semanticsStatus");
		if (properties.length() > 0) {
			properties.at(0).appendChild(semStatusProperty);
		} else {
			QDomElement propertiesElem = metamodel.createElement("properties");
			propertiesElem.appendChild(semStatusProperty);
			logic.appendChild(propertiesElem);
		}

		QDomElement idProperty = metamodel.createElement("property");
		QDomElement defaultValue = metamodel.createElement("default");
		QDomText val = metamodel.createTextNode(QString("-1"));
		defaultValue.appendChild(val);
		idProperty.setAttribute("type", "int");
		idProperty.setAttribute("name", "id");
		idProperty.appendChild(defaultValue);
		logic.elementsByTagName("properties").at(0).appendChild(idProperty);
	}
}

void VisualInterpreterPlugin::insertIdPropertyToBasicElements(QDomDocument metamodel) const
{
	insertIdPropertyInSpecificElemType(metamodel, metamodel.elementsByTagName("node"));
	insertIdPropertyInSpecificElemType(metamodel, metamodel.elementsByTagName("edge"));
}

void VisualInterpreterPlugin::insertIdPropertyInSpecificElemType(QDomDocument metamodel
		, QDomNodeList const &nodes) const
{
	for (int i = 0; i < nodes.length(); i++) {
		QDomElement const elem = nodes.at(i).toElement();

		QDomElement logic = elem.elementsByTagName("logic").at(0).toElement();
		QDomNodeList properties = logic.elementsByTagName("properties");

		QDomElement idProperty = metamodel.createElement("property");
		QDomElement defaultValue = metamodel.createElement("default");
		QDomText val = metamodel.createTextNode(QString("-1"));
		defaultValue.appendChild(val);
		idProperty.setAttribute("type", "int");
		idProperty.setAttribute("name", "id");
		idProperty.appendChild(defaultValue);
		if (properties.length() > 0) {
			properties.at(0).appendChild(idProperty);
		} else {
			QDomElement propertiesElem = metamodel.createElement("properties");
			propertiesElem.appendChild(idProperty);
			logic.appendChild(propertiesElem);
		}
	}
}

void VisualInterpreterPlugin::removePropertyDefaultValues(QDomDocument metamodel) const
{
	QDomNodeList properties = metamodel.elementsByTagName("property");
	for (int i = 0; i < properties.length(); i++) {
		properties.at(i).removeChild(properties.at(i).toElement().elementsByTagName("default").at(0));
	}
}

void VisualInterpreterPlugin::insertSpecialSemanticsElements(QDomDocument metamodel, QString const &diagramName) const
{
	QString const elementsXml =
	"<semanticElements>"
	"<node displayedName=\"Semantics Rule\" name=\"SemanticsRule\">"
		"<graphics>"
			"<picture sizex=\"100\" sizey=\"100\">"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"10\" x1=\"0\" y2=\"10\" stroke-width=\"2\" x2=\"100\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"10\" x1=\"100\" y2=\"100\" stroke-width=\"2\" x2=\"100\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"100\" x1=\"100\" y2=\"100\" stroke-width=\"2\" x2=\"0\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"100\" x1=\"0\" y2=\"0\" stroke-width=\"2\" x2=\"0\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"0\" x1=\"0\" y2=\"0\" stroke-width=\"2\" x2=\"50\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"0\" x1=\"50\" y2=\"10\" stroke-width=\"2\" x2=\"50\" fill-style=\"solid\" />"
			"</picture>"
			"<labels>"
				"<label x=\"0\" y=\"0\" textBinded=\"ruleName\"/>"
			"</labels>"
		"</graphics>"
		"<logic>"
			"<properties>"
				"<property type=\"string\" name=\"ruleName\" />"
				"<property type=\"code\" name=\"procedure\" />"
				"<property type=\"LanguageType\" name=\"type\">"
					"<default>Block Scheme (C-like)</default>"
				"</property>"
				"<property type=\"code\" name=\"applicationCondition\" />"
				"<property type=\"int\" name=\"priority\">"
					"<default>0</default>"
				"</property>"
			"</properties>"
			"<container>"
			"</container>"
		"</logic>"
	"</node>"
	"<node displayedName=\"Wildcard\" name=\"Wildcard\">"
		"<graphics>"
			"<picture sizex=\"50\" sizey=\"50\">"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"10\" x1=\"10\" y2=\"40\" stroke-width=\"2\" x2=\"40\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"10\" x1=\"40\" y2=\"40\" stroke-width=\"2\" x2=\"10\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"1\" x1=\"25\" y2=\"49\" stroke-width=\"2\" x2=\"25\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"25\" x1=\"1\" y2=\"25\" stroke-width=\"2\" x2=\"49\" fill-style=\"solid\" />"
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
			"<properties>"
				"<property type=\"int\" name=\"id\">"
					"<default>-1</default>"
				"</property>"
			"</properties>"
		"</logic>"
	"</node>"
	"<node displayedName=\"Initialization\" name=\"Initialization\">"
		"<graphics>"
			"<picture sizex=\"50\" sizey=\"50\">"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"0\" x1=\"0\" y2=\"40\" stroke-width=\"2\" x2=\"0\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"0\" x1=\"0\" y2=\"0\" stroke-width=\"2\" x2=\"40\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"40\" x1=\"0\" y2=\"40\" stroke-width=\"2\" x2=\"40\" fill-style=\"solid\" />"
				"<line fill=\"#000000\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"0\" x1=\"40\" y2=\"40\" stroke-width=\"2\" x2=\"40\" fill-style=\"solid\" />"
			"</picture>"
		"</graphics>"
		"<logic>"
			"<properties>"
				"<property type=\"code\" name=\"initializationCode\" />"
				"<property type=\"LanguageType\" name=\"languageType\">"
					"<default>Block Scheme (C-like)</default>"
				"</property>"
				"<property type=\"SemanticsType\" name=\"semanticsType\">"
					"<default>Interpretation</default>"
				"</property>"
			"</properties>"
		"</logic>"
	"</node>"
	"<node displayedName=\"Control Flow Mark\" name=\"ControlFlowMark\">"
		"<graphics>"
			"<picture sizex=\"20\" sizey=\"20\">"
				"<ellipse fill=\"#FFFFFF\" stroke-style=\"solid\" stroke=\"#000000\" "
						"y1=\"0\" x1=\"0\" y2=\"20\" stroke-width=\"1\" x2=\"20\" fill-style=\"solid\"/>"
			"</picture>"
			"<ports>"
				"<pointPort x=\"0\" y=\"10\"/>"
				"<pointPort x=\"10\" y=\"0\"/>"
				"<pointPort x=\"10\" y=\"20\"/>"
				"<pointPort x=\"20\" y=\"10\"/>"
				"<pointPort x=\"10\" y=\"10\"/>"
			"</ports>"
			"<labels>"
				"<label x=\"20\" y=\"0\" textBinded=\"semanticsStatus\"/>"
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

	QDomDocument const elements = mMetamodelGeneratorSupport->loadElementsFromString(elementsXml);
	QDomElement const container = elements.elementsByTagName("container").at(0).toElement();

	QStringList elementNames = mMetamodelGeneratorSupport->collectAllGraphicTypesInMetamodel(metamodel, false);
	elementNames << "Wildcard" << "ControlFlowMark" << "Replacement" << "ControlFlowLocation";

	mMetamodelGeneratorSupport->appendTypesToElement(elements, container
			, "contains", diagramName, elementNames);

	QDomNodeList const semanticsElems = elements.firstChild().childNodes();
	mMetamodelGeneratorSupport->insertElementsInDiagramSublevel(metamodel, "graphicTypes", semanticsElems);
}

void VisualInterpreterPlugin::insertPaletteGroups(QDomDocument metamodel, QString const &diagramDisplayedName) const
{
	QString const elementsXml =
	"<palette>"
		"<group name=\"Semantics Elements\">"
			"<element name=\"SemanticsRule\"/>"
			"<element name=\"Wildcard\"/>"
			"<element name=\"ControlFlowMark\"/>"
			"<element name=\"Replacement\"/>"
			"<element name=\"ControlFlowLocation\"/>"
			"<element name=\"Initialization\"/>"
		"</group>"
	"</palette>";

	QDomDocument elements = mMetamodelGeneratorSupport->loadElementsFromString(elementsXml);
	QStringList const elementNames =
			mMetamodelGeneratorSupport->collectAllGraphicTypesInMetamodel(metamodel, false);

	QDomElement editorGroup = metamodel.createElement("group");
	editorGroup.setAttribute("name", diagramDisplayedName);

	foreach (QString const &elemName, elementNames) {
		QDomElement el = metamodel.createElement("element");
		el.setAttribute("name", elemName);
		editorGroup.appendChild(el);
	}
	elements.firstChild().appendChild(editorGroup);

	QDomNodeList const palette = elements.childNodes();
	mMetamodelGeneratorSupport->insertElementsInDiagramSublevel(
			metamodel, "diagram", palette);
}

void VisualInterpreterPlugin::removeDirectory(QString const &dirName)
{
	QDir const dir(dirName);

	foreach (QFileInfo info, dir.entryInfoList(QDir::Hidden
			| QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files, QDir::DirsFirst))
	{
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
	mVisualInterpreterUnit->loadSemantics();
}

void VisualInterpreterPlugin::interpret()
{
	mVisualInterpreterUnit->interpret();
}

void VisualInterpreterPlugin::stopInterpretation()
{
	mVisualInterpreterUnit->stopInterpretation();
}

void VisualInterpreterPlugin::showWatchList()
{
	if (mWatchListWindow) {
		mWatchListWindow->close();
	}

	mWatchListWindow = new WatchListWindow(mVisualInterpreterUnit->ruleParser());
	mWatchListWindow->show();
}
