#include "editorGenerator.h"

#include <QFile>
#include <QTextStream>

using namespace qReal;
using namespace generators;

EditorGenerator::EditorGenerator(qrRepo::RepoApi const &api)
	: mApi(api)
{
}

void EditorGenerator::generate(Id const &editor)
{
	createMetaEditor(editor);
}

// Функция, генерирующая сущности и отношения в редакторе
QDomElement EditorGenerator::createNode(QDomDocument doc, Id const &nodeId, QString const &prefix)
{
	QString stringType = mApi.stringProperty(nodeId, "entity_type");
	if (stringType != "node" && stringType != "edge")
		stringType = "node";
	QString const postPrefix = stringType == "node" ? "n" : "e";
	QString const nameString = mApi.name(nodeId);
	QString const idString = prefix + postPrefix + nameString;

	QDomElement resultElement = doc.createElement(stringType);
	QDomElement graphElement = doc.createElement("graphics");
	QDomElement viewElement = doc.createElement("view");
	graphElement.appendChild(viewElement);
	resultElement.appendChild(graphElement);
	QDomElement domLogicElement = doc.createElement("logic");
	resultElement.appendChild(domLogicElement);
	QDomElement domPropElement = doc.createElement("properties");
	QDomElement domGeneElement = doc.createElement("generalizations");

	resultElement.setAttribute("id", idString);
	resultElement.setAttribute("name", nameString);

	if (mApi.stringProperty(nodeId, "line_type") != "" && postPrefix == "e") {
		QDomElement lineType = doc.createElement("line_type");
		lineType.setAttribute("type", mApi.stringProperty(nodeId, "line_type"));
		viewElement.appendChild(lineType);
	}
	if (postPrefix == "n") {
		QDomElement picture = doc.createElement("picture");
		picture.setAttribute("sizex", "31");
		picture.setAttribute("sizey", "100");
		viewElement.appendChild(picture);
	}

	// here links should be proceded
	IdList const links = mApi.outcomingLinks(nodeId);
	foreach (Id const link, links) {
		//here we are asking things about link

		// TODO: Не знаю, что тут должно происходить, надо разобраться.

		Id const otherNode = mApi.otherEntityFromLink(link, nodeId);
		if (mApi.typeName(otherNode) == "mednMetaEntityAttribute") {
			QDomElement domProp = doc.createElement("property");
			domPropElement.appendChild(domProp);
			domProp.setAttribute("name", mApi.name(otherNode));
			QString const attributeType = mApi.stringProperty(otherNode, "attributeType");
			domProp.setAttribute("type", attributeType.isEmpty() ? "string" : attributeType);
		} else if (mApi.typeName(otherNode) == "mednMetaEntity") {
			QDomElement domGene = doc.createElement("parent");
			domGeneElement.appendChild(domGene);
			QString const entityType = mApi.stringProperty(otherNode, "entity_type");
			domGene.setAttribute("parent_id", prefix + (entityType == "node" ? "n" : "e")
				+ mApi.name(otherNode));
		} else {
			QDomElement domGene = doc.createElement("parent");
			domGeneElement.appendChild(domGene);
			domGene.setAttribute("parent_id", mApi.typeName(otherNode));
		}
	}

	domLogicElement.appendChild(domPropElement);
	domLogicElement.appendChild(domGeneElement);

	return resultElement;
}

// Функция создает и сохраняет редактор редактор
void EditorGenerator::createMetaEditor(Id const &editor)
{
	QString editorTitle = mApi.name(editor);
	QString includeString = "kernel_metamodel";
	QString entityPrefix = mApi.stringProperty(editor, "prefix_name");
	if (entityPrefix.isEmpty())
		entityPrefix = "tmp";

	QDomDocument doc(editorTitle);

	QDomElement domMainElement = doc.createElement("metamodel");
	domMainElement.setAttribute("xmlns","http://schema.real.com/schema/");
	doc.appendChild(domMainElement);

	QDomElement domIncludeEl = doc.createElement("include");
	domIncludeEl.appendChild(doc.createTextNode(includeString));

	QDomElement domNamespaceEl = doc.createElement("namespace");
	domNamespaceEl.appendChild(doc.createTextNode(editorTitle));
	QDomElement domEditorEl = doc.createElement("editor");
	domMainElement.appendChild(domIncludeEl);
	domMainElement.appendChild(domNamespaceEl);
	domMainElement.appendChild(domEditorEl);

	domEditorEl.setAttribute("name", editorTitle);

	IdList const children = mApi.children(editor);
	foreach (Id const child, children) {
		if (mApi.typeName(child) == "mednMetaEntity") {
			QDomElement domNewNode = createNode(doc, child, entityPrefix);
			domEditorEl.appendChild(domNewNode);
		}
	}

	QFile file(editorTitle + ".xml");
	if (file.open(QIODevice::WriteOnly))  {
		QTextStream(&file) << "<?xml version='1.0' encoding='utf-8'?>\n";
		QTextStream(&file) << doc.toString();
		file.close();
	}
}
