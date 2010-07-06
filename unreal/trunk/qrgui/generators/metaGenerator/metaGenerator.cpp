#include "metaGenerator.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

#include "../../kernel/definitions.h"
#include "../../../qrrepo/repoApi.h"

#include "../../../utils/outFile.h"

using namespace qReal;
using namespace generators;
using namespace utils;

MetaGenerator::MetaGenerator(qrRepo::RepoApi const &api)
	: mApi(api)
{
}

QString MetaGenerator::generateEditor(QString const &pathToFile)
{
	Id repoId = ROOT_ID;
	IdList metamodels = mApi.children(repoId);
	QString includePro;
	QFileInfo fileName(pathToFile);
	QString baseName = fileName.baseName();

	foreach (Id const idChild, metamodels) {
		QString const objectType = mApi.typeName(idChild);
		if (objectType == "MetaEditor_MetamodelDiagram") {
			QDomElement metamodel = mDocument.createElement("metamodel");
			metamodel.setAttribute("xmlns", "http://schema.real.com/schema/");
			mDocument.appendChild(metamodel);

			QString const includeFile = mApi.stringProperty(idChild, "include");
			QStringList const includeList = includeFile.split(", ", QString::SkipEmptyParts);
			foreach (QString const name, includeList) {
				includePro += " " + name;
				QDomElement include = mDocument.createElement("include");
				QDomText value = mDocument.createTextNode(name);
				include.appendChild(value);
				metamodel.appendChild(include);
			}

			QDomElement nameSpace = mDocument.createElement("namespace");
			QDomText nameSpaceName = mDocument.createTextNode("UML 2.0");
			nameSpace.appendChild(nameSpaceName);
			metamodel.appendChild(nameSpace);

			createDiagrams (metamodel, idChild);
		}
	}
	OutFile outpro(pathToFile + ".pro");
	outpro() << QString("QREAL_XML = %1\n").arg(baseName + ".xml");
	outpro() << QString("QREAL_XML_DEPENDS = %1\n").arg(includePro);
	outpro() << QString ("QREAL_EDITOR_NAME = %1\n").arg(baseName);
	outpro() << "\n";
	outpro() << "include (../editorsCommon.pri)";

	OutFile outxml(pathToFile + ".xml");
	outxml() << "<?xml version='1.0' encoding='utf-8'?>\n";
	outxml() << mDocument.toString(4);
	return "";

}

void MetaGenerator::createDiagrams(QDomElement &parent, const Id &id)
{
	IdList rootDiagrams = mApi.children(id);
	foreach (Id const typeDiagram, rootDiagrams) {
		QString const objectType = mApi.typeName(typeDiagram);
		if (objectType == "MetaEditor_MetaEditorDiagramNode") {
			QDomElement package = mDocument.createElement("diagram");
			package.setAttribute("name", mApi.name(typeDiagram));
			package.setAttribute("displayedName", mApi.stringProperty(typeDiagram, "displayedName"));
			parent.appendChild(package);

			serializeObjects(package, typeDiagram);
		}
	}
}

void MetaGenerator::serializeObjects(QDomElement &parent, Id const &idParent)
{
	IdList const childElems = mApi.children(idParent);
	mElements = childElems;

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
	QDomElement package = mDocument.createElement("node");
	package.setAttribute("name", mApi.name(id));
	QString name = mApi.name(id);
	package.setAttribute("displayedName", mApi.stringProperty(id, "displayedName"));
	parent.appendChild(package);

	QDomElement tag = mDocument.createElement("logic");
	package.appendChild(tag);

	setContainer(tag, name);
	setUsages(tag, id);
	setConnections(tag, id);
	setLogicAttributes(tag, id);
}

void MetaGenerator::createEdge(QDomElement &parent, Id const &id)
{
	QDomElement edge = mDocument.createElement("edge");
	edge.setAttribute("name", mApi.name(id));
	QString name = mApi.name(id);
	edge.setAttribute("displayedName", mApi.stringProperty(id, "displayedName"));
	parent.appendChild(edge);

	QDomElement logic = mDocument.createElement("logic");
	edge.appendChild(logic);

	setContainer(logic, name);
	setAssotiations(logic, id);
	setLogicAttributes(logic, id);
}

void MetaGenerator::createEnum(QDomElement &parent,Id const &id)
{
	QDomElement enumTag = mDocument.createElement("enum");
	enumTag.setAttribute("name", mApi.name(id));
	enumTag.setAttribute("displayedName", mApi.stringProperty(id, "displayedName"));
	parent.appendChild(enumTag);

	setValues(enumTag, id);
}

void MetaGenerator::setLogicAttributes(QDomElement &parent,Id const &id)
{
	QDomElement tag = mDocument.createElement("properties");
	parent.appendChild(tag);
	IdList const childElems = mApi.children(id);

	foreach (Id const idChild, childElems)
		if (idChild != ROOT_ID) {
		QString const objectType = mApi.typeName(idChild);
		if (objectType == "MetaEditor_MetaEntity_Attribute"){
			QDomElement property = mDocument.createElement("property");
			property.setAttribute("type", mApi.stringProperty(idChild, "attributeType"));
			property.setAttribute("name", mApi.name(idChild));
			if (mApi.stringProperty(idChild, "defaultValue") != "") {
				QDomElement defaultTag = mDocument.createElement("default");
				QDomText value = mDocument.createTextNode(mApi.stringProperty(idChild, "defaultValue"));
				defaultTag.appendChild(value);
				property.appendChild(defaultTag);
			}
			tag.appendChild(property);
		}
	}
	QDomElement relation = mDocument.createElement("generalizations");
	parent.appendChild(relation);
	IdList const out = mApi.outgoingLinks(id);
	if (!out.empty()) {
		foreach (Id const idOut, out){
			Id const idImported = mApi.to(idOut);
			QDomElement newRelation = mDocument.createElement("parent");
			newRelation.setAttribute("parentName", mApi.stringProperty(idImported, "name"));
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
			assotiationTag.setAttribute("beginType", mApi.stringProperty(idChild, "beginType"));
			assotiationTag.setAttribute("endType", mApi.stringProperty(idChild, "endType"));
			parent.appendChild(assotiationTag);

			QDomElement assotiation = mDocument.createElement("assotiation");
			assotiation.setAttribute("beginName", mApi.stringProperty(idChild, "beginName"));
			assotiation.setAttribute("endName", mApi.stringProperty(idChild, "endName"));
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
			connection.setAttribute("type", mApi.stringProperty(idChild, "Type"));
			connectionsTag.appendChild(connection);
		}
	}
}

void MetaGenerator::setContainer(QDomElement &parent, QString name)
{
	QDomElement container = mDocument.createElement("container");
	parent.appendChild(container);

	foreach (Id const idChild, mElements) {
		if ((mApi.stringProperty(idChild, "container")) == name) {
			QDomElement contain = mDocument.createElement("contain");
			contain.setAttribute("type", mApi.name(idChild));
			container.appendChild(contain);
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
			import.setAttribute("name", mApi.stringProperty(idImported, "name") + "::" + mApi.stringProperty(idChild, "name"));
			import.setAttribute("as", mApi.stringProperty(idChild, "as"));
			import.setAttribute("displayedName", mApi.stringProperty(idChild, "displayedName"));
			parent.appendChild(import);
		}
	}
}

