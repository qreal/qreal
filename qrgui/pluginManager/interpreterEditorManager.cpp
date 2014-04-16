#include <QtCore/QCoreApplication>
#include <QtCore/QtDebug>
#include <QtCore/QUuid>
#include <QtWidgets/QMessageBox>
#include <QtGui/QIcon>

#include <qrkernel/ids.h>
#include <qrkernel/exception/exception.h>
#include <qrrepo/repoApi.h>
#include <qrutils/outFile.h>

#include "pluginManager/interpreterEditorManager.h"
#include "umllib/nodeElement.h"
#include "umllib/edgeElement.h"
#include "pluginManager/interpreterElementImpl.h"
#include "mainwindow/mainWindow.h"

using namespace qReal;
using namespace utils;

InterpreterEditorManager::InterpreterEditorManager(QString const &fileName, QObject *parent)
		: QObject(parent)
		, mMetamodelFile(fileName)
{
	qrRepo::RepoApi * const repo = new qrRepo::RepoApi(fileName);
	mEditorRepoApi.insert("test", repo);
}

InterpreterEditorManager::~InterpreterEditorManager()
{
	foreach (qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
		delete repo;
	}
}

Id InterpreterEditorManager::element(Id const &id, qrRepo::RepoApi const * const repo, Id const &diagram) const
{
	foreach (Id const &element, repo->children(diagram)) {
		if (id.element() == repo->name(element) && repo->isLogicalElement(element)) {
			return element;
		}
	}

	return Id();
}

Id InterpreterEditorManager::diagramOrElement(Id const &id, qrRepo::RepoApi const * const repo, Id const &editor) const
{
	foreach (Id const &diagram, repo->children(editor)) {
		if (diagram.element() == "MetaEditorDiagramNode"
				&& id.diagram() == repo->name(diagram)
				&& repo->isLogicalElement(diagram))
		{
			if (!id.element().isEmpty()) {
				return element(id, repo, diagram);
			} else {
				return diagram;
			}
		}
	}

	return Id();
}

QPair<qrRepo::RepoApi*, Id> InterpreterEditorManager::repoAndMetaId(Id const &id) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id const &editor,  repo->elementsByType("MetamodelDiagram")) {
			if (id.editor() == repo->name(editor) && repo->isLogicalElement(editor)) {
				if (!id.diagram().isEmpty()) {
					return qMakePair(repo, diagramOrElement(id, repo, editor));
				} else {
					return qMakePair(repo, editor);
				}
			}
		}
	}

	return QPair<qrRepo::RepoApi*, Id>();
}

IdList InterpreterEditorManager::editors() const
{
	IdList result;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id const &editor, repo->elementsByType("MetaEditorDiagramNode")) {
			if (repo->isLogicalElement(editor)) {
				result << Id(repo->name(repo->parent(editor)), repo->name(editor));
			}
		}
	}

	return result;
}

IdList InterpreterEditorManager::diagrams(const Id &editor) const
{
	IdList result;
	foreach (qrRepo::RepoApi const * const repo, mEditorRepoApi.values()) {
		foreach (Id const &edit, repo->elementsByType("MetamodelDiagram")) {
			if (editor.editor() != repo->name(edit) || !repo->isLogicalElement(edit)) {
				continue;
			}

			foreach (Id const &diagram, repo->children(edit)) {
				if (diagram.element() == "MetaEditorDiagramNode") {
					result << Id(repo->name(edit), repo->name(diagram));
				}
			}
		}
	}

	return result;
}

IdList InterpreterEditorManager::elements(const Id &diagram) const
{
	IdList result;
	foreach (qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
		foreach (Id const &editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id const &editorChild, repo->children(editor)) {
				if (diagram.diagram() != repo->name(editorChild) || !repo->isLogicalElement(editorChild)) {
					continue;
				}

				foreach (Id const &diagramChild, repo->children(editorChild)) {
					if (diagramChild.element() == "MetaEntityEdge"
							|| !repo->stringProperty(diagramChild, "shape").isEmpty())
					{
						if (!repo->hasProperty(diagramChild, "isHidden")) {
							repo->setProperty(diagramChild, "isHidden", "false");
						}

						if (repo->stringProperty(diagramChild, "isHidden") != "true") {
							result << Id(repo->name(editor), repo->name(editorChild), repo->name(diagramChild));
						}
					}
				}
			}
		}
	}

	return result;
}

QString InterpreterEditorManager::friendlyName(const Id &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	if (repoAndMetaIdPair.first->hasProperty(repoAndMetaIdPair.second, "displayedName")
			&& !repoAndMetaIdPair.first->stringProperty(repoAndMetaIdPair.second, "displayedName").isEmpty())
	{
		return repoAndMetaIdPair.first->stringProperty(repoAndMetaIdPair.second, "displayedName");
	}

	return repoAndMetaIdPair.first->name(repoAndMetaIdPair.second);
}

bool InterpreterEditorManager::hasElement(Id const &elementId) const
{
	return repoAndMetaId(elementId).first != NULL;
}

QString InterpreterEditorManager::propertyDescription(const Id &id, const QString &propertyName) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	return repoAndMetaIdPair.first->stringProperty(repoAndMetaIdPair.second, propertyName);
}

QStringList InterpreterEditorManager::allChildrenTypesOf(Id const &parent) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(parent);
	foreach (Id const &child, repoAndMetaIdPair.first->children(repoAndMetaIdPair.second)) {
		result << child.element();
	}

	return result;
}

QList<Explosion> InterpreterEditorManager::explosions(Id const &source) const
{
	// TODO: implement me
	Q_UNUSED(source)
	return QList<Explosion>();
}

bool InterpreterEditorManager::isParentOf(Id const &child, Id const &parent) const
{
	qrRepo::RepoApi const * const repoMetaModelChild = repoAndMetaId(child).first;
	Id const repoChild = repoAndMetaId(child).second;

	qrRepo::RepoApi const * const repoMetaModelParent = repoAndMetaId(parent).first;
	Id const repoParent = repoAndMetaId(parent).second;

	if (repoChild == repoParent && repoMetaModelChild == repoMetaModelParent) {
		return true;
	}

	if (repoMetaModelChild != repoMetaModelParent) {
		return false;
	}

	foreach (Id const &link , repoMetaModelChild->incomingLinks(repoChild)) {
		if (link.element() == "Inheritance") {
			if (repoMetaModelChild->otherEntityFromLink(link, repoChild) == repoParent) {
				return true;
			} else {
				Id metaChildParent = repoMetaModelChild->otherEntityFromLink(link, repoChild);
				QPair<Id, Id> editorAndDiagramPair = editorAndDiagram(repoMetaModelChild, metaChildParent);
				return isParentOf(Id(repoMetaModelChild->name(editorAndDiagramPair.first)
						, repoMetaModelChild->name(editorAndDiagramPair.second)
						, repoMetaModelChild->name(metaChildParent)), parent);
			}
		}
	}

	return false;
}

bool InterpreterEditorManager::isEditor(Id const &id) const
{
	return id.idSize() == 1;
}

bool InterpreterEditorManager::isDiagram(Id const &id) const
{
	return id.idSize() == 2;
}

bool InterpreterEditorManager::isElement(Id const &id) const
{
	return id.idSize() == 3;
}

QString InterpreterEditorManager::mouseGesture(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	if (repoAndMetaIdPair.first->hasProperty(repoAndMetaIdPair.second, "path")) {
		return repoAndMetaIdPair.first->stringProperty(repoAndMetaIdPair.second, "path");
	}

	return "";
}

class InterpreterEditorManager::CheckPropertyForParent {
public:
	virtual QString stringProperty(qrRepo::RepoApi const * const repo, Id const &parentProperty
			, QString const &propertyName) const = 0;
};

class InterpreterEditorManager::CompareProperty: public InterpreterEditorManager::CheckPropertyForParent {
public:
	CompareProperty(QString value)
			: mValue(value)
	{
	}

	virtual QString stringProperty(qrRepo::RepoApi const * const repo, Id const &parentProperty
			, QString const &propertyName) const
	{
		if (repo->name(parentProperty) == propertyName) {
			return repo->stringProperty(parentProperty, mValue);
		}

		return "";
	}

private:
	QString mValue;
};

class InterpreterEditorManager::GetProperty: public InterpreterEditorManager::CheckPropertyForParent {
public:
	virtual QString stringProperty(qrRepo::RepoApi const * const repo, Id const &parentProperty
			, QString const &propertyName) const
	{
		if (parentProperty.element() == propertyName) {
			return repo->name(parentProperty);
		}

		return "";
	}
};

class InterpreterEditorManager::HasProperty: public InterpreterEditorManager::CheckPropertyForParent {
public:
	virtual QString stringProperty(qrRepo::RepoApi const * const repo, Id const &parentProperty
			, QString const &propertyName) const
	{
		if (repo->hasProperty(parentProperty, propertyName)) {
			return repo->name(parentProperty);
		}

		return "";
	}
};

QStringList InterpreterEditorManager::propertiesFromParents(Id const &id
		, QString const &propertyName, CheckPropertyForParent const &checker) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	Id const &metaId = repoAndMetaIdPair.second;
	foreach (Id const &link , repo->incomingLinks(metaId)) {
		if (link.element() == "Inheritance") {
			foreach (Id const &parentProperty, repo->children(repo->otherEntityFromLink(link, metaId))) {
				if (!repo->hasProperty(parentProperty, "isHidden")) {
					repo->setProperty(parentProperty, "isHidden", "false");
				}

				if (repo->stringProperty(parentProperty, "isHidden") == "false") {
					QString const strProperty = checker.stringProperty(repo, parentProperty, propertyName);
					if (!strProperty.isEmpty()) {
						result << strProperty;
					}
				}
			}

			Id const metaChildParent = repo->otherEntityFromLink(link, metaId);
			if (metaChildParent != Id::rootId()) {
				QPair<Id, Id> const editorAndDiagramPair = editorAndDiagram(repo, metaChildParent);
				result << propertiesFromParents(Id(repo->name(editorAndDiagramPair.first)
						, repo->name(editorAndDiagramPair.second), repo->name(metaChildParent)), propertyName, checker);
			}
		}
	}

	return result;
}

QString InterpreterEditorManager::valueOfProperty(Id const &id, QString const &propertyName, QString const &value) const
{
	QString valueOfProperty = "";
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaIdPair.first;
	Id const metaId = repoAndMetaIdPair.second;
	QStringList const parentsProperties =  propertiesFromParents(id, propertyName, CompareProperty(value));
	if (!parentsProperties.isEmpty()) {
		valueOfProperty = parentsProperties.first();
	}

	foreach (Id const &property, repo->children(metaId)) {
		if (repo->name(property) == propertyName) {
			if (repo->hasProperty(property, value)) {
				valueOfProperty = repo->stringProperty(property, value);
			}
		}
	}

	return valueOfProperty;
}

QString InterpreterEditorManager::propertyDisplayedName(Id const &id, QString const &propertyName) const
{
	return valueOfProperty(id, propertyName, "displayedName");
}

QString InterpreterEditorManager::defaultPropertyValue(Id const &id, QString name) const
{
	return valueOfProperty(id, name, "defaultValue");
}

QString InterpreterEditorManager::typeName(Id const &id, QString const &name) const
{
	return valueOfProperty(id, name, "attributeType");
}

Id InterpreterEditorManager::findElementByType(QString const &type) const
{
	foreach (qrRepo::RepoApi const * const repo, mEditorRepoApi.values()) {
		foreach (Id const &editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id const &diagram, repo->children(editor)) {
				foreach (Id const &element, repo->children(diagram)) {
					if (type == repo->name(element)) {
						return Id(repo->name(editor), repo->name(diagram), repo->name(element));
					}
				}
			}
		}
	}

	throw Exception("No type " + type);
}

bool InterpreterEditorManager::loadPlugin(QString const &pluginName)
{
	Q_UNUSED(pluginName);
	return true;
}

bool InterpreterEditorManager::unloadPlugin(QString const &pluginName)
{
	Q_UNUSED(pluginName);
	return true;
}

QString InterpreterEditorManager::description(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	if (repoAndMetaIdPair.first->hasProperty(repoAndMetaIdPair.second, "description")) {
		return repoAndMetaIdPair.first->stringProperty(repoAndMetaIdPair.second, "description");
	}

	return "";
}

QIcon InterpreterEditorManager::icon(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaIdPair.first;
	Id const metaId = repoAndMetaIdPair.second;
	QDomDocument classDoc;
	QDomElement sdfElement;
	if (metaId.element() == "MetaEntityEdge") {
		sdfElement = classDoc.createElement("picture");
		sdfElement.setAttribute("sizex", 100);
		sdfElement.setAttribute("sizey", 60);
		QDomElement lineElement = classDoc.createElement("line");
		lineElement.setAttribute("fill", "#000000");
		QString const lineType = repo->stringProperty(metaId, "lineType").remove("Line");
		lineElement.setAttribute("stroke-style", lineType);
		lineElement.setAttribute("stroke", "#000000");
		lineElement.setAttribute("y1", 0);
		lineElement.setAttribute("x1", 0);
		lineElement.setAttribute("y2", 60);
		lineElement.setAttribute("stroke-width", 2);
		lineElement.setAttribute("x2", 100);
		lineElement.setAttribute("fill-style", "solid");
		sdfElement.appendChild(lineElement);
	} else {
		QDomDocument graphics;
		graphics.setContent(repo->stringProperty(metaId, "shape"));
		sdfElement = graphics.firstChildElement("graphics").firstChildElement("picture");
	}

	if (sdfElement.isNull()) {
		return QIcon();
	}

	classDoc.appendChild(classDoc.importNode(sdfElement, true));
	SdfIconEngineV2 * const engine = new SdfIconEngineV2(classDoc);
	return QIcon(engine);
}

ElementImpl *InterpreterEditorManager::elementImpl(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	InterpreterElementImpl * const impl = new InterpreterElementImpl(repoAndMetaIdPair.first, repoAndMetaIdPair.second);
	if (!impl) {
		return 0;
	}

	return impl;
}

IdList InterpreterEditorManager::containedTypes(Id const &id) const
{
	IdList containedTypes;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaIdPair.first;
	Id const metaId = repoAndMetaIdPair.second;
	foreach (Id const &link, repo->outgoingLinks(metaId)) {
		if (link.element() == "Container") {
			Id const metaIdTo = repo->otherEntityFromLink(link, metaId);
			QPair<Id, Id> const editorAndDiagramPair = editorAndDiagram(repo, metaIdTo);
			containedTypes << Id(repo->name(editorAndDiagramPair.first)
					 , repo->name(editorAndDiagramPair.second), repo->name(metaIdTo));
		}
	}

	return containedTypes;
}

QStringList InterpreterEditorManager::enumValues(Id const &id, const QString &name) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaIdPair.first;
	Id const metaId = repoAndMetaIdPair.second;
	foreach (Id const &property, repo->children(metaId)) {
		if (repo->name(property) == name) {
			foreach (Id const propertyChild, repo->children(property)) {
				result.append(repo->name(propertyChild));
			}
		}
	}

	return result;
}

QStringList InterpreterEditorManager::propertyNames(Id const &id) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	Id const metaId = repoAndMetaIdPair.second;
	QStringList const propertiesFromParentsList = propertiesFromParents(id, "MetaEntity_Attribute", GetProperty());
	if (!propertiesFromParentsList.isEmpty()) {
		result << propertiesFromParentsList;
	}

	foreach (Id const &idProperty, repo->children(metaId)) {
		if (idProperty.element() == "MetaEntity_Attribute") {
			if (!repo->hasProperty(idProperty, "isHidden")) {
				repo->setProperty(idProperty, "isHidden", "false");
			}

			if (repo->stringProperty(idProperty, "isHidden") != "true") {
				result << repo->name(idProperty);
			}
		}
	}

	return result;
}

QStringList InterpreterEditorManager::portTypes(Id const &id) const
{
	Q_UNUSED(id);
	return QStringList("NonTyped");
}

QStringList InterpreterEditorManager::propertiesWithDefaultValues(Id const &id) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaIdPair.first;
	Id const metaId = repoAndMetaIdPair.second;
	QStringList const parentsPropertiesWithDefaultValues = propertiesFromParents(id, "defaultName", HasProperty());
	if (!parentsPropertiesWithDefaultValues.isEmpty()) {
		result << parentsPropertiesWithDefaultValues;
	}

	foreach (Id const &property, repo->children(metaId)) {
		if (repo->hasProperty(property, "defaultName")) {
			result << repo->name(property);
		}
	}

	return result;
}

IdList InterpreterEditorManager::checkNeededPlugins(qrRepo::LogicalRepoApi const &logicalApi
		, qrRepo::GraphicalRepoApi const &graphicalApi) const
{
	Q_UNUSED(logicalApi);
	Q_UNUSED(graphicalApi);
	return IdList();
}

QList<ListenerInterface *> InterpreterEditorManager::listeners() const
{
	return QList<ListenerInterface *>();
}

bool InterpreterEditorManager::isDiagramNode(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	Id const diagram = repoAndMetaIdPair.second;
	return diagram.element() == "MetaEditorDiagramNode";
}

bool InterpreterEditorManager::isGraphicalElementNode(const Id &id) const
{
	Q_UNUSED(id);
	return false;
}

Id InterpreterEditorManager::theOnlyDiagram() const
{
	IdList const allEditors(editors());
	return (allEditors.length() == 1 && diagrams(allEditors[0]).length() == 1)
			? diagrams(allEditors[0])[0] : Id();
}

QString InterpreterEditorManager::diagramNodeNameString(Id const &editor, Id const &diagram) const
{
	QString const diagramNodeNameString = diagramNodeName(editor.editor(), diagram.diagram());
	return QString("qrm:/%1/%2/%3").arg(editor.editor(), diagram.diagram(), diagramNodeNameString);
}

QPair<qrRepo::RepoApi*, Id> InterpreterEditorManager::repoAndElement(QString const &editor
		, QString const &element) const
{
	foreach (qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
		foreach (Id const &edit,  repo->elementsByType("MetamodelDiagram")) {
			if (editor != repo->name(edit) || !repo->isLogicalElement(edit)) {
				continue;
			}

			foreach (Id const &diagram, repo->children(edit)) {
				foreach (Id const &elem, repo->children(diagram)) {
					if (element == repo->name(elem)) {
						return qMakePair(repo, elem);
					}
				}
			}
		}
	}

	return QPair<qrRepo::RepoApi*, Id>();
}

QPair<qrRepo::RepoApi*, Id> InterpreterEditorManager::repoAndDiagram(QString const &editor
		, QString const &diagram) const
{
	foreach (qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
		foreach (Id const &edit, repo->elementsByType("MetamodelDiagram")) {
			if (editor != repo->name(edit) || !repo->isLogicalElement(edit)) {
				continue;
			}

			foreach (Id const &editChild, repo->children(edit)) {
				if (diagram == repo->name(editChild)) {
					return qMakePair(repo, editChild);
				}
			}
		}
	}

	return QPair<qrRepo::RepoApi*, Id>();
}

QPair<Id, Id> InterpreterEditorManager::editorAndDiagram(qrRepo::RepoApi const * const repo, Id const &element) const
{
	foreach (Id const &edit,  repo->elementsByType("MetamodelDiagram")) {
		foreach (Id const &diagram, repo->children(edit)) {
			foreach (Id const &elem, repo->children(diagram)) {
				if (element == elem) {
					return qMakePair(edit, diagram);
				}
			}
		}
	}

	return QPair<Id, Id>();
}

QList<StringPossibleEdge> InterpreterEditorManager::possibleEdges(QString const &editor
		, QString const &elementName) const
{
	QList<StringPossibleEdge> result;
	QPair<qrRepo::RepoApi*, Id> const repoAndElementPair = repoAndElement(editor, elementName);
	qrRepo::RepoApi const * const repo = repoAndElementPair.first;
	Id const element = repoAndElementPair.second;
	foreach (Id const &child, repo->children(element)) {
		if (repo->name(child) == "possibleEdge") {
			foreach (Id const &grandChild, repo->children(child)) {
				bool const directed = repo->stringProperty(grandChild, "directed") == "true";
				result.append(qMakePair(qMakePair(repo->stringProperty(grandChild, "beginName")
						, repo->stringProperty(grandChild, "endName")), qMakePair(directed, repo->name(element))));
			}
		}
	}

	return result;
}

QStringList InterpreterEditorManager::elements(QString const &editor, QString const &diagram) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndDiagramPair = repoAndDiagram(editor, diagram);
	qrRepo::RepoApi const * const repo = repoAndDiagramPair.first;
	Id const diag = repoAndDiagramPair.second;
	foreach (Id const &element, repo->children(diag)) {
		result.append(repo->name(element));
	}

	return result;
}

int InterpreterEditorManager::isNodeOrEdge(QString const &editor, QString const &element) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndElementPair = repoAndElement(editor, element);
	Id const elem = repoAndElementPair.second;
	if (elem.element() == "MetaEntityEdge") {
		return -1;
	}

	if (elem.element() == "MetaEntityNode") {
		return 1;
	}

	return 0;
}

bool InterpreterEditorManager::isParentOf(QString const &editor, QString const &parentDiagram
		, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	return isParentOf(Id(editor, childDiagram, childElement)
			, Id(editor, parentDiagram, parentElement));
}

QString InterpreterEditorManager::diagramName(QString const &editor, QString const &diagram) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndDiagramPair = repoAndDiagram(editor, diagram);
	return repoAndDiagramPair.first->stringProperty(repoAndDiagramPair.second, "displayedName");
}

QString InterpreterEditorManager::diagramNodeName(QString const &editor, QString const &diagram) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndDiagramPair = repoAndDiagram(editor, diagram);
	return repoAndDiagramPair.first->stringProperty(repoAndDiagramPair.second, "nodeName");
}

bool InterpreterEditorManager::isInterpretationMode() const
{
	return true;
}

bool InterpreterEditorManager::isParentProperty(Id const &id, QString const &propertyName) const
{
	QStringList const propertiesFromParentsList = propertiesFromParents(id, "MetaEntity_Attribute", GetProperty());
	return propertiesFromParentsList.contains(propertyName);
}

void InterpreterEditorManager::deletePropertyInElement(qrRepo::RepoApi *repo
		, Id const &diagram, QString const &propertyName) const
{
	foreach (Id const &element, repo->children(diagram)) {
		foreach (Id const &property, repo->children(element)) {
			if (property.element() == "MetaEntity_Attribute") {
				if (repo->name(property) == propertyName) {
					repo->setProperty(property, "isHidden", "true");
					//repo->removeChild(element, property);
				}
			}
		}
	}
}

void InterpreterEditorManager::deleteProperty(QString const &propertyName) const
{
	foreach (qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
		foreach (Id const &editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id const &diagram, repo->children(editor)) {
				if (repo->isLogicalElement(diagram)) {
					deletePropertyInElement(repo, diagram, propertyName);
				}
			}
		}
	}
}

void InterpreterEditorManager::addProperty(Id const &id, QString const &propDisplayedName) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	Id const newId = Id(repoAndMetaIdPair.second.editor(), repoAndMetaIdPair.second.diagram()
			, "MetaEntity_Attribute", QUuid::createUuid().toString());
	repoAndMetaIdPair.first->addChild(repoAndMetaIdPair.second, newId);
	repoAndMetaIdPair.first->setProperty(newId, "name", propDisplayedName);
	repoAndMetaIdPair.first->setProperty(newId, "displayedName", propDisplayedName);
	QStringList propertyNames;
	propertyNames << propDisplayedName;
	repoAndMetaIdPair.first->setProperty(newId, "maskedNames", propertyNames);
	repoAndMetaIdPair.first->setProperty(newId, "isHidden", "false");
}

IdList InterpreterEditorManager::propertiesWithTheSameName(
		Id const &id
		, QString const &propertyCurrentName
		, QString const &propertyNewName
		) const
{
	if (propertyDisplayedName(id, propertyCurrentName) == propertyNewName) {
		return IdList();
	}

	IdList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	Id const metaId = repoAndMetaIdPair.second;

	foreach (Id const &idProperty, repo->children(metaId)) {
		if (idProperty.element() == "MetaEntity_Attribute") {
			if (repo->hasProperty(idProperty, "maskedNames")) {
				if (repo->property(idProperty, "maskedNames").toStringList().contains(propertyNewName)) {
					result << idProperty;
				}
			} else {
				QStringList propertyNames;
				propertyNames << repo->stringProperty(idProperty, "displayedName");
				repo->setProperty(idProperty, "maskedNames", propertyNames);
				if (repo->stringProperty(idProperty, "displayedName") == propertyNewName) {
					result << idProperty;
				}
			}
		}
	}

	return result;
}

QStringList InterpreterEditorManager::getSameNamePropertyParams(Id const &propertyId, QString const &propertyName) const
{
	QStringList paramsList;
	qrRepo::RepoApi *repo = nullptr;
	foreach (qrRepo::RepoApi * const repoApi, mEditorRepoApi.values()) {
		if (repoApi->exist(propertyId))
			repo = repoApi;
	}

	paramsList << propertyName;
	QString state = "";
	if (repo->hasProperty(propertyId, "isHidden")) {
		if (repo->stringProperty(propertyId, "isHidden") == "true") {
			state = tr("Deleted");
		}
	}

	if (state.isEmpty()) {
		if (propertyName.compare(repo->stringProperty(propertyId, "displayedName")) == 0) {
			state = tr("Existed");
		} else {
			state = tr("Renamed to ");
			state += repo->stringProperty(propertyId, "displayedName");
		}
	}

	paramsList << state;
	paramsList << repo->stringProperty(propertyId, "attributeType");
	paramsList << repo->stringProperty(propertyId, "defaultValue");
	return paramsList;
}

void InterpreterEditorManager::restoreRemovedProperty(Id const &propertyId, QString const &previousName) const
{
	qrRepo::RepoApi *repo = nullptr;
	foreach (qrRepo::RepoApi *const repoApi, mEditorRepoApi.values()) {
		if (repoApi->exist(propertyId))
			repo = repoApi;
	}

	repo->setProperty(propertyId, "isHidden", "false");
	if (repo->stringProperty(propertyId, "displayedName") != previousName) {
		repo->setProperty(propertyId, "displayedName", previousName);
	}
}

void InterpreterEditorManager::restoreRenamedProperty(Id const &propertyId, QString const &previousName) const
{
	qrRepo::RepoApi *repo = nullptr;
	for (qrRepo::RepoApi * const repoApi : mEditorRepoApi.values()) {
		if (repoApi->exist(propertyId)) {
			repo = repoApi;
		}
	}

	if (repo) {
		repo->setProperty(propertyId, "displayedName", previousName);
	}
}

void InterpreterEditorManager::setProperty(qrRepo::RepoApi *repo, Id const &id
		, QString const &property, QVariant const &propertyValue) const
{
	repo->setProperty(id, property, propertyValue);
}

void InterpreterEditorManager::updateProperties(Id const &id, QString const &property, QString const &propertyType
		, QString const &propertyDefaultValue, QString const &propertyDisplayedName) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	Id propertyMetaId;
	foreach (Id const &propertyId, repo->children(repoAndMetaIdPair.second)) {
		if (propertyId.element() == "MetaEntity_Attribute" && repo->name(propertyId) == property) {
			propertyMetaId = propertyId;
		}
	}

	setProperty(repo, propertyMetaId, "attributeType", propertyType);
	setProperty(repo, propertyMetaId, "defaultValue", propertyDefaultValue);
	QStringList propertyNames;
	if (repo->hasProperty(propertyMetaId, "maskedNames")) {
		propertyNames = repo->property(propertyMetaId, "maskedNames").toStringList();
	} else {
		propertyNames << repo->stringProperty(propertyMetaId,"displayedName");
	}

	if (!propertyNames.contains(propertyDisplayedName)) {
		propertyNames << propertyDisplayedName;
	}

	setProperty(repo, propertyMetaId, "maskedNames", propertyNames);
	setProperty(repo, propertyMetaId, "displayedName", propertyDisplayedName);
}

QString InterpreterEditorManager::propertyNameByDisplayedName(Id const &id, QString const &displayedPropertyName) const
{
	QString propertyName = "";
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	foreach (Id const &property, repoAndMetaIdPair.first->children(repoAndMetaIdPair.second)) {
		if (property.element() == "MetaEntity_Attribute"
			&& repoAndMetaIdPair.first->stringProperty(property, "displayedName") == displayedPropertyName)
		{
			propertyName = repoAndMetaIdPair.first->name(property);
		}
	}

	return propertyName;
}

IdList InterpreterEditorManager::children(Id const &parent) const {
	IdList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(parent);
	qrRepo::RepoApi const * const repo = repoAndMetaIdPair.first;
	Id const metaId = repoAndMetaIdPair.second;
	foreach (Id const &link , repo->outgoingLinks(metaId)) {
		if (link.element() == "Inheritance") {
			Id const metaChild = repo->otherEntityFromLink(link, metaId);
			QPair<Id, Id> const editorAndDiagramPair = editorAndDiagram(repo, metaChild);
			Id const child = Id(repo->name(editorAndDiagramPair.first), repo->name(editorAndDiagramPair.second)
					, repo->name(metaChild));
			result << child;
			result << children(child);
		}
	}

	return result;
}

QString InterpreterEditorManager::shape(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaIdPair.first;
	Id const metaId = repoAndMetaIdPair.second;
	if (metaId.element() == "MetaEntityNode") {
		return repo->stringProperty(metaId, "shape");
	}

	return "";
}

void InterpreterEditorManager::updateShape(Id const &id, QString const &graphics) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	if (repoAndMetaIdPair.second.element() == "MetaEntityNode") {
		repoAndMetaIdPair.first->setProperty(repoAndMetaIdPair.second, "shape", graphics);
	}
}

void InterpreterEditorManager::resetIsHidden(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	repoAndMetaIdPair.first->setProperty(repoAndMetaIdPair.second, "isHidden", "false");
}

QString InterpreterEditorManager::getIsHidden(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	Id const metaId = repoAndMetaIdPair.second;
	return repo->stringProperty(metaId, "isHidden");
}

void InterpreterEditorManager::deleteElement(MainWindow *mainWindow, Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	Id const metaId = repoAndMetaIdPair.second;
	IdList const logicalIdList = mainWindow->models()->logicalRepoApi().logicalElements(id.type());
	foreach (Id const &logicalId, logicalIdList) {
		QModelIndex const index = mainWindow->models()->logicalModelAssistApi().indexById(logicalId);
		mainWindow->models()->logicalModel()->removeRow(index.row(), index.parent());
	}
	repo->setProperty(metaId, "isHidden", "true");
	//repo->removeChild(repo->parent(metaId), metaId);
	//repo->removeElement(metaId);
}

bool InterpreterEditorManager::isRootDiagramNode(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaIdPair.first;
	Id const metaId = repoAndMetaIdPair.second;
	QPair<Id, Id> const editorAndDiagramPair = editorAndDiagram(repo, metaId);
	return repo->stringProperty(editorAndDiagramPair.second, "nodeName") == repo->name(metaId);
}

void InterpreterEditorManager::setStandartConfigurations(qrRepo::RepoApi *repo, Id const &id, Id const &parent
		, QString const &name) const
{
	repo->addChild(parent, id);
	repo->setProperty(id, "name", name);
	repo->setProperty(id, "displayedName", name);
	repo->setFrom(id, Id::rootId());
	repo->setTo(id, Id::rootId());
	repo->setProperty(id, "fromPort", 0.0);
	repo->setProperty(id, "toPort", 0.0);
	repo->setProperty(id, "links", IdListHelper::toVariant(IdList()));
	repo->setProperty(id, "outgoingConnections", IdListHelper::toVariant(IdList()));
	repo->setProperty(id, "incomingConnections", IdListHelper::toVariant(IdList()));
	repo->setProperty(id, "outgoingUsages", IdListHelper::toVariant(IdList()));
	repo->setProperty(id, "incomingUsages", IdListHelper::toVariant(IdList()));
	repo->setProperty(id, "position", QPointF(0,0));
	repo->setProperty(id, "configuration", QVariant(QPolygon()));
}


void InterpreterEditorManager::addNodeElement(Id const &diagram, QString const &name, bool isRootDiagramNode) const
{
	QString const shape =
			"<graphics>\n"
			"    <picture sizex=\"50\" sizey=\"50\">\n"
			"        <image y1=\"0\" name=\"\" x1=\"0\" y2=\"50\" x2=\"50\"/>\n"
			"    </picture>\n"
			"    <labels>\n"
			"        <label x=\"-10\" y=\"60\" textBinded=\"name\"/>\n"
			"    </labels>\n"
			"    <ports>\n"
			"        <pointPort x=\"0\" y=\"25\"/>\n"
			"        <pointPort x=\"50\" y=\"25\"/>\n"
			"        <pointPort x=\"25\" y=\"0\"/>\n"
			"        <pointPort x=\"25\" y=\"50\"/>\n"
			"    </ports>\n"
			"    <nonResizeable/>\n"
			"</graphics>\n"
			;

	QPair<qrRepo::RepoApi*, Id> const repoAndDiagramPair = repoAndDiagram(diagram.editor(), diagram.diagram());
	qrRepo::RepoApi * const repo = repoAndDiagramPair.first;
	Id const diag = repoAndDiagramPair.second;
	Id const nodeId("MetaEditor", "MetaEditor", "MetaEntityNode", QUuid::createUuid().toString());
	repo->addChild(diag, nodeId);

	if (isRootDiagramNode) {
		repo->setProperty(diag, "nodeName", name);
	}

	repo->setProperty(nodeId, "name", name);
	repo->setProperty(nodeId, "displayedName", name);
	repo->setProperty(nodeId, "shape", shape);
	repo->setProperty(nodeId, "isResizeable", "true");
	repo->setProperty(nodeId, "isPin", "false");
	repo->setProperty(nodeId, "isAction", "false");
	repo->setProperty(nodeId, "links", IdListHelper::toVariant(IdList()));
	repo->setProperty(nodeId, "createChildrenFromMenu", "false");
	repo->setProperty(nodeId, "isHidden", "false");
	foreach (Id const &elem, repo->children(diag)) {
		if (repo->name(elem) == "AbstractNode" && repo->isLogicalElement(elem)) {
			Id const inheritanceLink("MetaEditor", "MetaEditor", "Inheritance", QUuid::createUuid().toString());
			setStandartConfigurations(repo, inheritanceLink, Id::rootId(), "Inheritance");
			repo->setFrom(inheritanceLink, elem);
			repo->setTo(inheritanceLink, nodeId);

			Id const containerLink("MetaEditor", "MetaEditor", "Container", QUuid::createUuid().toString());
			setStandartConfigurations(repo, containerLink, Id::rootId(), "Container");
			repo->setFrom(containerLink, nodeId);
			repo->setTo(containerLink, elem);
		}
	}
}

void InterpreterEditorManager::addEdgeElement(Id const &diagram, QString const &name, QString const &labelText
		, QString const &labelType, QString const &lineType, QString const &beginType, QString const &endType) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndDiagramPair = repoAndDiagram(diagram.editor(), diagram.diagram());
	qrRepo::RepoApi * const repo = repoAndDiagramPair.first;
	Id const diag = repoAndDiagramPair.second;
	Id edgeId("MetaEditor", "MetaEditor", "MetaEntityEdge", QUuid::createUuid().toString());
	repo->addChild(diag, edgeId);
	Id associationId("MetaEditor", "MetaEditor", "MetaEntityAssociation", QUuid::createUuid().toString());
	repo->addChild(edgeId, associationId);

	repo->setProperty(edgeId, "name", name);
	repo->setProperty(edgeId, "displayedName", name);
	repo->setProperty(edgeId, "labelText", labelText);
	repo->setProperty(edgeId, "labelType", labelType);
	repo->setProperty(edgeId, "lineType", lineType);
	repo->setProperty(edgeId, "links", "");
	repo->setProperty(edgeId, "shape", "broken");
	repo->setProperty(edgeId, "isHidden", "false");

	repo->setProperty(associationId, "name", name + "Association");
	repo->setProperty(associationId, "beginType", beginType);
	repo->setProperty(associationId, "endType", endType);
}

QPair<Id, Id> InterpreterEditorManager::createEditorAndDiagram(QString const &name) const
{
	Id const editor("MetaEditor", "MetaEditor", "MetamodelDiagram", QUuid::createUuid().toString());
	Id const diagram("MetaEditor", "MetaEditor", "MetaEditorDiagramNode", QUuid::createUuid().toString());
	qrRepo::RepoApi * const repo = mEditorRepoApi.value("test");
	repo->addChild(Id::rootId(), editor);
	repo->setProperty(editor, "name", name);
	repo->setProperty(editor, "displayedName", name);
	repo->addChild(editor, diagram);
	repo->setProperty(diagram, "name", name);
	repo->setProperty(diagram, "displayedName", name);
	repo->setProperty(diagram, "nodeName", name);
	Id const nodeId("MetaEditor", "MetaEditor", "MetaEntityNode", QUuid::createUuid().toString());
	repo->addChild(diagram, nodeId);
	repo->setProperty(nodeId, "name", "AbstractNode");
	repo->setProperty(nodeId, "displayedName", "AbstractNode");
	repo->setProperty(nodeId, "shape", "");
	repo->setProperty(nodeId, "links", IdListHelper::toVariant(IdList()));
	repo->setProperty(nodeId, "isResizeable", "true");
	repo->setProperty(nodeId, "isPin", "false");
	repo->setProperty(nodeId, "isAction", "false");
	Id const containerLink("MetaEditor", "MetaEditor", "Container", QUuid::createUuid().toString());
	setStandartConfigurations(repo, containerLink, Id::rootId(), "Container");
	repo->setFrom(containerLink, nodeId);
	repo->setTo(containerLink, nodeId);
	return qMakePair(Id(repo->name(editor)), Id(repo->name(editor), repo->name(diagram)));
}

void InterpreterEditorManager::saveMetamodel(QString const &newMetamodelFileName)
{
	if (!newMetamodelFileName.isEmpty()) {
		mEditorRepoApi.value("test")->saveTo(newMetamodelFileName);
		mMetamodelFile = newMetamodelFileName;
	} else {
		mEditorRepoApi.value("test")->saveTo(mMetamodelFile);
	}
}

QString InterpreterEditorManager::saveMetamodelFilePath() const
{
	return mMetamodelFile;
}

QStringList InterpreterEditorManager::paletteGroups(Id const &editor, Id const &diagram) const
{
	Q_UNUSED(editor);
	Q_UNUSED(diagram);
	return QStringList();
}

QStringList InterpreterEditorManager::paletteGroupList(Id const &editor,Id const &diagram, QString const &group) const
{
	Q_UNUSED(editor);
	Q_UNUSED(diagram);
	Q_UNUSED(group);
	return QStringList();
}

QStringList InterpreterEditorManager::referenceProperties(const Id &id) const
{
	Q_UNUSED(id);
	return QStringList();
}

QString InterpreterEditorManager::paletteGroupDescription(Id const &editor, const Id &diagram
		, const QString &group) const
{
	Q_UNUSED(editor);
	Q_UNUSED(diagram);
	Q_UNUSED(group);
	return "";
}

bool InterpreterEditorManager::shallPaletteBeSorted(const Id &editor, Id const &diagram) const
{
	Q_UNUSED(editor);
	Q_UNUSED(diagram);
	return true;
}

IdList InterpreterEditorManager::groups(Id const &diagram)
{
	Q_UNUSED(diagram);
	return IdList();
}

Pattern InterpreterEditorManager::getPatternByName (QString const &str) const
{
	Q_UNUSED(str);
	return Pattern();
}

QList<QString> InterpreterEditorManager::getPatternNames() const
{
	return QList<QString>();
}

QSize InterpreterEditorManager::iconSize(Id const &id) const
{
	Q_UNUSED(id);
	return QSize();
}
