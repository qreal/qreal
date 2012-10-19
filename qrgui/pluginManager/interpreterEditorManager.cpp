#include <QtCore/QCoreApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>
#include <QtCore/QtDebug>

#include "interpreterEditorManager.h"
#include "../../qrkernel/ids.h"
#include "../../qrrepo/repoApi.h"
#include "../umllib/nodeElement.h"
#include "../umllib/edgeElement.h"
#include "../../qrkernel/exception/exception.h"
#include "interpreterElementImpl.h"
#include "../../qrutils/outFile.h"

using namespace qReal;
using namespace utils;

InterpreterEditorManager::InterpreterEditorManager(QString const fileName, QObject *parent)
	: QObject(parent)
{
	qrRepo::RepoApi * const repo = new qrRepo::RepoApi(fileName);
	mEditorRepoApi.insert("test", repo);
}

Id InterpreterEditorManager::getElement(Id const &id, qrRepo::RepoApi const * const repo, Id const &diagram) const
{
	foreach (Id element, repo->children(diagram)) {
		if (id.element() == repo->name(element) && repo->isLogicalElement(element)) {
			return element;
		}
	}
	return Id();
}

Id InterpreterEditorManager::getDiagramOrElement(Id const &id, qrRepo::RepoApi const * const repo, Id const &editor) const
{
	foreach (Id diagram, repo->children(editor)) {
		if (diagram.element() == "MetaEditorDiagramNode" && id.diagram() == repo->name(diagram) && repo->isLogicalElement(diagram)) {
			if (id.element() != "") {
				return getElement(id, repo, diagram);
			} else {
				return diagram;
			}
		}
	}
	return Id();
}

QPair<qrRepo::RepoApi*, Id> InterpreterEditorManager::getRepoAndMetaId(Id const &id) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id editor,  repo->elementsByType("MetamodelDiagram")) {
			if (id.editor() == repo->name(editor) && repo->isLogicalElement(editor)) {
				if (id.diagram() != "") {
					return qMakePair(repo, getDiagramOrElement(id, repo, editor));
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
		foreach (Id editor, repo->elementsByType("MetaEditorDiagramNode")) {
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
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id edit, repo->elementsByType("MetamodelDiagram")) {
			if (editor.editor() == repo->name(edit) && repo->isLogicalElement(edit)) {
				foreach (Id diagram, repo->children(edit)) {
					if (diagram.element() == "MetaEditorDiagramNode") {
						result << Id(repo->name(edit), repo->name(diagram));
					}
				}
			}
		}
	}
	return result;
}

IdList InterpreterEditorManager::elements(const Id &diagram) const
{
	IdList result;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id editorChild, repo->children(editor)) {
				if (diagram.diagram() == repo->name(editorChild) && repo->isLogicalElement(editorChild)) {
					foreach (Id diagramChild, repo->children(editorChild)) {
						if (diagramChild.element() == "MetaEntityEdge" || !repo->stringProperty(diagramChild, "shape").isEmpty()) {
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
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	if (repoAndMetaId.first->hasProperty(repoAndMetaId.second, "displayedName")
			&& !repoAndMetaId.first->stringProperty(repoAndMetaId.second, "displayedName").isEmpty())
	{
		return repoAndMetaId.first->stringProperty(repoAndMetaId.second, "displayedName");
	}
	return repoAndMetaId.first->name(repoAndMetaId.second);
}

bool InterpreterEditorManager::hasElement(Id const &elementId) const
{
	return getRepoAndMetaId(elementId).first != NULL;
}

QString InterpreterEditorManager::propertyDescription(const Id &id, const QString &propertyName) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	return repoAndMetaId.first->stringProperty(repoAndMetaId.second, propertyName);
}

QStringList InterpreterEditorManager::getAllChildrenTypesOf(Id const &parent) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(parent);
	foreach (Id child, repoAndMetaId.first->children(repoAndMetaId.second)) {
		result << child.element();
	}
	return result;
}

bool InterpreterEditorManager::isParentOf(Id const &child, Id const &parent) const
{
	qrRepo::RepoApi const * const repoMetaModelChild = getRepoAndMetaId(child).first;
	Id const repoChild = getRepoAndMetaId(child).second;

	qrRepo::RepoApi const * const repoMetaModelParent = getRepoAndMetaId(parent).first;
	Id const repoParent = getRepoAndMetaId(parent).second;

	if (repoChild == repoParent && repoMetaModelChild == repoMetaModelParent)
		return true;

	if (repoMetaModelChild != repoMetaModelParent) {
		return false;
	}

	foreach (Id const link , repoMetaModelChild->incomingLinks(repoChild)) {
		if (link.element() == "Inheritance") {
			if (repoMetaModelChild->otherEntityFromLink(link, repoChild) == repoParent) {
				return true;
			} else {
				Id metaChildParent = repoMetaModelChild->otherEntityFromLink(link, repoChild);
				QPair<Id, Id> editorAndDiagram = getEditorAndDiagram(repoMetaModelChild, metaChildParent);
				return isParentOf(Id(repoMetaModelChild->name(editorAndDiagram.first), repoMetaModelChild->name(editorAndDiagram.second), repoMetaModelChild->name(metaChildParent)), parent);
			}
		}
	}
	return false;
}

IdList InterpreterEditorManager::getConnectedTypes(const Id &id) const
{
	return getRepoAndMetaId(id).first->connectedElements(getRepoAndMetaId(id).second);
}

bool InterpreterEditorManager::isEditor(const Id &id) const
{
	return id.idSize() == 1;
}

bool InterpreterEditorManager::isDiagram(const Id &id) const
{
	return id.idSize() == 2;
}

bool InterpreterEditorManager::isElement(const Id &id) const
{
	return id.idSize() == 3;
}

QString InterpreterEditorManager::getTypeName(const Id &id, const QString &name) const
{
	QString propertyType = "";
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	foreach (Id const property, repo->children(metaId)) {
		if (repo->name(property) == name) {
			propertyType = repo->stringProperty(property, "attributeType");
		}
	}
	return propertyType;
}

QString InterpreterEditorManager::mouseGesture(Id const &id) const
{
	//QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	//return repoAndMetaId.first->stringProperty(repoAndMetaId.second, "path");
	return "";
}

QString InterpreterEditorManager::propertyDisplayedName(Id const &id, QString const &propertyName) const
{
	QString displayedName = "";
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	foreach (Id property, repo->children(metaId)) {
		if (repo->name(property) == propertyName) {
			displayedName = repo->stringProperty(property, "displayedName");
		}
	}
	return displayedName;
}

QString InterpreterEditorManager::getDefaultPropertyValue(Id const &id, QString name) const
{
	QString defaultProperty = "";
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	foreach (Id property, repo->children(metaId)) {
		if (repo->name(property) == name) {
			defaultProperty = repo->stringProperty(property, "defaultValue");
		}
	}
	return defaultProperty;
}

Id InterpreterEditorManager::findElementByType(QString const &type) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id diagram, repo->children(editor)) {
				foreach (Id element, repo->children(diagram)) {
					if (type == repo->name(element)) {
						return Id(editor.id(), repo->name(diagram), repo->name(element));
					}
				}
			}
		}
	}
	throw Exception("No type " + type);
}

bool InterpreterEditorManager::loadPlugin(QString const &pluginName)
{
	return true;
}

bool InterpreterEditorManager::unloadPlugin(QString const &pluginName)
{
	return true;
}

QString InterpreterEditorManager::description(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	if (repoAndMetaId.first->hasProperty(repoAndMetaId.second, "displayedName")) {
		return repoAndMetaId.first->stringProperty(repoAndMetaId.second, "displayedName");
	}
	return "";
}

QIcon InterpreterEditorManager::icon(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	QDomDocument classDoc;
	QDomElement sdfElement;
	if(metaId.element() == "MetaEntityEdge") {
		sdfElement = classDoc.createElement("picture");
		sdfElement.setAttribute("sizex", 100);
		sdfElement.setAttribute("sizey", 60);
		QDomElement lineElement = classDoc.createElement("line");
		lineElement.setAttribute("fill", "#000000");
		QString lineType = repo->stringProperty(metaId, "lineType").remove("Line");
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
	SdfIconEngineV2 *engine = new SdfIconEngineV2(classDoc);
	return QIcon(engine);
}

Element* InterpreterEditorManager::graphicalObject(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	InterpreterElementImpl *impl = new InterpreterElementImpl(repoAndMetaId.first, repoAndMetaId.second, id);
	if( !impl ){
		qDebug() << "no impl";
		return 0;
	}
	if (impl->isNode()) {
		return new NodeElement(impl);
	}
	return  new EdgeElement(impl);
}

IdList InterpreterEditorManager::getContainedTypes(const Id &id) const
{
	IdList containedTypes;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	foreach (Id const link, repo->outgoingLinks(metaId)) {
		if (link.element() == "Container") {
			Id metaIdTo = repo->otherEntityFromLink(link, metaId);
			QPair<Id, Id> editorAndDiagram = getEditorAndDiagram(repo, metaIdTo);
			containedTypes << Id(repo->name(editorAndDiagram.first), repo->name(editorAndDiagram.second), repo->name(metaIdTo));
		}
	}
	return containedTypes;
}

IdList InterpreterEditorManager::getUsedTypes(const Id &id) const
{
	IdList usedTypes;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	foreach (Id child, repo->children(metaId)) {
		if (repo->name(child) == "MetaEntityUsage") {
			usedTypes << repo->property(child, "type").value<Id>();
		}
	}
	return usedTypes;
}

QStringList InterpreterEditorManager::getEnumValues(Id const &id, const QString &name) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	foreach (Id property, repo->children(metaId)) {
		if (repo->name(property) == name) {
			foreach (Id propertyChild, repo->children(property)) {
				result.append(repo->name(propertyChild));
			}
		}
	}
	return result;
}

QStringList InterpreterEditorManager::getPropertyNames(Id const &id) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	foreach (Id child, repo->children(metaId)) {
		if (child.element() == "MetaEntity_Attribute") {
			result << repo->name(child);
		}
	}
	return result;
}

QStringList InterpreterEditorManager::getPropertiesWithDefaultValues(Id const &id) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	foreach (Id property, repo->children(metaId)) {
		if (repo->hasProperty(property, "defaultName")) {
			result << repo->name(property);
		}
	}
	return result;
}

IdList InterpreterEditorManager::checkNeededPlugins(qrRepo::LogicalRepoApi const &logicalApi
		, qrRepo::GraphicalRepoApi const &graphicalApi) const
{
	return IdList();
}

QList<ListenerInterface *> InterpreterEditorManager::listeners() const
{
	return QList<ListenerInterface *>();
}

EditorInterface* InterpreterEditorManager::editorInterface(QString const &editor) const
{
	return NULL;
}

bool InterpreterEditorManager::isDiagramNode(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	Id const diagram = repoAndMetaId.second;
	return diagram.element() == "MetaEditorDiagramNode";
}

bool InterpreterEditorManager::isGraphicalElementNode(const Id &id) const
{
	return false;
}

QPair<qrRepo::RepoApi*, Id> InterpreterEditorManager::getRepoAndElement(QString const &editor, QString const &element) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id edit,  repo->elementsByType("MetamodelDiagram")) {
			if (editor == repo->name(edit) && repo->isLogicalElement(edit)) {
				foreach (Id diagram, repo->children(edit)) {
					foreach (Id elem, repo->children(diagram)) {
						if (element == repo->name(elem)) {
							return qMakePair(repo, elem);
						}
					}
				}
			}
		}
	}
	return QPair<qrRepo::RepoApi*, Id>();
}

QPair<qrRepo::RepoApi*, Id> InterpreterEditorManager::getRepoAndDiagram(QString const &editor, QString const &diagram) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id edit,  repo->elementsByType("MetamodelDiagram")) {
			if (editor == repo->name(edit) && repo->isLogicalElement(edit)) {
				foreach (Id editChild, repo->children(edit)) {
					if (diagram == repo->name(editChild)) {
						return qMakePair(repo, editChild);
					}
				}
			}
		}
	}
	return QPair<qrRepo::RepoApi*, Id>();
}

QPair<Id, Id> InterpreterEditorManager::getEditorAndDiagram(qrRepo::RepoApi const * const repo, Id const &element) const
{
	foreach (Id edit,  repo->elementsByType("MetamodelDiagram")) {
		foreach (Id diagram, repo->children(edit)) {
			foreach (Id elem, repo->children(diagram)) {
				if (element == elem) {
					return qMakePair(edit, diagram);
				}
			}
		}
	}
	return QPair<Id, Id>();
}

QList<QPair<QPair<QString, QString>, QPair<bool, QString> > > InterpreterEditorManager::getPossibleEdges(QString const &editor, QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	QPair<qrRepo::RepoApi*, Id> repoAndElement = getRepoAndElement(editor, element);
	qrRepo::RepoApi const * const repo = repoAndElement.first;
	Id const elem = repoAndElement.second;
	foreach (Id prop, repo->children(elem)) {
		if (repo->name(prop) == "possibleEdge") {
			foreach (Id propChild, repo->children(prop)) {
				bool bparam = false;
				if (repo->stringProperty(propChild, "directed") == "true") {
					bparam = true;
				}
				result.append(qMakePair(qMakePair(repo->stringProperty(propChild, "beginName"),repo->stringProperty(propChild, "endName")), qMakePair(bparam,repo->name(elem))));
			}
		}
	}
	return result;
}

QStringList InterpreterEditorManager::elements(QString const &editor, QString const &diagram) const
{
	QStringList result;
	Id id;
	QPair<qrRepo::RepoApi*, Id> repoAndDiagram = getRepoAndDiagram(editor, diagram);
	qrRepo::RepoApi const * const repo = repoAndDiagram.first;
	Id const diag = repoAndDiagram.second;
	foreach (Id element, repo->children(diag)) {
		result.append(repo->name(element));
	}
	return result;
}

int InterpreterEditorManager::isNodeOrEdge(QString const &editor, QString const &element) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndElement = getRepoAndElement(editor, element);
	Id const elem = repoAndElement.second;
	if (elem.element() == "MetaEntityEdge") {
		return -1;
	}
	if (elem.element() == "MetaEntityNode") {
		return 1;
	}
	return 0;
}

bool InterpreterEditorManager::isParentOf(QString const &editor, QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	if (childElement == parentElement && childDiagram == parentDiagram)
		return true;
	Id id;
	QPair<qrRepo::RepoApi*, Id> repoAndDiagram = getRepoAndDiagram(editor, childDiagram);
	qrRepo::RepoApi const * const repo = repoAndDiagram.first;
	Id const diagram = repoAndDiagram.second;
	foreach (Id element, repo->children(diagram)) {
		if (childElement == repo->name(element)) {
			return parentElement == repo->name(repo->parent(element));
		}
	}
	return false;
}

QString InterpreterEditorManager::diagramName(QString const &editor, QString const &diagram) const
{
	Id id;
	QPair<qrRepo::RepoApi*, Id> repoAndDiagram = getRepoAndDiagram(editor, diagram);
	return repoAndDiagram.first->stringProperty(repoAndDiagram.second, "displayedName");
}

QString InterpreterEditorManager::diagramNodeName(QString const &editor, QString const &diagram) const
{
	Id id;
	QPair<qrRepo::RepoApi*, Id> repoAndDiagram = getRepoAndDiagram(editor, diagram);
	return repoAndDiagram.first->stringProperty(repoAndDiagram.second, "nodeName");
}

//unsupported method
QStringList InterpreterEditorManager::paletteGroups(Id const &editor, Id const &diagram) const
{
	return QStringList();
}

QStringList InterpreterEditorManager::paletteGroupList(Id const &editor,Id const &diagram, QString const &group) const
{
	return QStringList();
}

QStringList InterpreterEditorManager::getReferenceProperties(const Id &id) const
{
	return QStringList();
}
