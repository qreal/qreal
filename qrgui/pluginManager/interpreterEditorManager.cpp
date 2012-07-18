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
#include "beep.h"
#include "interpreterElementImpl.h"

using namespace qReal;

InterpreterEditorManager::InterpreterEditorManager(QString fileName, QObject *parent)
	: QObject(parent)
{
	//qrRepo::RepoApi *repo = new qrRepo::RepoApi("../metamodel/testMetamodel.qrs");
	qrRepo::RepoApi *repo = new qrRepo::RepoApi(fileName);
	mEditorRepoApi.insert("test", repo);
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
	QString property = "";
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id diagram, repo->children(editor)) {
				foreach (Id element, repo->children(diagram)) {
					if (id.element() == repo->name(element)) {
						property = repo->stringProperty(element, propertyName);
					}
				}
			}
		}
	}
	return property;
}

QStringList InterpreterEditorManager::getAllChildrenTypesOf(Id const &parent) const
{
	QStringList result;
	qrRepo::RepoApi *repoMetaModel;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id diagram, repo->children(editor)) {
				foreach (Id element, repo->children(diagram)) {
					if (parent.element() == repo->name(element)) {
						repoMetaModel = repo;
					}
				}
			}
		}
	}
	foreach (Id child, repoMetaModel->children(parent))
		result << repoMetaModel->typeName(child);
	return result;
}

bool InterpreterEditorManager::isParentOf(Id const &child, Id const &parent) const // child — EnginesForware, parent — AbstractNode
{
	qrRepo::RepoApi *repoMetaModelChild = NULL;
	Id repoChild;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id diagram, repo->children(editor)) {
				foreach (Id element, repo->children(diagram)) {
					if (child.element() == repo->name(element)) {
						repoChild = element;
						repoMetaModelChild = repo;
					}
				}
			}
		}
	}
	Id repoParent;
	qrRepo::RepoApi *repoMetaModelParent = NULL;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id diagram, repo->children(editor)) {
				foreach (Id element, repo->children(diagram)) {
					if (parent.element() == repo->name(element)) {
						repoParent = element;
						repoMetaModelParent = repo;
					}
				}
			}
		}
	}
	if (repoMetaModelChild != repoMetaModelParent)
		return false;
	foreach(Id link ,repoMetaModelParent->links(parent)) {
		if (repoMetaModelChild->typeName(link) == "Container") {
			if((repoMetaModelChild->from(repoParent) == repoMetaModelChild->to(repoChild) || repoMetaModelChild->from(repoChild) == repoMetaModelChild->to(repoParent))) {
				return true;
			}
		}
	}
	return false;
}

IdList InterpreterEditorManager::getConnectedTypes(const Id &id) const
{
	qrRepo::RepoApi *repoMetaModel;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		if (repo->exist(id))
			repoMetaModel = repo;
	return repoMetaModel->connectedElements(id);
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
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		if (repo->exist(id))
			propertyType = repo->property(id, name).typeName();
	return propertyType;
}

QString InterpreterEditorManager::mouseGesture(Id const &id) const
{
	QString mouseGes = "";
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		if (repo->exist(id))
			mouseGes = repo->stringProperty(id, "path");
	return mouseGes;
}
////////////////////////////////////////////////////////////

//QIcon InterpreterEditorManager::icon(const Id &id) const
//{
//	SdfIconEngineV2 *engine = new SdfIconEngineV2(":/generated/shapes/" + id.element() + "Class.sdf");
//	// QIcon will take ownership of engine, no need for us to delete
//	return QIcon(engine);
//}

QString InterpreterEditorManager::propertyDisplayedName(Id const &id, QString const &propertyName) const
{
	QString displayedName = "";
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
			foreach (Id editor, repo->elementsByType("MetamodelDiagram"))
				foreach (Id diagram, repo->children(editor))
					foreach (Id element, repo->children(diagram))
						if (element == id)
							foreach (Id child, repo->children(element))
								if (repo->name(child) == propertyName)
									displayedName = repo->stringProperty(child, "displayedName");
	return displayedName;
}

QString InterpreterEditorManager::getDefaultPropertyValue(Id const &id, QString name) const
{
	QString defaultProperty;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		if (repo->exist(id))
			foreach(Id child, repo->children(id)) {
				if (repo->name(child) == name)
					defaultProperty = repo->stringProperty(child, "defaultValue");
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
	throw Exception("No type " + type + " in loaded plugins");
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
//TODO:
QIcon InterpreterEditorManager::icon(Id const &id) const
{
	return QIcon();
}

Element* InterpreterEditorManager::graphicalObject(Id const &id) const
{
	qrRepo::RepoApi *repoMetaModel = NULL;
	Id metaId;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id diagram, repo->children(editor)) {
				foreach (Id element, repo->children(diagram)) {
					QString idElem = id.element();
					QString repoElem = repo->name(element);
					if (id.element() == repo->name(element)) {
						repoMetaModel = repo;
						metaId = element;
					}
				}
			}
		}
	}

	InterpreterElementImpl *impl = new InterpreterElementImpl(repoMetaModel, metaId);
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
					if (repo->name(element) == "Container")
						if (repo->from(element) == id)
							containedTypes << repo->to(element);
	return containedTypes;
}

IdList InterpreterEditorManager::getUsedTypes(const Id &id) const
{
	IdList usedTypes;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id editor, repo->elementsByType("MetamodelDiagram"))
			foreach (Id diagram, repo->children(editor))
				foreach (Id element, repo->children(diagram))
					if (element == id)
						foreach(Id child, repo->children(element))
							if (repo->name(child) == "MetaEntityUsage")
								usedTypes << repo->property(child, "type").value<Id>();
	return usedTypes;
}

QStringList InterpreterEditorManager::getEnumValues(Id const &id, const QString &name) const
{
	QStringList result;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id editor, repo->elementsByType("MetamodelDiagram"))
			foreach (Id diagram, repo->children(editor))
				foreach (Id element, repo->children(diagram))
					if (repo->name(element) == id.element())
						foreach (Id prop, repo->children(element))
							if (repo->name(prop) == name)
								foreach(Id propChild, repo->children(prop))
									result.append(repo->name(propChild));
	return result;
}

QStringList InterpreterEditorManager::getPropertyNames(Id const &id) const
{
	QStringList result;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
			foreach (Id editor, repo->elementsByType("MetamodelDiagram"))
				foreach (Id diagram, repo->children(editor))
					foreach (Id element, repo->children(diagram))
						if (element == id)
							foreach (Id child, repo->children(element))
								if (repo->name(child) == "MetaEntity Attribute")
									result << repo->stringProperty(id, "displayedName");
	return result;
}

//TODO:
QStringList InterpreterEditorManager::getPropertiesWithDefaultValues(Id const &id) const
{
	QStringList result;
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
			foreach (Id editor, repo->elementsByType("MetamodelDiagram"))
				foreach (Id diagram, repo->children(editor))
					foreach (Id element, repo->children(diagram))
						if (element == id)
							foreach (Id prop, repo->children(element))
								if(repo->hasProperty(prop, "defaultName"))
									result.append(repo->name(prop));
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
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values())
		foreach (Id editor, repo->elementsByType("MetamodelDiagram"))
			foreach (Id diagram, repo->children(editor))
				foreach (Id element, repo->children(diagram))
					if (element == id)
						if (repo->stringProperty(diagram, "nodeName") == repo->name(id))
							return true;
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
										if(repo->stringProperty(propChild, "directed") == "true")
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
						if(element == repo->name(elem))
						{
							if(repo->typeName(elem) == "MetaEntityEdge")
								return -1;
							else if(repo->typeName(elem) == "MetaEntityNode")
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
							if(childElement == repo->name(element)) {
								if(parentElement == repo->name(repo->parent(element)))
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
