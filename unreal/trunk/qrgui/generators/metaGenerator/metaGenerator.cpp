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
		if (objectType == "MetaEditor_MetamodelDiagram") {
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
		if (objectType == "MetaEditor_MetaEditorDiagramNode") {
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
			if (objectType == "MetaEditor_MetaEntityEnum")
				createEnum(tagNonGraphic, id);
		}
	}

	QDomElement tagGraphic = mDocument.createElement("graphicTypes");
	parent.appendChild(tagGraphic);

	setImported(tagGraphic, idParent);

	foreach (Id const id, childElems) {
		if (idParent != ROOT_ID) {
			QString const objectType = mApi.typeName(id);
			if (objectType == "MetaEditor_MetaEntityNode")
				createNode (tagGraphic, id);
			if (objectType == "MetaEditor_MetaEntityEdge")
				createEdge (tagGraphic, id);
		}
	}
}

void MetaGenerator::createNode(QDomElement &parent, Id const &id)
{
	QDomElement node = mDocument.createElement("node");
	ensureCorrectness(id, node, "name", mApi.name(id));
	QString name = mApi.name(id);
	ensureCorrectness(id, node, "displayedName", mApi.stringProperty(id, "displayedName"));
	parent.appendChild(node);

	QDomDocument graphics;
	graphics.setContent(mApi.stringProperty(id, "set Shape"));
	node.appendChild(graphics);

	QDomElement logic = mDocument.createElement("logic");
	node.appendChild(logic);

	setContainer(logic, name, id);
	setUsages(logic, id);
	setConnections(logic, id);
	setPossibleEdges(logic, id);
	setLogicAttributes(logic, id);
	setPin(logic, id);
	setAction(logic, id);
}

void MetaGenerator::createEdge(QDomElement &parent, Id const &id)
{
	QDomElement edge = mDocument.createElement("edge");
	ensureCorrectness(id, edge, "name", mApi.name(id));
	ensureCorrectness(id, edge, "displayedName", mApi.stringProperty(id, "displayedName"));
	parent.appendChild(edge);

	QDomElement graphics = mDocument.createElement("graphics");
	edge.appendChild(graphics);
	QDomElement lineType = mDocument.createElement("lineType");
	ensureCorrectness(id, lineType, "type", mApi.stringProperty(id, "lineType"));
	graphics.appendChild(lineType);

	QDomElement logic = mDocument.createElement("logic");
	edge.appendChild(logic);

	setAssotiations(logic, id);
	setLogicAttributes(logic, id);
}

void MetaGenerator::createEnum(QDomElement &parent,Id const &id)
{
	QDomElement enumElement = mDocument.createElement("enum");
	ensureCorrectness(id, enumElement, "name", mApi.name(id));
	ensureCorrectness(id, enumElement, "displayedName", mApi.stringProperty(id, "displayedName"));
	parent.appendChild(enumElement);

	setValues(enumElement, id);
}

void MetaGenerator::setLogicAttributes(QDomElement &parent,Id const &id)
{
	QDomElement tagProperties = mDocument.createElement("properties");
	parent.appendChild(tagProperties);
	IdList const childElems = mApi.children(id);

	foreach (Id const idChild, childElems)
		if (idChild != ROOT_ID) {
		QString const objectType = mApi.typeName(idChild);
		if (objectType == "MetaEditor_MetaEntity_Attribute"){
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
	QDomElement relation = mDocument.createElement("generalizations");
	parent.appendChild(relation);
	IdList const out = mApi.outgoingLinks(id);
	if (!out.empty()) {
		foreach (Id const idOut, out){
			Id const idImported = mApi.to(idOut);
			QDomElement newRelation = mDocument.createElement("parent");
			ensureCorrectness(idImported, newRelation, "parentName", mApi.stringProperty(idImported, "name"));
			relation.appendChild(newRelation);
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
		if (objectType == "MetaEditor_MetaEntityAssotiation") {
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
	newSetConnections(parent, id, "usages", "usage", "MetaEditor_MetaEntityUsage");
}

void MetaGenerator::setConnections(QDomElement &parent, const Id &id)
{
	newSetConnections(parent, id, "connections", "connection", "MetaEditor_MetaEntityConnection");
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
		if (objectType == "MetaEditor_MetaEntityPossibleEdge") {
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
		if (mApi.typeName(idChild) == "MetaEditor_MetaEntityPropertiesAsContainer") {
			QDomElement properties = mDocument.createElement("properties");
			parent.appendChild(properties);
			if (mApi.stringProperty(idChild, "sortContainer") == "true") {
				QDomElement softContainer = mDocument.createElement("softContainer");
				properties.appendChild(softContainer);
			}
			QDomElement forestalling = mDocument.createElement("forestalling");
			properties.appendChild(forestalling);
			ensureCorrectness (idChild, forestalling, "size", mApi.stringProperty(idChild, "forestalling size"));
			QDomElement childrenForestalling = mDocument.createElement("childrenForestalling");
			properties.appendChild(childrenForestalling);
			ensureCorrectness (idChild, childrenForestalling, "size", mApi.stringProperty(idChild, "childrenForestalling size"));
			if (mApi.stringProperty(idChild, "minimizeToChildren") == "true") {
				QDomElement minimizeToChildren = mDocument.createElement("minimizeToChildren");
				properties.appendChild(minimizeToChildren);
			}
			if (mApi.stringProperty(idChild, "banChildrenMove") == "true") {
				QDomElement banChildrenMove = mDocument.createElement("banChildrenMove");
				properties.appendChild(banChildrenMove);
			}
		}
	}
}

void MetaGenerator::setImported(QDomElement &parent, const Id &idParent)
{
	IdList out = mApi.outgoingLinks(idParent);

	foreach (Id const idChild, out) {
		if (mApi.typeName(idChild) == "MetaEditor_Importation") {
			Id const idImported = mApi.to(idChild);
			QDomElement import = mDocument.createElement("import");
			ensureCorrectness(idChild, import, "name", mApi.stringProperty(idImported, "name") + "::" + mApi.stringProperty(idChild, "name"));
			ensureCorrectness(idChild, import, "as", mApi.stringProperty(idChild, "as"));
			ensureCorrectness(idChild, import, "displayedName", mApi.stringProperty(idChild, "displayedName"));
			parent.appendChild(import);
		}
	}
}

void MetaGenerator::ensureCorrectness(const Id &id, QDomElement element, const QString &tagName, const QString &value)
{
	QString tag = tagName;
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

