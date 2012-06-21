#include "editorGenerator.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

#include <QtGui/QMessageBox>

#include "../../../qrkernel/roles.h"

#include "../../../qrutils/outFile.h"
#include "../../../qrutils/nameNormalizer.h"

#include "../../../qrkernel/settingsManager.h"

using namespace qReal;
using namespace metaEditor;
using namespace utils;

EditorGenerator::EditorGenerator(qrRepo::LogicalRepoApi const &api, ErrorReporterInterface &errorReporter)
		: mApi(api)
		, mErrorReporter(errorReporter)
{
}

QHash<Id, QPair<QString,QString> > EditorGenerator::getMetamodelList()
{
	Id repoId = Id::rootId();

	IdList const metamodels = mApi.children(repoId);
	QHash<Id, QPair<QString,QString> > metamodelList;

	foreach (Id const key, metamodels) {
		QString const objectType = key.element();
		if (objectType == "MetamodelDiagram" && mApi.isLogicalElement(key)) {
			// Now the user must specify the full path to the directory and the relative path to source files of QReal
			QString const directoryName = mApi.stringProperty(key, "name of the directory");
			QString const pathToQRealRoot = mApi.stringProperty(key, "relative path to QReal Source Files");
			if ((!directoryName.isEmpty()) && (!pathToQRealRoot.isEmpty())) {
				QPair<QString, QString> savingData;
				savingData.first = directoryName;
				savingData.second = pathToQRealRoot;
				metamodelList.insert(key, savingData);
			}
			else {
				mErrorReporter.addError("no name of the directory or relative path to QReal Source Files", key);
			}
		}
	}
	return metamodelList;
}

QString EditorGenerator::generateEditor(Id const &metamodelId, QString const &pathToFile, QString const &pathToQRealSource)
{
	QString const editorPath = calculateEditorPath(pathToFile, pathToQRealSource);

	QDomElement metamodel = mDocument.createElement("metamodel");
	metamodel.setAttribute("xmlns", "http://schema.real.com/schema/");
	mDocument.appendChild(metamodel);

	QString const includeFile = mApi.stringProperty(metamodelId, "include");
	QStringList const includeList = includeFile.split(", ", QString::SkipEmptyParts);
	QString includeProList;
	foreach (QString const &name, includeList) {
		includeProList += " " + name;
		QDomElement include = mDocument.createElement("include");
		QDomText value = mDocument.createTextNode(name);
		include.appendChild(value);
		metamodel.appendChild(include);
	}

	QDomElement nameSpace = mDocument.createElement("namespace");
	QDomText nameSpaceName = mDocument.createTextNode("UML 2.0");
	nameSpace.appendChild(nameSpaceName);
	metamodel.appendChild(nameSpace);

	createDiagrams(metamodel, metamodelId);

	QString const fileBaseName = NameNormalizer::normalize(mApi.name(metamodelId), false);

	try {
		OutFile outpro(pathToFile + "/" + fileBaseName + ".pro");
		outpro() << QString("QREAL_XML = %1\n").arg(fileBaseName + ".xml");
		if (includeProList != "") {
			outpro() << QString("QREAL_XML_DEPENDS = %1\n").arg(includeProList);
		}
		outpro() << QString ("QREAL_EDITOR_PATH = %1\n").arg(editorPath);
		outpro() << QString ("ROOT = %1\n").arg(pathToQRealSource);
		outpro() << "\n";
		outpro() << QString("include (%1)").arg(pathToQRealSource + "/plugins/editorsSdk/editorsCommon.pri");
	}
	catch (char* e) {
		mErrorReporter.addCritical(QObject::tr("incorrect file name"));
	}

	OutFile outxml(pathToFile + "/" + fileBaseName + ".xml");
	QDomNode const header = mDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
	mDocument.insertBefore(header, mDocument.firstChild());
	mDocument.save(outxml(), 4);
	mDocument.clear();

	copyImages(pathToFile);

	return fileBaseName;
}

QString EditorGenerator::calculateEditorPath(QString const &pathToFile, QString const &pathToQRealSource)
{
	QFileInfo const pluginDir(pathToFile);
	QFileInfo const sourcesDir(pathToFile + "/" + pathToQRealSource);
	QFileInfo const qRealPluginsDir(sourcesDir.absoluteFilePath() + "/plugins/");

	int const levels = qRealPluginsDir.absoluteFilePath().split("/", QString::SkipEmptyParts).count();
	QString result;
	for (int i = 0; i < levels; ++i) {
		result += "/..";
	}

	if (pluginDir.absoluteFilePath().count() > 2 && pluginDir.absoluteFilePath()[1] == ':') {
		// Remove drive letter on Windows.
		result += pluginDir.absoluteFilePath().remove(0, 2);
	}

	return result;
}

void EditorGenerator::copyImages(QString const &pathToFile)
{
	QString const workingDirName = SettingsManager::value("workingDir", "./save").toString();
	QDir sourceDir(workingDirName);
	sourceDir.cd("images");
	if (!sourceDir.exists()) {
		return;
	}

	QFileInfo const destDirInfo(pathToFile);
	QDir destDir = destDirInfo.dir();
	destDir.mkdir("images");
	destDir.cd("images");

	foreach (QString const &file, sourceDir.entryList(QDir::Files)) {
		QFile::copy(sourceDir.absolutePath() + "/" + file, destDir.absolutePath() + "/" + file);
	}
}

void EditorGenerator::createDiagrams(QDomElement &parent, Id const &id)
{
	IdList const rootElements = mApi.children(id);
	foreach (Id const typeElement, rootElements) {
		QString const objectType = typeElement.element();
		if (objectType == "MetaEditorDiagramNode") {
			QDomElement diagram = mDocument.createElement("diagram");
			ensureCorrectness(typeElement, diagram, "name", mApi.name(typeElement));
			ensureCorrectness(typeElement, diagram, "displayedName", mApi.stringProperty(typeElement, "displayedName"));
			ensureCorrectness(typeElement, diagram, "nodeName", mApi.stringProperty(typeElement, "nodeName"));
			parent.appendChild(diagram);

			serializeObjects(diagram, typeElement);
			mElements.clear();
		}
		else if (objectType == "Listener") {
			QDomElement listener = mDocument.createElement("listener");
			ensureCorrectness(typeElement, listener, "class", mApi.stringProperty(typeElement, "class"));
			ensureCorrectness(typeElement, listener, "file", mApi.stringProperty(typeElement, "file"));
			parent.appendChild(listener);
		}
	}
}

void EditorGenerator::serializeObjects(QDomElement &parent, Id const &idParent)
{
	IdList const childElems = mApi.children(idParent);
	mElements = childElems;
	mDiagramName = mApi.name(idParent);

	QDomElement tagNonGraphic = mDocument.createElement("nonGraphicTypes");

	foreach (Id const &id, childElems) {
		if (idParent != Id::rootId()) {
			QString const objectType = id.element();
			if (objectType == "MetaEntityEnum") {
				createEnum(tagNonGraphic, id);
			}
		}
	}

	if (!tagNonGraphic.childNodes().isEmpty()) {
		parent.appendChild(tagNonGraphic);
	}

	QDomElement tagGraphic = mDocument.createElement("graphicTypes");
	parent.appendChild(tagGraphic);

	foreach (Id const &id, childElems) {
		if (idParent != Id::rootId()) {
			QString const objectType = id.element();
			if (objectType == "MetaEntityImport") {
				createImport(tagGraphic, id);
			} else if (objectType == "MetaEntityNode") {
				createNode(tagGraphic, id);
			} else if (objectType == "MetaEntityEdge") {
				createEdge(tagGraphic, id);
			}
		}
	}
}

void EditorGenerator::createImport(QDomElement &parent, const Id &id)
{
	QDomElement import = mDocument.createElement("import");
	if ((mApi.stringProperty(id, "importedFrom") != "") && (mApi.name(id) != "")) {
		ensureCorrectness(id, import, "name", mApi.stringProperty(id, "importedFrom") + "::" + mApi.name(id));
	} else {
		mErrorReporter.addWarning(QString ("not filled name/importedFrom"), id);
		import.setAttribute("name", "");
	}
	ensureCorrectness(id, import, "displayedName", mApi.stringProperty(id, "displayedName"));
	ensureCorrectness(id, import, "as", mApi.stringProperty(id, "as"));
	parent.appendChild(import);
}

void EditorGenerator::createNode(QDomElement &parent, Id const &id)
{
	QDomElement node = mDocument.createElement("node");
	ensureCorrectness(id, node, "name", mApi.name(id));
	ensureCorrectness(id, node, "displayedName", mApi.stringProperty(id, "displayedName"));
	if (!mApi.stringProperty(id, "path").isEmpty()) {
		node.setAttribute("path", mApi.stringProperty(id, "path"));
	}
	if (!mApi.stringProperty(id, "description").isEmpty()) {
		node.setAttribute("description", mApi.stringProperty(id, "description"));
	}
	parent.appendChild(node);

	QDomDocument graphics;
	graphics.setContent(mApi.stringProperty(id, "shape"));
	node.appendChild(graphics);

	if (mApi.stringProperty(id, "isResizeable") == "false") {
		QDomElement nonResizeable = mDocument.createElement("nonResizeable");
		graphics.firstChild().appendChild(nonResizeable);
	}

	QDomElement logic = mDocument.createElement("logic");
	node.appendChild(logic);

	setContainer(logic, id);
	setUsages(logic, id);
	setConnections(logic, id);
	setProperties(logic, id);
	setPin(logic, id);
	setAction(logic, id);
	setGeneralization(logic, id);
	setContextMenuFields(logic, id);
}

void EditorGenerator::createEdge(QDomElement &parent, Id const &id)
{
	QDomElement edge = mDocument.createElement("edge");
	ensureCorrectness(id, edge, "name", mApi.name(id));
	ensureCorrectness(id, edge, "displayedName", mApi.stringProperty(id, "displayedName"));
	parent.appendChild(edge);

	if (mApi.stringProperty(id, "lineType") != "") {
		QDomElement graphics = mDocument.createElement("graphics");
		edge.appendChild(graphics);
		QDomElement lineType = mDocument.createElement("lineType");
		ensureCorrectness(id, lineType, "type", mApi.stringProperty(id, "lineType"));
		graphics.appendChild(lineType);

		QString const labelText = mApi.stringProperty(id, "labelText");
		if (!labelText.isEmpty()) {
			QDomElement labels = mDocument.createElement("labels");
			graphics.appendChild(labels);

			QDomElement label = mDocument.createElement("label");
			labels.appendChild(label);

			QString const labelType = mApi.stringProperty(id, "labelType");
			if (labelType == "Static text") {
				label.setAttribute("text", labelText);
			} else if (labelType == "Dynamic text") {
				label.setAttribute("textBinded", labelText);
			} else {
				mErrorReporter.addWarning("Incorrect label type", id);
			}
		}
	}

	QDomElement logic = mDocument.createElement("logic");
	edge.appendChild(logic);

	setAssociations(logic, id);
	setPossibleEdges(logic, id);
	setProperties(logic, id);
	setGeneralization(logic, id);
}

void EditorGenerator::createEnum(QDomElement &parent, Id const &id)
{
	QDomElement enumElement = mDocument.createElement("enum");
	ensureCorrectness(id, enumElement, "name", mApi.name(id));
	ensureCorrectness(id, enumElement, "displayedName", mApi.stringProperty(id, "displayedName"));
	parent.appendChild(enumElement);

	setValues(enumElement, id);
}

void EditorGenerator::setGeneralization(QDomElement &parent, const Id &id)
{
	QDomElement generalizations = mDocument.createElement("generalizations");

	IdList const inLinks = mApi.incomingLinks(id);

	foreach (Id const &inLink, inLinks) {
		if (inLink.element() == "Inheritance") {
			Id const parentId = mApi.from(inLink);
			if ((parentId.element() == "MetaEntityImport")
					|| (parentId.element() == "MetaEntityNode")
					|| (parentId.element() == "MetaEntityEdge"))
			{
				QDomElement generalization = mDocument.createElement("parent");
				ensureCorrectness(parentId, generalization, "parentName", mApi.stringProperty(parentId, "name"));
				generalizations.appendChild(generalization);
			}
		}
	}
	if (!generalizations.childNodes().isEmpty()) {
		parent.appendChild(generalizations);
	}
}

void EditorGenerator::setProperties(QDomElement &parent, Id const &id)
{
	QDomElement tagProperties = mDocument.createElement("properties");
	IdList const childElems = mApi.children(id);

	foreach (Id const &idChild, childElems) {
		if (idChild != Id::rootId()) {
			QString const objectType = idChild.element();
			if (objectType == "MetaEntity_Attribute") {
				QDomElement property = mDocument.createElement("property");
				ensureCorrectness(idChild, property, "type", mApi.stringProperty(idChild, "attributeType"));
				ensureCorrectness(idChild, property, "name", mApi.name(idChild));
				ensureCorrectness(idChild, property, "displayedName", mApi.stringProperty(idChild, "displayedName"));
				if (mApi.stringProperty(idChild, "defaultValue") != "") {
					QDomElement defaultTag = mDocument.createElement("default");
					QDomText value = mDocument.createTextNode(mApi.stringProperty(idChild, "defaultValue"));
					defaultTag.appendChild(value);
					property.appendChild(defaultTag);
				}
				tagProperties.appendChild(property);
			}
		}
	}

	if (!tagProperties.childNodes().isEmpty()) {
		parent.appendChild(tagProperties);
	}
}

void EditorGenerator::setContextMenuFields(QDomElement &parent, const Id &id)
{
	QDomElement fields = mDocument.createElement("bonusContextMenuFields");
	IdList const childElems = mApi.children(id);

	foreach (Id const idChild, childElems)
		if (idChild != Id::rootId()) {
			QString const objectType = idChild.element();
			if (objectType == "MetaEntityContextMenuField"){
				QDomElement field = mDocument.createElement("field");
				ensureCorrectness(idChild, field, "name", mApi.name(idChild));
				fields.appendChild(field);
			}
		}

	if (!fields.childNodes().isEmpty())
		parent.appendChild(fields);
}

void EditorGenerator::setValues(QDomElement &parent, const Id &id)
{
	IdList childElems = mApi.children(id);

	foreach (Id const idChild, childElems) {
		if (idChild != Id::rootId()) {
			QDomElement valueTag = mDocument.createElement("value");
			QDomText value = mDocument.createTextNode(mApi.stringProperty(idChild, "valueName"));
			valueTag.appendChild(value);
			parent.appendChild(valueTag);
		}
	}
}

void EditorGenerator::setAssociations(QDomElement &parent, const Id &id)
{
	IdList const childElems = mApi.children(id);

	foreach (Id const idChild, childElems) {
		QString const objectType = idChild.editor();
		if (objectType == "MetaEntityAssociation") {
			QDomElement associationTag = mDocument.createElement("associations");
			ensureCorrectness(idChild, associationTag, "beginType", mApi.stringProperty(idChild, "beginType"));
			ensureCorrectness(idChild, associationTag, "endType", mApi.stringProperty(idChild, "endType"));
			parent.appendChild(associationTag);

			QDomElement association = mDocument.createElement("association");
			ensureCorrectness(idChild, association, "beginName", mApi.stringProperty(idChild, "beginName"));
			ensureCorrectness(idChild, association, "endName", mApi.stringProperty(idChild, "endName"));
			associationTag.appendChild(association);
		}
	}
}

void EditorGenerator::setUsages(QDomElement &parent, const Id &id)
{
	newSetConnections(parent, id, "usages", "usage", "MetaEntityUsage");
}

void EditorGenerator::setConnections(QDomElement &parent, const Id &id)
{
	newSetConnections(parent, id, "connections", "connection", "MetaEntityConnection");
}

void EditorGenerator::newSetConnections(QDomElement &parent, const Id &id,
		QString const &commonTagName, QString const &internalTagName, QString const &typeName)
{
	IdList const childElems = mApi.children(id);

	QDomElement connectionsTag = mDocument.createElement(commonTagName);

	foreach (Id const idChild, childElems) {
		QString const objectType = idChild.editor();
		if (objectType == typeName) {
			QDomElement connection = mDocument.createElement(internalTagName);
			ensureCorrectness(idChild, connection,"type", mApi.stringProperty(idChild, "type"));
			connectionsTag.appendChild(connection);
		}
	}

	if (!connectionsTag.childNodes().isEmpty()) {
		parent.appendChild(connectionsTag);
	}
}

void EditorGenerator::setPossibleEdges(QDomElement &parent, const Id &id)
{
	IdList const childElems = mApi.children(id);

	QDomElement possibleEdges = mDocument.createElement("possibleEdges");

	foreach (Id const idChild, childElems) {
		QString const objectType = idChild.editor();
		if (objectType == "MetaEntityPossibleEdge") {
			QDomElement possibleEdge = mDocument.createElement("possibleEdge");
			possibleEdges.appendChild(possibleEdge);
			ensureCorrectness(idChild, possibleEdge, "beginName", mApi.stringProperty(idChild, "beginName"));
			ensureCorrectness(idChild, possibleEdge, "endName", mApi.stringProperty(idChild, "endName"));
			ensureCorrectness(idChild, possibleEdge, "directed", mApi.stringProperty(idChild, "directed"));
		}
	}

	if (!possibleEdges.childNodes().isEmpty()) {
		parent.appendChild(possibleEdges);
	}
}

void EditorGenerator::setPin(QDomElement &parent, const Id &id)
{
	setStatusElement(parent, id, "pin", "isPin");
}

void EditorGenerator::setAction(QDomElement &parent, const Id &id)
{
	setStatusElement(parent, id, "action", "isAction");
}

void EditorGenerator::setStatusElement(QDomElement &parent, const Id &id, const QString &tagName, const QString &propertyName)
{
	if (mApi.stringProperty(id, propertyName) == "true") {
		QDomElement statusElement = mDocument.createElement(tagName);
		parent.appendChild(statusElement);
	}
}

void EditorGenerator::setContainer(QDomElement &parent, const Id &id)
{
	QDomElement container = mDocument.createElement("container");
	parent.appendChild(container);

	IdList inLinks = mApi.outgoingLinks(id);
	foreach (Id const inLink, inLinks) {
		if (inLink.editor() == "Container") {
			Id const elementId = mApi.to(inLink);
			QString const typeName = elementId.editor();
			if (typeName == "MetaEntityNode") {
				QDomElement contains = mDocument.createElement("contains");
				ensureCorrectness(elementId, contains, "type", mApi.name(elementId));
				container.appendChild(contains);
			} else if (typeName == "MetaEntityImport") {
				QDomElement contains = mDocument.createElement("contains");
				ensureCorrectness(elementId, contains, "type", mApi.stringProperty(elementId, "importedFrom") + "::" + mApi.name(elementId));
				container.appendChild(contains);
			}
		}
	}

	setContainerProperties(container, id);
}

void EditorGenerator::setContainerProperties(QDomElement &parent, Id const &id)
{
	IdList elements = mApi.children(id);

	foreach (Id const idChild, elements) {
		if (idChild.editor() == "MetaEntityPropertiesAsContainer") {
			QDomElement properties = mDocument.createElement("properties");
			parent.appendChild(properties);

			setSizesForContainer("forestalling", properties, idChild);
			setSizesForContainer("childrenForestalling", properties, idChild);

			setBoolValuesForContainer("sortContainer", properties, idChild);
			setBoolValuesForContainer("banChildrenMove", properties, idChild);
			setBoolValuesForContainer("minimizeToChildren", properties, idChild);
			setBoolValuesForContainer("maximizeChildren", properties, idChild);
		}
	}
}

void EditorGenerator::setSizesForContainer(QString const &propertyName, QDomElement &properties, Id const &id)
{
	if (mApi.stringProperty(id, propertyName + "Size") != "") {
		QDomElement property = mDocument.createElement(propertyName);
		properties.appendChild(property);
		property.setAttribute("size", mApi.stringProperty(id, propertyName + "Size"));
	}
}

void EditorGenerator::setBoolValuesForContainer(QString const &propertyName, QDomElement &properties, Id const &id)
{
	if (mApi.stringProperty(id, propertyName) == "true") {
		QDomElement property = mDocument.createElement(propertyName);
		properties.appendChild(property);
	}
}

void EditorGenerator::ensureCorrectness(Id const &id, QDomElement element, QString const &tagName, QString const &value)
{
	QString const tag = tagName;
	if (value.isEmpty() && tag == "displayedName") {
		return;
	} else if (value.isEmpty()) {
		mErrorReporter.addWarning(QString ("not filled %1\n").arg(tagName), id);
		element.setAttribute(tagName, "");
	} else if (tag == "name") {
		QRegExp patten;
		patten.setPattern("[A-Za-z_]+([A-Za-z_0-9 :]*)");
		if (patten.exactMatch(value)) {
			element.setAttribute(tagName, value);
		} else {
			mErrorReporter.addWarning("wrong name\n", id);
			element.setAttribute(tagName, value);
		}
	}
	else {
		element.setAttribute(tagName, value);
	}
}

