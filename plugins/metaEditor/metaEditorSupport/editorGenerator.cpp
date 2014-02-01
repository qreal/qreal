#include "editorGenerator.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

#include <QtWidgets/QMessageBox>

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

QHash<Id, QString > EditorGenerator::getMetamodelList(QString const &pathToQRealSources)
{
	IdList const metamodels = mApi.children(Id::rootId());
	QHash<Id, QString > metamodelList;

	foreach (Id const key, metamodels) {
		QString const objectType = key.element();
		if (objectType == "MetamodelDiagram" && mApi.isLogicalElement(key)) {
			// Now the user must specify the full path to the directory
			QString const directoryName = mApi.stringProperty(key, "name of the directory");
			QString pathToQRealRoot;
			if (pathToQRealSources == "") {
				pathToQRealRoot = SettingsManager::value("pathToQRealSourceFiles").toString();
			} else {
				pathToQRealRoot = pathToQRealSources;
			}
			if (!directoryName.isEmpty() && !pathToQRealRoot.isEmpty()) {
				QString savingData = directoryName;
				metamodelList.insert(key, savingData);
			} else {
				mErrorReporter.addError(
						QObject::tr("no directory to generated code or path to QReal Source Files"), key);
			}
		}
	}
	return metamodelList;
}

QPair<QString, QString> EditorGenerator::generateEditor(Id const &metamodelId
		, QString const &pathToFile
		, QString const &pathToQrealRoot
		, QString const &destDir)
{
	mErrorReporter.clear();
	mErrorReporter.clearErrors();

	QString const editorPath = calculateEditorPath(pathToFile, pathToQrealRoot);

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

	QRegExp patten;
	patten.setPattern("[A-Za-z]+([A-Za-z0-9]*)");

	if (!patten.exactMatch(fileBaseName) || fileBaseName.isEmpty()) {
		mErrorReporter.addError(QObject::tr("wrong name\n"), metamodelId);
		return QPair<QString, QString>("", "");
	}

	try {
		OutFile outpro(pathToFile + "/" + fileBaseName + ".pro");
		outpro() << QString("QREAL_XML = %1\n").arg(fileBaseName + ".xml");
		if (includeProList != "") {
			outpro() << QString("QREAL_XML_DEPENDS = %1\n").arg(includeProList);

		}
		outpro() << QString ("QREAL_EDITOR_PATH = %1\n").arg(editorPath);

		QString const relativeQRealSourcesPath = calculateRelativeQRealSourcesPath(pathToQrealRoot);

		outpro() << QString ("ROOT = %1\n").arg(relativeQRealSourcesPath);
		outpro() << "\n";

		outpro() << QString("include (%1)\n").arg(pathToQrealRoot + "/plugins/editorsSdk/editorsCommon.pri");
		if (!destDir.isEmpty()) {
			outpro() << QString("DESTDIR = %1\n").arg(destDir);
		}
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

	return QPair<QString, QString>(mApi.name(metamodelId), fileBaseName);
}

QString EditorGenerator::calculateEditorPath(QString const &pathToFile, QString const &pathToQRealSource)
{
	QFileInfo const pluginDir(pathToFile);
	QFileInfo const sourcesDir(pathToQRealSource);
	QFileInfo const qRealPluginsDir(sourcesDir.absoluteFilePath() + "/plugins/" + "editorsSdk");

	return calculateRelativePath(qRealPluginsDir.absoluteFilePath(), pluginDir.absoluteFilePath());
}

QString EditorGenerator::calculateRelativeQRealSourcesPath(QString const &pathToQRealSource)
{
	QFileInfo const sdkDir(pathToQRealSource);
	QFileInfo const pluginDir(sdkDir.absoluteFilePath() + "/plugins/" + "editorsSdk");
	QFileInfo const sourcesDir(pathToQRealSource);

	return calculateRelativePath(pluginDir.absoluteFilePath(), sourcesDir.absoluteFilePath());
}

QString EditorGenerator::newMethodForCalculatingEditorPath(QString const &pathToFile)
{
	QString result = "";

	int const levels = pathToFile.split("/", QString::SkipEmptyParts).count();
	for (int i = 0; i < levels; i++) {
		result += "/..";
	}

	if (pathToFile.count() > 2 && pathToFile[1] == ':') {
		// Remove drive letter
		result += pathToFile.mid(2);
	} else {
		result += pathToFile;
	}

	return result;
}

QString EditorGenerator::calculateRelativePath(QString const &pathOne, QString const &pathTwo)
{

	int const levels = pathOne.split("/", QString::SkipEmptyParts).count();
	QString result;
	for (int i = 0; i < levels; ++i) {
		result += "/..";
	}

	if (pathTwo.count() > 2 && pathTwo[1] == ':') {
		// Remove drive letter on Windows.
		result += pathTwo.mid(2);
	} else {
		result += pathTwo;
	}

	return result;
}

void EditorGenerator::copyImages(QString const &pathToFile)
{
	QString const workingDirName = SettingsManager::value("workingDir").toString();
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
			} else if (objectType == "MetaEntityPort") {
				createPort(tagNonGraphic, id);
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
		mErrorReporter.addWarning(QObject::tr("not filled name/importedFrom"), id);
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
	setAction(logic, id);
	setCreateChildrenFromMenu(logic, id);
	setGeneralization(logic, id);
	setContextMenuFields(logic, id);
	setExplosion(logic, id);
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

		QDomElement shapeType = mDocument.createElement("shape");
		ensureCorrectness(id, shapeType, "type", mApi.stringProperty(id, "shape"));
		graphics.appendChild(shapeType);

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
				mErrorReporter.addWarning(QObject::tr("Incorrect label type"), id);
			}
		}
	}

	QDomElement logic = mDocument.createElement("logic");
	edge.appendChild(logic);

	setAssociations(logic, id);
	setPossibleEdges(logic, id);
	setProperties(logic, id);
	setPorts(logic, id, "from");
	setPorts(logic, id, "to");
	setGeneralization(logic, id);
	setExplosion(logic, id);
}

void EditorGenerator::createEnum(QDomElement &parent, Id const &id)
{
	QDomElement enumElement = mDocument.createElement("enum");
	ensureCorrectness(id, enumElement, "name", mApi.name(id));
	ensureCorrectness(id, enumElement, "displayedName", mApi.stringProperty(id, "displayedName"));
	parent.appendChild(enumElement);

	setValues(enumElement, id);
}

void EditorGenerator::createPort(QDomElement &parent, Id const &id)
{
	QDomElement portElement = mDocument.createElement("port");
	ensureCorrectness(id, portElement, "name", mApi.name(id));
	parent.appendChild(portElement);
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

void EditorGenerator::setPorts(QDomElement &parent, Id const &id, QString const &direction)
{
	QString const propertyName = direction + "Ports";
	QDomElement portsTag = mDocument.createElement(propertyName);
	QStringList const ports = mApi.stringProperty(id, propertyName).split(',', QString::SkipEmptyParts);
	foreach (QString const &port, ports) {
		QDomElement portElem = mDocument.createElement("port");
		Id const portId = Id::loadFromString(port);
		portElem.setAttribute("type", mApi.name(portId));
		portsTag.appendChild(portElem);
	}
	parent.appendChild(portsTag);
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
		QString const objectType = idChild.element();
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

void EditorGenerator::setAction(QDomElement &parent, const Id &id)
{
	setStatusElement(parent, id, "action", "isAction");
}

void EditorGenerator::setCreateChildrenFromMenu(QDomElement &parent, const Id &id)
{
	setStatusElement(parent, id, "createChildrenFromMenu", "createChildrenFromMenu");
}

void EditorGenerator::setStatusElement(
		QDomElement &parent, Id const &id, QString const &tagName, QString const &propertyName)
{
	if (mApi.stringProperty(id, propertyName) == "true") {
		QDomElement statusElement = mDocument.createElement(tagName);
		parent.appendChild(statusElement);
	}
}

void EditorGenerator::setContainer(QDomElement &parent, Id const &id)
{
	QDomElement container = mDocument.createElement("container");
	parent.appendChild(container);

	IdList inLinks = mApi.outgoingLinks(id);
	foreach (Id const inLink, inLinks) {
		if (inLink.element() == "Container") {
			Id const elementId = mApi.to(inLink);
			QString const typeName = elementId.element();
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
		if (idChild.element() == "MetaEntityPropertiesAsContainer") {
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

void EditorGenerator::setExplosion(QDomElement &parent, Id const &id)
{
	QDomElement explodesTo = mDocument.createElement("explodesTo");
	parent.appendChild(explodesTo);

	IdList const inLinks = mApi.incomingLinks(id);
	foreach (Id const inLink, inLinks) {
		if (inLink.element() == "Explosion") {
			Id const elementId = mApi.from(inLink);
			QString const typeName = elementId.element();
			if (typeName == "MetaEntityNode") {
				QDomElement target = mDocument.createElement("target");
				ensureCorrectness(elementId, target, "type", mApi.name(elementId));
				setExplosionProperties(target, inLink);
				explodesTo.appendChild(target);
			} else if (typeName == "MetaEntityImport") {
				QDomElement target = mDocument.createElement("target");
				ensureCorrectness(elementId, target, "type"
						, mApi.stringProperty(elementId, "importedFrom") + "::" + mApi.name(elementId));
				setExplosionProperties(target, inLink);
				explodesTo.appendChild(target);
			}
		}
	}
}

void EditorGenerator::setExplosionProperties(QDomElement &target, Id const &linkId)
{
	target.setAttribute("makeReusable", mApi.property(linkId, "makeReusable").toString());
	target.setAttribute("requireImmediateLinkage", mApi.property(linkId, "requireImmediateLinkage").toString());
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

void EditorGenerator::ensureCorrectness(
		Id const &id, QDomElement element, QString const &tagName, QString const &value)
{
	if (value.isEmpty() && (tagName == "displayedName")) {
		return;
	} else if (value.isEmpty()) {
		mErrorReporter.addWarning(QString (QObject::tr("not filled %1\n")).arg(tagName), id);
		element.setAttribute(tagName, "");
	} else if (tagName == "name") {
		QRegExp patten;
		patten.setPattern("[A-Za-z_]+([A-Za-z_0-9 :]*)");
		if (patten.exactMatch(value)) {
			element.setAttribute(tagName, value);
		} else {
			mErrorReporter.addWarning(QObject::tr("wrong name\n"), id);
			element.setAttribute(tagName, value);
		}
	} else if ((element.nodeName() == "possibleEdge") && ((tagName == "beginName") || (tagName == "endName"))) {
		if ((value == "NonTyped") || findPort(value)) {
			element.setAttribute(tagName, value);
		} else {
			mErrorReporter.addError(QObject::tr("wrong %1 for possible edge: must be port type\n").arg(tagName), id);
		}
	} else {
		element.setAttribute(tagName, value);
	}
}

bool EditorGenerator::findPort(QString const &name) const
{
	foreach (Id const &port, mApi.elementsByType("MetaEntityPort")) {
		if (mApi.name(port) == name) {
			return true;
		}
	}

	return false;
}
