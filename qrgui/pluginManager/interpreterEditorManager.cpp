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

InterpreterEditorManager::InterpreterEditorManager(QString fileName, QObject *parent)
	: QObject(parent)
{
	//qrRepo::RepoApi *repo = new qrRepo::RepoApi("../metamodel/testMetamodel.qrs");
	qrRepo::RepoApi *repo = new qrRepo::RepoApi(fileName);
	mEditorRepoApi.insert("test", repo);
}

QPair<qrRepo::RepoApi*, Id> InterpreterEditorManager::getRepoAndMetaId(Id const &id) const{
	qrRepo::RepoApi *repoMetaModel = NULL;
	Id metaId;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id diagram, repo->children(editor)) {
				foreach (Id element, repo->children(diagram)) {
					if (id.element() == repo->name(element) && repo->isLogicalElement(element)) {
						repoMetaModel = repo;
						metaId = element;
					}
				}
			}
		}
	}
	return qMakePair(repoMetaModel, metaId);
}

IdList InterpreterEditorManager::editors() const
{
	IdList result;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id editor, repo->elementsByType("MetaEditorDiagramNode"))
			if (repo->isLogicalElement(editor))
				result << Id(repo->name(repo->parent(editor)), repo->name(editor));
	}
	return result;
}

IdList InterpreterEditorManager::diagrams(const Id &editor) const
{
	IdList result;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id edit, repo->elementsByType("MetamodelDiagram"))
			if (editor.editor() == repo->name(edit) && repo->isLogicalElement(edit))
				foreach (Id editChild, repo->children(edit))
					if (editChild.element() == "MetaEditorDiagramNode")
						result << Id(repo->name(edit), repo->name(editChild));

	return result;
}

IdList InterpreterEditorManager::elements(const Id &diagram) const
{
	IdList result;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id editor, repo->elementsByType("MetamodelDiagram"))
			foreach (Id editorChild, repo->children(editor))
				if (diagram.diagram() == repo->name(editorChild) && repo->isLogicalElement(editor))
					foreach (Id diagramChild, repo->children(editorChild)) {
						result << Id(repo->name(editor), repo->name(editorChild), repo->name(diagramChild));
					}

	return result;
}

QString InterpreterEditorManager::friendlyName(const Id &id) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id editor,  repo->elementsByType("MetamodelDiagram")) {
			if (id.editor() == repo->name(editor) && repo->isLogicalElement(editor)) {
				if (id.diagram() != "") {
					foreach (Id editorChild, repo->children(editor)) {
						if (editorChild.element() == "MetaEditorDiagramNode") {
							if(id.diagram() == repo->name(editorChild)) {
								if (id.element() != "") {
									foreach (Id diagramChild, repo->children(editorChild)) {
										if(id.element() == repo->name(diagramChild))
											return repo->stringProperty(diagramChild, "displayedName");
									}
								} else {
									return repo->stringProperty(editorChild, "displayedName");
								}
							}
						}
					}
				} else {
					return repo->stringProperty(editor, "displayedName");
				}
			}
		}
	}
	return "";
}

bool InterpreterEditorManager::hasElement(Id const &elementId) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id diagram, repo->children(editor)) {
				foreach (Id element, repo->children(diagram)) {
					if (elementId.element() == repo->name(element)) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

QString InterpreterEditorManager::propertyDescription(const Id &id, const QString &propertyName) const
{
	QPair<qrRepo::RepoApi*, Id> pairOfRepoAndMetaId = getRepoAndMetaId(id);
	return pairOfRepoAndMetaId.first->stringProperty(pairOfRepoAndMetaId.second, propertyName);
}

QStringList InterpreterEditorManager::getAllChildrenTypesOf(Id const &parent) const
{
	QStringList result;
	qrRepo::RepoApi *repoMetaModel = getRepoAndMetaId(parent).first;
	foreach (Id child, repoMetaModel->children(parent))
		result << child.element();
	return result;
}

bool InterpreterEditorManager::isParentOf(Id const &child, Id const &parent) const // child — EnginesForware, parent — AbstractNode
{
	qrRepo::RepoApi *repoMetaModelChild = getRepoAndMetaId(child).first;
	Id repoChild = getRepoAndMetaId(child).second;

	qrRepo::RepoApi *repoMetaModelParent = getRepoAndMetaId(parent).first;
	Id repoParent = getRepoAndMetaId(parent).second;

	if (repoMetaModelChild != repoMetaModelParent)
		return false;
	foreach(Id link ,repoMetaModelParent->links(parent)) {
		if (link.element() == "Container") {
			if((repoMetaModelChild->from(repoParent) == repoMetaModelChild->to(repoChild) || repoMetaModelChild->from(repoChild) == repoMetaModelChild->to(repoParent))) {
				return true;
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
	qrRepo::RepoApi *repo = getRepoAndMetaId(id).first;
	Id metaId = getRepoAndMetaId(id).second;
	foreach (Id property, repo->children(metaId))
		if (repo->name(property) == name)
			propertyType = repo->stringProperty(property, "attributeType");
	return propertyType;
}

//TODO:
QString InterpreterEditorManager::mouseGesture(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> pairOfRepoAndMetaId = getRepoAndMetaId(id);
	//return pairOfRepoAndMetaId.first->stringProperty(pairOfRepoAndMetaId.second, "path");
	return "";
}
//////////////////////////////////////////////////////////

QString InterpreterEditorManager::propertyDisplayedName(Id const &id, QString const &propertyName) const
{
	QString displayedName = "";
	qrRepo::RepoApi *repo = getRepoAndMetaId(id).first;
	Id metaId = getRepoAndMetaId(id).second;
	foreach (Id property, repo->children(metaId))
		if (repo->name(property) == propertyName)
			displayedName = repo->stringProperty(property, "displayedName");
	return displayedName;
}

QString InterpreterEditorManager::getDefaultPropertyValue(Id const &id, QString name) const
{
	QString defaultProperty = "";
	qrRepo::RepoApi *repo = getRepoAndMetaId(id).first;
	Id metaId = getRepoAndMetaId(id).second;
	foreach (Id property, repo->children(metaId)) {
		if (repo->name(property) == name) {
			defaultProperty = repo->stringProperty(property, "defaultValue");
		}
	}
	return defaultProperty;
}

Id InterpreterEditorManager::findElementByType(QString const &type) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id editor, repo->elementsByType("MetamodelDiagram"))
			foreach (Id diagram, repo->children(editor))
				foreach (Id element, repo->children(diagram))
					if (type == repo->name(element))
						return Id(editor.id(), repo->name(diagram), repo->name(element));
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

//TODO:
QString InterpreterEditorManager::description(Id const &id) const
{
	return "";
}

QIcon InterpreterEditorManager::icon(Id const &id) const
{
	qrRepo::RepoApi *repoMetaModel = getRepoAndMetaId(id).first;
	Id metaId = getRepoAndMetaId(id).second;
	QDomDocument classDoc;
	QDomElement sdfElement;
	if(metaId.element() == "MetaEntityEdge") {
		sdfElement = classDoc.createElement("picture");
		sdfElement.setAttribute("sizex", 100);
		sdfElement.setAttribute("sizey", 60);
		QDomElement lineElement = classDoc.createElement("line");
		lineElement.setAttribute("fill", "#000000");
		QString lineType = repoMetaModel->stringProperty(metaId, "lineType").remove("Line");
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
		graphics.setContent(repoMetaModel->stringProperty(metaId, "shape"));
		sdfElement = graphics.firstChildElement("graphics").firstChildElement("picture");
		OutFile out1("sdfElement.txt");
		sdfElement.save(out1(), 1);
	}
	if (sdfElement.isNull())
		return QIcon();
	classDoc.appendChild(classDoc.importNode(sdfElement, true));
	OutFile out2("classDoc.txt");
	classDoc.save(out2(), 1);
	SdfIconEngineV2 *engine = new SdfIconEngineV2(classDoc);
	return QIcon(engine);
}

Element* InterpreterEditorManager::graphicalObject(Id const &id) const
{
	qrRepo::RepoApi *repoMetaModel = getRepoAndMetaId(id).first;
	Id metaId = getRepoAndMetaId(id).second;
	InterpreterElementImpl *impl = new InterpreterElementImpl(repoMetaModel, metaId, id);
	if( !impl ){
		qDebug() << "no impl";
		return 0;
	}
	if (impl->isNode())
		return new NodeElement(impl);

	return  new EdgeElement(impl);
}

IdList InterpreterEditorManager::getContainedTypes(const Id &id) const
{
	IdList containedTypes;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id editor, repo->elementsByType("MetamodelDiagram"))
			foreach (Id diagram, repo->children(editor))
				foreach (Id element, repo->children(diagram))
					if (repo->name(element) == "Container" && repo->isLogicalElement(element))
						if (repo->from(element) == id)
							containedTypes << repo->to(element);
	return containedTypes;
}

IdList InterpreterEditorManager::getUsedTypes(const Id &id) const
{
	IdList usedTypes;
	qrRepo::RepoApi *repo = getRepoAndMetaId(id).first;
	Id metaId = getRepoAndMetaId(id).second;
	foreach(Id child, repo->children(metaId))
		if (repo->name(child) == "MetaEntityUsage")
			usedTypes << repo->property(child, "type").value<Id>();
	return usedTypes;
}

QStringList InterpreterEditorManager::getEnumValues(Id const &id, const QString &name) const
{
	QStringList result;
	qrRepo::RepoApi *repo = getRepoAndMetaId(id).first;
	Id metaId = getRepoAndMetaId(id).second;
	foreach (Id property, repo->children(metaId))
		if (repo->name(property) == name)
			foreach(Id propertyChild, repo->children(property))
				result.append(repo->name(propertyChild));
	return result;
}

QStringList InterpreterEditorManager::getPropertyNames(Id const &id) const
{
	QStringList result;
	qrRepo::RepoApi *repo = getRepoAndMetaId(id).first;
	Id metaId = getRepoAndMetaId(id).second;
	foreach (Id child, repo->children(metaId))
		if (child.element() == "MetaEntity_Attribute")
			result << repo->name(child);
	return result;
}

QStringList InterpreterEditorManager::getPropertiesWithDefaultValues(Id const &id) const
{
	QStringList result;
	qrRepo::RepoApi *repo = getRepoAndMetaId(id).first;
	Id metaId = getRepoAndMetaId(id).second;
	foreach (Id property, repo->children(metaId))
		if (repo->hasProperty(property, "defaultName"))
			result.append(repo->name(property));
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

//TODO: ???
bool InterpreterEditorManager::isDiagramNode(Id const &id) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id editor, repo->elementsByType("MetamodelDiagram"))
			foreach (Id diagram, repo->children(editor))
				if (id.diagram() == repo->name(diagram) && repo->isLogicalElement(diagram))
					if (diagram.element() == "MetaEditorDiagramNode")
							return true;
	return false;
}

bool InterpreterEditorManager::isGraphicalElementNode(const Id &id) const
{
	return false;
}

//TODO: functional realization below
QList<QPair<QPair<QString, QString>, QPair<bool, QString> > > InterpreterEditorManager::getPossibleEdges(QString const &editor, QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id edit, repo->elementsByType("MetamodelDiagram")) {
			if (editor == repo->name(edit))
				foreach (Id diagram, repo->children(edit))
					foreach (Id elem, repo->children(diagram))
						if (element == repo->name(elem))
							foreach (Id prop, repo->children(elem))
								if (repo->name(prop) == "possibleEdge")
									foreach (Id propChild, repo->children(prop)) {
										bool bparam = false;
										if (repo->stringProperty(propChild, "directed") == "true")
											bparam = true;
										result.append(qMakePair(qMakePair(repo->stringProperty(propChild, "beginName"),repo->stringProperty(propChild, "endName")), qMakePair(bparam,repo->name(elem))));
									}
		}
	return result;
}

QStringList InterpreterEditorManager::elements(QString const &editor, QString const &diagram) const
{
	QStringList result;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id edit, repo->elementsByType("MetamodelDiagram")) {
			if (editor == repo->name(edit))
				foreach (Id editChild, repo->children(edit))
					if (diagram == repo->name(editChild))
						foreach (Id editChildChild, repo->children(editChild))
							result.append(repo->name(editChildChild));
		}
	return result;
}

int InterpreterEditorManager::isNodeOrEdge(QString const &editor, QString const &element) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id edit, repo->elementsByType("MetamodelDiagram")) {
			if (editor == repo->name(edit))
				foreach (Id diagram, repo->children(edit))
					foreach (Id elem, repo->children(diagram))
						if (element == repo->name(elem))
						{
							if (elem.element() == "MetaEntityEdge")
								return -1;
							else if (elem.element() == "MetaEntityNode")
								return 1;
						}
		}
	return 0;
}

bool InterpreterEditorManager::isParentOf(QString const &editor, QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	if (childElement == parentElement && childDiagram == parentDiagram)
		return true;

	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id edit, repo->elementsByType("MetamodelDiagram")) {
			if (editor == repo->name(edit))
				foreach (Id diagram, repo->children(edit))
					if (childDiagram == repo->name(diagram)) {
						foreach (Id element, repo->children(diagram))
							if (childElement == repo->name(element)) {
								if (parentElement == repo->name(repo->parent(element)))
									return true;
								else
									return false;
							}
					}
		}
	return false;
}

QString InterpreterEditorManager::diagramName(QString const &editor, QString const &diagram) const
{
	QString diagramName = "";
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id edit, repo->elementsByType("MetamodelDiagram"))
			if (editor == repo->name(edit) && repo->isLogicalElement(edit))
				foreach (Id editChild, repo->children(edit))
					if (diagram == repo->name(editChild)) {
						diagramName = repo->stringProperty(editChild, "displayedName");
					}
	return diagramName;
}

QString InterpreterEditorManager::diagramNodeName(QString const &editor, QString const &diagram) const
{
	QString diagramNodeName = "";
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id edit, repo->elementsByType("MetamodelDiagram"))
			if (editor == repo->name(edit) && repo->isLogicalElement(edit))
				foreach (Id editChild, repo->children(edit))
				{
					if (diagram == repo->name(editChild)) {
						diagramNodeName = repo->stringProperty(editChild, "nodeName");
					}
				}
	return diagramNodeName;
}
//unsupported method
QStringList InterpreterEditorManager::paletteGroups(Id const &editor, Id const &diagram) const
{
	return *new QStringList();
}

QStringList InterpreterEditorManager::paletteGroupList(Id const &editor,Id const &diagram, QString const &group) const
{
	return *new QStringList();
}

QStringList InterpreterEditorManager::getReferenceProperties(const Id &id) const
{
	return *new QStringList();
}
