#include "metaGenerator.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QMessageBox>

#include "../../kernel/roles.h"
#include "../../../qrrepo/repoApi.h"

#include "../../../utils/outFile.h"

using namespace qReal;
using namespace generators;
using namespace utils;

MetaGenerator::MetaGenerator(qrRepo::RepoApi const &api)
	: mApi(api)
{
}

QHash<Id, QString> MetaGenerator::getMetamodelList()
{
	Id repoId = ROOT_ID;

	IdList const metamodels = mApi.children(repoId);
	QHash<Id, QString> metamodelList;

	foreach (Id const key, metamodels) {
		QString const objectType = mApi.typeName(key);
		if (objectType == "MetamodelDiagram") {
			if (mApi.stringProperty(key, "name of the directory") != "")
				metamodelList.insert(key, mApi.stringProperty(key, "name of the directory"));
			else
				mErrorReporter.addWarning("no the name of the directory", key);
		}
	}
	return metamodelList;
}

gui::ErrorReporter MetaGenerator::generateEditor(Id const metamodelId, const QString &pathToFile)
{
	QString includeProList;
	QFileInfo fileName(pathToFile);
	QString baseName = fileName.baseName();

	QDomElement metamodel = mDocument.createElement("metamodel");
	metamodel.setAttribute("xmlns", "http://schema.real.com/schema/");
	mDocument.appendChild(metamodel);

	QString const includeFile = mApi.stringProperty(metamodelId, "include");
	QStringList const includeList = includeFile.split(", ", QString::SkipEmptyParts);
	foreach (QString const name, includeList) {
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

	createDiagrams (metamodel, metamodelId);

	OutFile outpro(pathToFile + ".pro");
	outpro() << QString("QREAL_XML = %1\n").arg(baseName + ".xml");
	if (includeProList != "")
		outpro() << QString("QREAL_XML_DEPENDS = %1\n").arg(includeProList);
	outpro() << QString ("QREAL_EDITOR_NAME = %1\n").arg(baseName);
	outpro() << "\n";
	outpro() << "include (../editorsCommon.pri)";

	OutFile outxml(pathToFile + ".xml");
	outxml() << "<?xml version='1.0' encoding='utf-8'?>\n";
	outxml() << mDocument.toString(4);
	mDocument.clear();
	return mErrorReporter;;

}

void MetaGenerator::createDiagrams(QDomElement &parent, const Id &id)
{
	IdList rootDiagrams = mApi.children(id);
	foreach (Id const typeDiagram, rootDiagrams) {
		QString const objectType = mApi.typeName(typeDiagram);
		if (objectType == "MetaEditorDiagramNode") {
			QDomElement diagram = mDocument.createElement("diagram");
			ensureCorrectness (typeDiagram, diagram, "name", mApi.name(typeDiagram));
			ensureCorrectness (typeDiagram, diagram, "displayedName", mApi.stringProperty(typeDiagram, "displayedName"));
			parent.appendChild(diagram);

			serializeObjects(diagram, typeDiagram);
		}
	}
}

void MetaGenerator::serializeObjects(QDomElement &parent, Id const &idParent)
{
	IdList const childElems = mApi.children(idParent);
	mElements = childElems;
	mDiagramName = mApi.name(idParent);

	QDomElement tagNonGraphic = mDocument.createElement("nonGraphicTypes");
	parent.appendChild(tagNonGraphic);

	foreach (Id const id, childElems) {
		if (idParent != ROOT_ID) {
			QString const objectType = mApi.typeName(id);
			if (objectType == "MetaEntityEnum")
				createEnum(tagNonGraphic, id);
		}
	}

	QDomElement tagGraphic = mDocument.createElement("graphicTypes");
	parent.appendChild(tagGraphic);

	foreach (Id const id, childElems) {
		if (idParent != ROOT_ID) {
			QString const objectType = mApi.typeName(id);
			if (objectType == "MetaEntityImport")
				createImport(tagGraphic, id);
			if (objectType == "MetaEntityNode")
				createNode (tagGraphic, id);
			if (objectType == "MetaEntityEdge")
				createEdge (tagGraphic, id);
		}
	}
}

void MetaGenerator::createImport(QDomElement &parent, const Id &id)
{
	QDomElement import = mDocument.createElement("import");
	if ((mApi.stringProperty(id, "Imported from") != "") && (mApi.name(id) != ""))
		ensureCorrectness(id, import, "name", mApi.stringProperty(id, "Imported from") + "::" + mApi.name(id));
	else {
		mErrorReporter.addWarning(QString ("not filled %1\n").arg("name/imported from"), id);
		import.setAttribute("name", "");
	}
	ensureCorrectness(id, import, "displayedName", mApi.stringProperty(id, "displayedName"));
	ensureCorrectness(id, import, "as", mApi.stringProperty(id, "as"));
	parent.appendChild(import);
}

void MetaGenerator::createNode(QDomElement &parent, Id const &id)
{
	QDomElement node = mDocument.createElement("node");
	ensureCorrectness(id, node, "name", mApi.name(id));
	QString name = mApi.name(id);
	ensureCorrectness(id, node, "displayedName", mApi.stringProperty(id, "displayedName"));
	if (mApi.stringProperty(id, "Path") != "")
		node.setAttribute("path", mApi.stringProperty(id, "Path"));
	parent.appendChild(node);

	QDomDocument graphics;
	graphics.setContent(mApi.stringProperty(id, "Shape"));
	node.appendChild(graphics);

	QDomElement logic = mDocument.createElement("logic");
	node.appendChild(logic);

	setContainer(logic, name, id);
	setUsages(logic, id);
	setConnections(logic, id);
	setPossibleEdges(logic, id);
	setProperties(logic, id);
	setPin(logic, id);
	setAction(logic, id);
	setGeneralization(logic, id);
	setContextMenuFields(logic, id);
}

void MetaGenerator::createEdge(QDomElement &parent, Id const &id)
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
	}

	QDomElement logic = mDocument.createElement("logic");
	edge.appendChild(logic);

	setAssotiations(logic, id);
	setGeneralization(logic, id);
	setProperties(logic, id);
}

void MetaGenerator::createEnum(QDomElement &parent,Id const &id)
{
	QDomElement enumElement = mDocument.createElement("enum");
	ensureCorrectness(id, enumElement, "name", mApi.name(id));
	ensureCorrectness(id, enumElement, "displayedName", mApi.stringProperty(id, "displayedName"));
	parent.appendChild(enumElement);

	setValues(enumElement, id);
}

void MetaGenerator::setGeneralization(QDomElement &parent, const Id &id)
{
	QDomElement generalizations = mDocument.createElement("generalizations");
	parent.appendChild(generalizations);

	IdList parents = mApi.children(id);
	foreach (Id const id, parents) {
		QString objectType = mApi.typeName(id);
		if (objectType == "MetaEntityParent") {
			QDomElement parent = mDocument.createElement("parent");
			ensureCorrectness(id, parent, "parentName", mApi.stringProperty(id, "name"));
			generalizations.appendChild(parent);
		}
	}
}

void MetaGenerator::setProperties(QDomElement &parent,Id const &id)
{
	QDomElement tagProperties = mDocument.createElement("properties");
	parent.appendChild(tagProperties);
	IdList const childElems = mApi.children(id);

	foreach (Id const idChild, childElems)
		if (idChild != ROOT_ID) {
		QString const objectType = mApi.typeName(idChild);
		if (objectType == "MetaEntity_Attribute"){
			QDomElement property = mDocument.createElement("property");
			ensureCorrectness(idChild, property, "type", mApi.stringProperty(idChild, "attributeType"));
			ensureCorrectness(idChild, property, "name", mApi.name(idChild));
			if (mApi.stringProperty(idChild, "defaultValue") != "") {
				QDomElement defaultTag = mDocument.createElement("default");
				QDomText value = mDocument.createTextNode(mApi.stringProperty(idChild, "defaultValue"));
				defaultTag.appendChild(value);
				property.appendChild(defaultTag);
			}
			tagProperties.appendChild(property);
		}
	}

	/*QDomElement relation = mDocument.createElement("generalizations");
	parent.appendChild(relation);
	IdList const out = mApi.outgoingLinks(id);
	if (!out.empty()) {
		foreach (Id const idOut, out){
			Id const parentId = mApi.to(idOut);
			if (mApi.typeName(parentId) == "MetaEntityParent") {
				QDomElement newRelation = mDocument.createElement("parent");
				ensureCorrectness(parentId, newRelation, "parentName", mApi.stringProperty(parentId, "name"));
				relation.appendChild(newRelation);
			}
		}
	}*/
}

void MetaGenerator::setContextMenuFields(QDomElement &parent, const Id &id)
{
	QDomElement fields = mDocument.createElement("bonusContextMenuFields");
	parent.appendChild(fields);
	IdList const childElems = mApi.children(id);

	foreach (Id const idChild, childElems)
		if (idChild != ROOT_ID) {
		QString const objectType = mApi.typeName(idChild);
		if (objectType == "MetaEntityContextMenuField"){
			QDomElement field = mDocument.createElement("field");
			ensureCorrectness(idChild, field, "name", mApi.name(idChild));
			fields.appendChild(field);
		}
	}
}

void MetaGenerator::setValues(QDomElement &parent, const Id &id)
{
	IdList childElems = mApi.children(id);

	foreach (Id const idChild, childElems) {
		if (idChild != ROOT_ID) {
			QDomElement valueTag = mDocument.createElement("value");
			QDomText value = mDocument.createTextNode(mApi.stringProperty(idChild, "valueName"));
			valueTag.appendChild(value);
			parent.appendChild(valueTag);
		}
	}
}

void MetaGenerator::setAssotiations(QDomElement &parent, const Id &id)
{
	IdList const childElems = mApi.children(id);

	foreach (Id const idChild, childElems) {
		QString const objectType = mApi.typeName(idChild);
		if (objectType == "MetaEntityAssotiation") {
			QDomElement assotiationTag = mDocument.createElement("assotiations");
			ensureCorrectness(idChild, assotiationTag, "beginType", mApi.stringProperty(idChild, "beginType"));
			ensureCorrectness(idChild, assotiationTag, "endType", mApi.stringProperty(idChild, "endType"));
			parent.appendChild(assotiationTag);

			QDomElement assotiation = mDocument.createElement("assotiation");
			ensureCorrectness(idChild, assotiation, "beginName", mApi.stringProperty(idChild, "beginName"));
			ensureCorrectness(idChild, assotiation, "endName", mApi.stringProperty(idChild, "endName"));
			assotiationTag.appendChild(assotiation);
		}
	}
}

void MetaGenerator::setUsages(QDomElement &parent, const Id &id)
{
	newSetConnections(parent, id, "usages", "usage", "MetaEntityUsage");
}

void MetaGenerator::setConnections(QDomElement &parent, const Id &id)
{
	newSetConnections(parent, id, "connections", "connection", "MetaEntityConnection");
}

void MetaGenerator::newSetConnections(QDomElement &parent, const Id &id,
									  QString const &commonTagName, QString const &internalTagName, QString const &typeName)
{
	IdList const childElems = mApi.children(id);

	QDomElement connectionsTag = mDocument.createElement(commonTagName);
	parent.appendChild(connectionsTag);

	foreach (Id const idChild, childElems) {
		QString const objectType = mApi.typeName(idChild);
		if (objectType == typeName) {
			QDomElement connection = mDocument.createElement(internalTagName);
			ensureCorrectness(idChild, connection,"type", mApi.stringProperty(idChild, "Type"));
			connectionsTag.appendChild(connection);
		}
	}
}

void MetaGenerator::setPossibleEdges(QDomElement &parent, const Id &id)
{
	IdList const childElems = mApi.children(id);

	QDomElement possibleEdges = mDocument.createElement("possibleEdges");
	parent.appendChild(possibleEdges);

	foreach (Id const idChild, childElems) {
		QString const objectType = mApi.typeName(idChild);
		if (objectType == "MetaEntityPossibleEdge") {
			QDomElement possibleEdge = mDocument.createElement("possibleEdge");
			possibleEdges.appendChild(possibleEdge);
			ensureCorrectness(idChild, possibleEdge, "beginName", mApi.stringProperty(idChild, "beginName"));
			ensureCorrectness(idChild, possibleEdge, "endName", mApi.stringProperty(idChild, "endName"));
			ensureCorrectness(idChild, possibleEdge, "directed", mApi.stringProperty(idChild, "directed"));
		}
	}
}

void MetaGenerator::setPin(QDomElement &parent, const Id &id)
{
	setStatusElement(parent, id, "pin", "is Pin");
}

void MetaGenerator::setAction(QDomElement &parent, const Id &id)
{
	setStatusElement(parent, id, "action", "is Action");
}

void MetaGenerator::setStatusElement(QDomElement &parent, const Id &id, const QString &tagName, const QString &propertyName)
{
	if (mApi.stringProperty(id, propertyName) == "true") {
		QDomElement statusElement = mDocument.createElement(tagName);
		parent.appendChild(statusElement);
	}
}

void MetaGenerator::setContainer(QDomElement &parent, QString name, const Id &id)
{
	QDomElement container = mDocument.createElement("container");
	parent.appendChild(container);

	foreach (Id const idChild, mElements) {
		if ((mApi.stringProperty(idChild, "container")) == name) {
			QDomElement contains = mDocument.createElement("contains");
			ensureCorrectness(idChild, contains, "type", mDiagramName + "::" + mApi.name(idChild));
			container.appendChild(contains);
		}
	}
	setContainerProperties(container, id);
}

void MetaGenerator::setContainerProperties(QDomElement &parent, const Id &id)
{
	IdList elements = mApi.children(id);

	foreach (Id const idChild, elements) {
		if (mApi.typeName(idChild) == "MetaEntityPropertiesAsContainer") {
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

void MetaGenerator::setSizesForContainer(const QString &propertyName,QDomElement &properties, const Id &id)
{
	if (mApi.stringProperty(id, propertyName + " size") != "") {
		QDomElement property = mDocument.createElement(propertyName);
		properties.appendChild(property);
		property.setAttribute("size", mApi.stringProperty(id, propertyName + " size"));
	}
}

void MetaGenerator::setBoolValuesForContainer(const QString &propertyName,QDomElement &properties, const Id &id)
{
	if (mApi.stringProperty(id, propertyName) == "true") {
		QDomElement property = mDocument.createElement(propertyName);
		properties.appendChild(property);
	}
}

void MetaGenerator::ensureCorrectness(const Id &id, QDomElement element, const QString &tagName, const QString &value)
{
	QString tag = tagName;
	if ((value == "") && ((tag == "displayedName")))
		return;
	if (value == "") {
		mErrorReporter.addWarning(QString ("not filled %1\n").arg(tagName),id);
		element.setAttribute(tagName, "");
	}
	else
		if (tag == "name") {
		QRegExp patten;
		patten.setPattern("[A-Za-z_]+([A-Za-z_0-9 ]*)");
		if ((patten.exactMatch(value)))
			element.setAttribute(tagName, value);
		else {
			mErrorReporter.addWarning("wrong name\n",id);
			element.setAttribute(tagName, value);
		}
	}
	else element.setAttribute(tagName, value);
}

