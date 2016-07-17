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

#include "refactoringPlugin.h"

#include <QtCore/QProcess>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <qrutils/xmlUtils.h>
#include <qrutils/outFile.h>
#include <qrkernel/settingsManager.h>
#include "refactoringWindow.h"

using namespace qReal;
using namespace refactoring;
using namespace utils;

RefactoringPlugin::RefactoringPlugin()
	: mPreferencesPage(new RefactoringPreferencesPage())
	, mRefactoringWindow(nullptr)
{
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
			configurator.mainWindowInterpretersInterface().errorReporter()
			, &configurator.mainWindowInterpretersInterface());
	mRepoControlIFace = &configurator.repoControlInterface();
	mMainWindowIFace = &configurator.mainWindowInterpretersInterface();
	mQRealSourceFilesPath = SettingsManager::value("qrealSourcesLocation", "").toString();
	mQRealSourceFilesPath = SettingsManager::value("qrealSourcesLocation").toString();
	mPathToRefactoringExamples = mQRealSourceFilesPath + "/plugins/refactoring/refactoringExamples/";

	mRefactoringWindow = new RefactoringWindow(mMainWindowIFace->windowWidget());
	connect(mRefactoringWindow, SIGNAL(rejected()), this, SLOT(discardRefactoring()));

	mRefactoringRepoApi = new qrRepo::RepoApi(mQRealSourceFilesPath + "/plugins/refactoring/refactoringExamples", true);
	mRefactoringFinder = new RefactoringFinder(configurator.logicalModelApi()
			, configurator.graphicalModelApi()
			, configurator.mainWindowInterpretersInterface()
			, mRefactoringRepoApi);

	connect(mRefactoringWindow, SIGNAL(findButtonClicked(QString)), this, SLOT(findRefactoring(QString)));
	connect(mRefactoringWindow, SIGNAL(findNextButtonClicked()), this, SLOT(findNextRefactoring()));
	connect(mRefactoringWindow, SIGNAL(discardButtonClicked()), this, SLOT(discardRefactoring()));
	connect(mRefactoringWindow, SIGNAL(applyButtonClicked()), this, SLOT(applyRefactoring()));

	mRefactoringApplier = new RefactoringApplier(configurator.logicalModelApi()
			, configurator.graphicalModelApi()
			, configurator.mainWindowInterpretersInterface()
			, mRefactoringRepoApi
			, &mCurrentMatch);
}

QPair<QString, gui::PreferencesPage *> RefactoringPlugin::preferencesPage()
{
	return qMakePair(tr("Refactorings"), static_cast<gui::PreferencesPage *>(mPreferencesPage));
}

QList<qReal::ActionInfo> RefactoringPlugin::actions()
{
	mRefactoringMenu = new QMenu(tr("Refactoring"));
	ActionInfo refactoringMenuInfo(mRefactoringMenu, "tools");

	mGenerateAndLoadRefactoringEditorAction = new QAction(tr("Generate and load refactoring editor"), nullptr);
	connect(mGenerateAndLoadRefactoringEditorAction, SIGNAL(triggered()), this, SLOT(generateRefactoringMetamodel()));
	mRefactoringMenu->addAction(mGenerateAndLoadRefactoringEditorAction);

	mOpenRefactoringWindowAction = new QAction(tr("Open Refactoring Window"), nullptr);
	connect(mOpenRefactoringWindowAction, SIGNAL(triggered()), this, SLOT(openRefactoringWindow()));
	mRefactoringMenu->addAction(mOpenRefactoringWindowAction);

	mSaveRefactoringAction = new QAction(tr("Save Refactoring"), nullptr);
	connect(mSaveRefactoringAction, SIGNAL(triggered()), this, SLOT(saveRefactoring()));
	mRefactoringMenu->addAction(mSaveRefactoringAction);

	mCreateRefactoringAction = new QAction(tr("Create Refactoring"), nullptr);
	connect(mCreateRefactoringAction, SIGNAL(triggered()), this, SLOT(createRefactoring()));
	mRefactoringMenu->addAction(mCreateRefactoringAction);

	mPlaceMenu = new QMenu(tr("Automatically arrange elements"));

	mPlaceTBAction = new QAction(tr("Top-Bottom"), nullptr);
	connect(mPlaceTBAction, SIGNAL(triggered()), this, SLOT(arrangeElementsTB()));
	mPlaceMenu->addAction(mPlaceTBAction);

	mPlaceLRAction = new QAction(tr("Left-Right"), nullptr);
	connect(mPlaceLRAction, SIGNAL(triggered()), this, SLOT(arrangeElementsLR()));
	mPlaceMenu->addAction(mPlaceLRAction);

	mPlaceBTAction = new QAction(tr("Bottom-Top"), nullptr);
	connect(mPlaceBTAction, SIGNAL(triggered()), this, SLOT(arrangeElementsBT()));
	mPlaceMenu->addAction(mPlaceBTAction);

	mPlaceRLAction = new QAction(tr("Right-Left"), nullptr);
	connect(mPlaceRLAction, SIGNAL(triggered()), this, SLOT(arrangeElementsRL()));
	mPlaceMenu->addAction(mPlaceRLAction);

	mRefactoringMenu->addMenu(mPlaceMenu);

	mActionInfos << refactoringMenuInfo;

	return mActionInfos;
}

void RefactoringPlugin::generateRefactoringMetamodel()
{
	QDir currentDir(".");

	QString editorMetamodelFilePath =
			QFileDialog::getOpenFileName(
					nullptr
					, tr("Select xml file to load refactoring metamodel")
					, currentDir.absolutePath()
					, "XML files (*.xml)"
			);
	if (editorMetamodelFilePath == "" || mQRealSourceFilesPath == "") {
		return;
	}

	QDomDocument metamodel = mMetamodelGeneratorSupport->loadMetamodelFromFile(editorMetamodelFilePath);
	QDomElement diagram = mMetamodelGeneratorSupport->diagramElement(metamodel);
	QDomElement graphics = metamodel.elementsByTagName("graphicTypes").at(0).toElement();
	QString const diagramName = diagram.attribute("name").replace(" ", "_");
	QString const displayedName = diagram.attribute("displayedName").isEmpty()
			? diagramName
			: diagram.attribute("displayedName");

	diagram.setAttribute("displayedName", displayedName + " Refactorings");
	insertRefactoringID(metamodel, metamodel.elementsByTagName("node"), true);
	insertRefactoringID(metamodel, metamodel.elementsByTagName("edge"), false);
	addPalette(metamodel, diagram, metamodelPaletteGroup(metamodel,
			metamodel.elementsByTagName("node"), metamodel.elementsByTagName("edge")));

	addRefactoringLanguageElements(diagramName, metamodel, graphics
			, mQRealSourceFilesPath + "/plugins/refactoring/editor/refactoringEditor.xml");
	mEditorElementNames.clear();
	QString sourceEditorName = editorMetamodelFilePath.split("/", QString::SkipEmptyParts).last();
	sourceEditorName.chop(4);
	qDebug() << sourceEditorName;
	QString metamodelName = sourceEditorName + "RefactoringsMetamodel";
	QString relativeEditorPath = sourceEditorName + "RefactoringsEditor";
	QString editorPath = mQRealSourceFilesPath + "/plugins/" + relativeEditorPath;

	mMetamodelGeneratorSupport->generateProFile(metamodel
			, editorMetamodelFilePath, mQRealSourceFilesPath, metamodelName
			, editorPath, relativeEditorPath);

	mMetamodelGeneratorSupport->saveMetamodelInFile(metamodel, editorPath + "/" + metamodelName + ".xml");

	mMetamodelGeneratorSupport->loadPlugin(editorPath, metamodelName
			, SettingsManager::value("pathToQmake").toString()
			, SettingsManager::value("pathToMake").toString()
			, SettingsManager::value("pluginExtension").toString()
			, SettingsManager::value("prefix").toString()
	);
}

void RefactoringPlugin::insertRefactoringID(QDomDocument metamodel, QDomNodeList const &list, bool isNode)
{
	for (int i = 0; i < list.size(); ++i) {
		QDomElement element = list.at(i).toElement();
		mEditorElementNames.append(element.attribute("name", ""));

		QDomElement graphics = element.elementsByTagName("graphics").at(0).toElement();
		QDomNodeList labels = graphics.elementsByTagName("labels");
		QString const x = graphics.elementsByTagName("picture").at(0).toElement().attribute("sizex");

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
void RefactoringPlugin::addRefactoringLanguageElements(QString const &diagramName
		, QDomDocument metamodel, QDomElement &graphics, QString const &pathToRefactoringMetamodel)
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
	mRefactoringWindow->updateRefactorings(mPathToRefactoringExamples);
	mRefactoringWindow->show();
}

void RefactoringPlugin::saveRefactoring()
{
	IdList const childrenIDs = mLogicalModelApi->children(Id::rootId());
	QHash<QString, IdList> diagramIds;
	foreach (Id const &childId, childrenIDs) {
		if (childId.element() == "RefactoringDiagramNode" && childId == mMainWindowIFace->activeDiagram()) {
			QString elementName = mGraphicalModelApi->name(childId).replace(" ", "")
					.replace("(", "").replace(")", "");
			if (!elementName.isEmpty()) {
				QString fileName = mPathToRefactoringExamples + elementName;
				IdList list;
				list << childId;
				diagramIds.insert(fileName + ".qrs", list);
				mMainWindowIFace->saveDiagramAsAPictureToFile(fileName + ".png");
			}
		}
	}
	mRepoControlIFace->saveDiagramsById(diagramIds);
	QMessageBox::information(nullptr, tr("Information"), tr("Saved successfully"), tr("Ok"));
}

QDomElement RefactoringPlugin::createPaletteElement(QString const &elementType
		, QDomDocument metamodel, QString const &name)
{
	QDomElement element = metamodel.createElement(elementType);
	element.setAttribute("name", name);
	return element;
}

QDomElement RefactoringPlugin::metamodelPaletteGroup(QDomDocument metamodel
		, QDomNodeList const &nodeList, QDomNodeList const &edgeList)
{
	QDomElement metamodelGroup = createPaletteElement("group", metamodel, "Source Metamodel Elements");
	addElementsToMetamodelGroup(metamodel, nodeList, metamodelGroup);
	addElementsToMetamodelGroup(metamodel, edgeList, metamodelGroup);
	return metamodelGroup;
}

void RefactoringPlugin::addElementsToMetamodelGroup(QDomDocument metamodel
		, QDomNodeList const &list, QDomElement &metamodelGroup)
{
	for (int i = 0; i < list.size(); ++i) {
		QDomElement element = list.at(i).toElement();
		QString name = nameForPaletteGroup(element.attribute("name", ""));
		QDomElement paletteElement = createPaletteElement("element", metamodel, name);
		metamodelGroup.appendChild(paletteElement);
	}
}

QString const RefactoringPlugin::nameForPaletteGroup(QString const &name)
{
	QString nameForPaletteGroup = name;
	if (nameForPaletteGroup.length() > 0) {
		if (nameForPaletteGroup.length() > 1) {
			nameForPaletteGroup = nameForPaletteGroup.at(0).toUpper() + nameForPaletteGroup.mid(1);
		}
		else {
			nameForPaletteGroup = nameForPaletteGroup.at(0).toUpper();
		}
	}
	return nameForPaletteGroup;
}

void RefactoringPlugin::addPalette(QDomDocument metamodel, QDomElement diagram
		, QDomElement const &metamodelPaletteGroup)
{
	QStringList patternGroupNamesList;
	patternGroupNamesList << "RefactoringDiagramNode"
			<< "FromBeforeToAter"
			<< "AfterBlock"
			<< "BeforeBlock";
	QStringList basicGroupNamesList;
	basicGroupNamesList << "Element"
			<< "Link"
			<< "SelectedSegment";
	QDomElement palette = metamodel.createElement("palette");
	addPaletteGroup(metamodel, palette, "Refactoring Rule Elements", patternGroupNamesList);
	addPaletteGroup(metamodel, palette, "Basic Elements", basicGroupNamesList);
	palette.appendChild(metamodelPaletteGroup);
	diagram.appendChild(palette);
}

void RefactoringPlugin::addPaletteGroup(QDomDocument metamodel
		, QDomElement palette, const QString &groupName, const QStringList &elementNameList)
{
	QDomElement group = createPaletteElement("group", metamodel, groupName);
	foreach (QString const &elementName, elementNameList) {
		QDomElement element = createPaletteElement("element", metamodel, elementName);
		group.appendChild(element);
	}
	palette.appendChild(group);
}

void RefactoringPlugin::arrangeElements(const QString &algorithm)
{
	if (SettingsManager::value("pathToDot").toString().isEmpty()) {
		mErrorReporter->addCritical(tr("Path to dot is not specified"));
		return;
	}

	mMainWindowIFace->arrangeElementsByDotRunner(algorithm, mQRealSourceFilesPath + "/bin");
}

void RefactoringPlugin::arrangeElementsBT()
{
	arrangeElements("TB"); // top-bottom
}

void RefactoringPlugin::arrangeElementsLR()
{
	arrangeElements("LR");
}
void RefactoringPlugin::arrangeElementsTB()
{
	arrangeElements("BT"); // bottom-top
}

void RefactoringPlugin::arrangeElementsRL()
{
	arrangeElements("RL");
}

void RefactoringPlugin::findRefactoring(const QString &refactoringName)
{
	QString const refactoringPath = mPathToRefactoringExamples + refactoringName + ".qrs";
	mRefactoringRepoApi->open(refactoringPath);
	if (mRefactoringFinder->refactoringRuleContainsSelectedSegment()) {
		mSelectedElementsOnActiveDiagram = mMainWindowIFace->selectedElementsOnActiveDiagram();
		foreach (Id const &selectedElement, mSelectedElementsOnActiveDiagram) {
			QColor const color = QColor(SettingsManager::value("refactoringColor", "cyan").toString());
			bool isExclusive = false;
			mMainWindowIFace->highlight(selectedElement, isExclusive, color);
		}
	}
	else if (mRefactoringFinder->findMatch()) {
		mMatches = mRefactoringFinder->matches();
		if (mMatches.isEmpty()) {
			mMainWindowIFace->errorReporter()->addInformation(tr("No match"));
			mRefactoringWindow->discard();
			return;
		}
		mCurrentMatch = mMatches.takeFirst();
		foreach (Id const &id, mCurrentMatch.keys()) {
			Id valueId = mCurrentMatch.value(id);
			if (mLogicalModelApi->isLogicalId(valueId)) {
				mMainWindowIFace->errorReporter()->addInformation(tr("No graphical elements match"));
				mRefactoringWindow->discard();
				return;
			}
		}
		foreach (Id const &id, mCurrentMatch.keys()) {
			Id valueId = mCurrentMatch.value(id);
			QColor const color = QColor(SettingsManager::value("refactoringColor", "cyan").toString());
			bool isExclusive = false;
			mMainWindowIFace->highlight(valueId, isExclusive, color);
		}
	}
	else {
		mMainWindowIFace->errorReporter()->addInformation(tr("No match"));
		mRefactoringWindow->discard();
		return;
	}
	mRefactoringWindow->activateRestButtons();
}

void RefactoringPlugin::findNextRefactoring()
{
	mMainWindowIFace->dehighlight();
	if (mMatches.isEmpty()) {
		mMainWindowIFace->errorReporter()->addInformation(tr("No next match"));
		mRefactoringWindow->discard();
	} else {
		mCurrentMatch = mMatches.takeFirst();
		foreach (Id const &id, mCurrentMatch.keys()) {
			QColor const color = QColor(SettingsManager::value("refactoringColor", "cyan").toString());
			bool isExclusive = false;
			mMainWindowIFace->highlight(mCurrentMatch.value(id), isExclusive, color);
		}
	}
}

void RefactoringPlugin::discardRefactoring()
{
	mMainWindowIFace->dehighlight();
	mMatches.clear();
	mCurrentMatch.clear();
	mSelectedElementsOnActiveDiagram.clear();
	mRefactoringWindow->discard();
}

void RefactoringPlugin::createRefactoring()
{
	if (mMainWindowIFace->pluginLoaded("RefactoringEditor")) {
		Id diagramId = Id("RefactoringEditor", "RefactoringDiagram"
				, "RefactoringDiagramNode", QUuid::createUuid().toString());
		mGraphicalModelApi->createElement(Id::rootId(), diagramId, false, "NewRefactoringRule", QPointF());

		QHash<QString, QPointF> elementTypesAndPositions;
		elementTypesAndPositions.insert("BeforeBlock", QPointF(300, 140));
		elementTypesAndPositions.insert("AfterBlock", QPointF(700, 140));
		elementTypesAndPositions.insert("FromBeforeToAter", QPointF(540, 200));

		foreach(QString const name, elementTypesAndPositions.keys()) {
			Id elementId = Id("RefactoringEditor", "RefactoringDiagram", name, QUuid::createUuid().toString());
			mGraphicalModelApi->createElement(diagramId, elementId, false, name
					, elementTypesAndPositions[name]);
		}
		mMainWindowIFace->activateItemOrDiagram(diagramId);
	}
}

void RefactoringPlugin::applyRefactoring()
{
	if (mSelectedElementsOnActiveDiagram.isEmpty()) {
		mRefactoringApplier->applyRefactoringRule();
	} else {
		makeSubprogramHARDCODE();
	}
	discardRefactoring();
	mMainWindowIFace->updateActiveDiagram();
}

void RefactoringPlugin::makeSubprogramHARDCODE() // FIXME
{
	removeUnnecessaryLinksFromSelected();
	Id const activeDiagramId = mMainWindowIFace->activeDiagram();
	Id const newDiagramId = Id(activeDiagramId.editor(), activeDiagramId.diagram()
			, activeDiagramId.element(), QUuid::createUuid().toString());
	Id const subprogramElementInRuleId = mRefactoringApplier->subprogramElementId();
	if (subprogramElementInRuleId == Id::rootId()) {
		return;
	}
	QString const newDiagramName = mGraphicalModelApi->name(activeDiagramId)
			+ "_Subprogram_" + mRefactoringRepoApi->name(subprogramElementInRuleId);
	mGraphicalModelApi->createElement(Id::rootId(), newDiagramId, false, newDiagramName, QPointF());
	foreach (Id const &id, mSelectedElementsOnActiveDiagram) {
		mGraphicalModelApi->changeParent(id, newDiagramId, mGraphicalModelApi->position(id));
	}

	QList<QPair<Id, QPair<Id, bool> > > const outsideLinks = findOutsideSelectionLinks();
	Id subprogramId = Id(subprogramElementInRuleId.editor()
			, subprogramElementInRuleId.diagram()
			, subprogramElementInRuleId.element()
			, QUuid::createUuid().toString());

	mGraphicalModelApi->createElement(activeDiagramId, subprogramId, false
			, mRefactoringRepoApi->name(subprogramElementInRuleId)
			, mGraphicalModelApi->position(outsideLinks.first().second.first));

	for (int i = 0; i <outsideLinks.size(); ++i) {
		if (outsideLinks.at(i).second.second) {
			mGraphicalModelApi->setTo(outsideLinks.at(i).first, subprogramId);
		} else {
			mGraphicalModelApi->setFrom(outsideLinks.at(i).first, subprogramId);
		}
	}

	mLogicalModelApi->addExplosion(mGraphicalModelApi->logicalId(subprogramId)
			, mGraphicalModelApi->logicalId(newDiagramId));
}

QList<QPair<Id, QPair<Id, bool> > > RefactoringPlugin::findOutsideSelectionLinks()
{
	QList<QPair<Id, QPair<Id, bool> > > result;
	foreach (Id const &id, mSelectedElementsOnActiveDiagram) {
		IdList const currentIdList = mGraphicalModelApi->graphicalRepoApi().links(id);
		foreach (Id const currentId, currentIdList) {
			Id const toId = mGraphicalModelApi->to(currentId);
			Id const fromId = mGraphicalModelApi->from(currentId);
			if (mSelectedElementsOnActiveDiagram.contains(toId)
					&& !(mSelectedElementsOnActiveDiagram.contains(fromId)))
			{
				result.append(QPair<Id, QPair<Id, bool> > (currentId, QPair<Id, bool>(toId, true)));
			}
			if (mSelectedElementsOnActiveDiagram.contains(fromId)
					&& !(mSelectedElementsOnActiveDiagram.contains(toId)))
			{
				result.append(QPair<Id, QPair<Id, bool> > (currentId, QPair<Id, bool>(fromId, false)));
			}
		}
	}
	return result;
}

void RefactoringPlugin::removeUnnecessaryLinksFromSelected()
{
	foreach (Id const &id, mSelectedElementsOnActiveDiagram) {
		if (mGraphicalModelApi->to(id) == Id::rootId()
				&& mGraphicalModelApi->from(id) == Id::rootId()) {
			continue;
		}
		Id const toId = mGraphicalModelApi->to(id);
		Id const fromId = mGraphicalModelApi->from(id);
		if ((mSelectedElementsOnActiveDiagram.contains(toId)
				&& !(mSelectedElementsOnActiveDiagram.contains(fromId))) ||
				(mSelectedElementsOnActiveDiagram.contains(fromId)
				&& !(mSelectedElementsOnActiveDiagram.contains(toId))))
		{
			mSelectedElementsOnActiveDiagram.removeAll(id);
		}
	}
}
