#include <QtCore/QCoreApplication>
#include <QtCore/QtDebug>
#include <QtCore/QUuid>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>

#include "interpreterEditorManager.h"
#include "../../qrkernel/ids.h"
#include "../../qrrepo/repoApi.h"
#include "../umllib/nodeElement.h"
#include "../umllib/edgeElement.h"
#include "../../qrkernel/exception/exception.h"
#include "interpreterElementImpl.h"
#include "../../qrutils/outFile.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;
using namespace utils;

InterpreterEditorManager::InterpreterEditorManager(QString const &fileName, QObject *parent)
	: QObject(parent)
	, mMetamodelFile(fileName)
{
	qrRepo::RepoApi * const repo = new qrRepo::RepoApi(fileName);
	mEditorRepoApi.insert("test", repo);
}

Id InterpreterEditorManager::getElement(Id const &id, qrRepo::RepoApi const * const repo, Id const &diagram) const
{
	foreach (Id const &element, repo->children(diagram)) {
		if (id.element() == repo->name(element) && repo->isLogicalElement(element)) {
			return element;
		}
	}
	return Id();
}

Id InterpreterEditorManager::getDiagramOrElement(Id const &id, qrRepo::RepoApi const * const repo, Id const &editor) const
{
	foreach (Id const &diagram, repo->children(editor)) {
		if (diagram.element() == "MetaEditorDiagramNode" && id.diagram() == repo->name(diagram)
			&& repo->isLogicalElement(diagram)) {
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
		foreach (Id const &editor,  repo->elementsByType("MetamodelDiagram")) {
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
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id const &edit, repo->elementsByType("MetamodelDiagram")) {
			if (editor.editor() == repo->name(edit) && repo->isLogicalElement(edit)) {
				foreach (Id const &diagram, repo->children(edit)) {
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
		foreach (Id const &editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id const &editorChild, repo->children(editor)) {
				if (diagram.diagram() == repo->name(editorChild) && repo->isLogicalElement(editorChild)) {
					foreach (Id const &diagramChild, repo->children(editorChild)) {
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
	foreach (Id const &child, repoAndMetaId.first->children(repoAndMetaId.second)) {
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
				QPair<Id, Id> editorAndDiagram = getEditorAndDiagram(repoMetaModelChild, metaChildParent);
				return isParentOf(Id(repoMetaModelChild->name(editorAndDiagram.first),
							 repoMetaModelChild->name(editorAndDiagram.second), repoMetaModelChild->name(metaChildParent)), parent);
			}
		}
	}
	return false;
}

IdList InterpreterEditorManager::getConnectedTypes(Id const &id) const
{
	IdList result;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	foreach (Id const &connectId, repo->connectedElements(metaId)) {
		QPair<Id, Id> editorAndDiagram = getEditorAndDiagram(repo, connectId);
		result << Id(repo->name(editorAndDiagram.first), repo->name(editorAndDiagram.second), repo->name(connectId));
	}
	return result;
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

QString InterpreterEditorManager::mouseGesture(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	if (repoAndMetaId.first->hasProperty(repoAndMetaId.second, "path")) {
		return repoAndMetaId.first->stringProperty(repoAndMetaId.second, "path");
	}
	return "";
}

class InterpreterEditorManager::CheckPropertyForParent {
public:
	virtual QString strProperty(qrRepo::RepoApi const * const repo, Id const &parentProperty,
				QString const &propertyName) const = 0;
};

class InterpreterEditorManager::CompareProperty: public InterpreterEditorManager::CheckPropertyForParent {
public:
	CompareProperty(QString value): mValue(value){}
	virtual QString strProperty(qrRepo::RepoApi const * const repo, Id const &parentProperty,
								QString const &propertyName) const {
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
	virtual QString strProperty(qrRepo::RepoApi const * const repo, Id const &parentProperty,
								QString const &propertyName) const {
		if (parentProperty.element() == propertyName) {
			return repo->name(parentProperty);
		}
		return "";
	}
};

class InterpreterEditorManager::HasProperty: public InterpreterEditorManager::CheckPropertyForParent {
public:
	virtual QString strProperty(qrRepo::RepoApi const * const repo, Id const &parentProperty,
								QString const &propertyName) const {
		if (repo->hasProperty(parentProperty, propertyName)) {
			return repo->name(parentProperty);
		}
		return "";
	}
};

QStringList InterpreterEditorManager::getPropertiesFromParents(Id const &id, QString const &propertyName, CheckPropertyForParent const &checker) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	foreach (Id const &link , repo->incomingLinks(metaId)) {
		if (link.element() == "Inheritance") {
			foreach (Id const &parentProperty, repo->children(repo->otherEntityFromLink(link, metaId))) {
				QString const strProperty = checker.strProperty(repo, parentProperty, propertyName);
				if (!strProperty.isEmpty()) {
					result << strProperty;
				}
			}
			Id const metaChildParent = repo->otherEntityFromLink(link, metaId);
			QPair<Id, Id> editorAndDiagram = getEditorAndDiagram(repo, metaChildParent);
			result << getPropertiesFromParents(Id(repo->name(editorAndDiagram.first), repo->name(editorAndDiagram.second), repo->name(metaChildParent)), propertyName, checker);
		}
	}
	return result;
}

QString InterpreterEditorManager::getValueOfProperty(Id const &id, QString const &propertyName, QString const &value) const
{
	QString valueOfProperty = "";
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	QStringList parentsProperties =  getPropertiesFromParents(id, propertyName, CompareProperty(value));
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
	return getValueOfProperty(id, propertyName, "displayedName");
}

QString InterpreterEditorManager::getDefaultPropertyValue(Id const &id, QString name) const
{
	return getValueOfProperty(id, name, "defaultValue");
}

QString InterpreterEditorManager::getTypeName(const Id &id, const QString &name) const
{
	return getValueOfProperty(id, name, "attributeType");
}

Id InterpreterEditorManager::findElementByType(QString const &type) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
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
	return true;
}

bool InterpreterEditorManager::unloadPlugin(QString const &pluginName)
{
	return true;
}

QString InterpreterEditorManager::description(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	if (repoAndMetaId.first->hasProperty(repoAndMetaId.second, "description")) {
		return repoAndMetaId.first->stringProperty(repoAndMetaId.second, "description");
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
	return new EdgeElement(impl);
}

IdList InterpreterEditorManager::getContainedTypes(const Id &id) const
{
	IdList containedTypes;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	foreach (Id const &link, repo->outgoingLinks(metaId)) {
		if (link.element() == "Container") {
			Id metaIdTo = repo->otherEntityFromLink(link, metaId);
			QPair<Id, Id> editorAndDiagram = getEditorAndDiagram(repo, metaIdTo);
			containedTypes << Id(repo->name(editorAndDiagram.first),
					 repo->name(editorAndDiagram.second), repo->name(metaIdTo));
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
	foreach (Id const &child, repo->children(metaId)) {
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
	foreach (Id const &property, repo->children(metaId)) {
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
	QStringList propertiesFromParents = getPropertiesFromParents(id, "MetaEntity_Attribute", GetProperty());
	if (!propertiesFromParents.isEmpty()) {
		result << propertiesFromParents;
	}
	foreach (Id const &idProperty, repo->children(metaId)) {
		if (idProperty.element() == "MetaEntity_Attribute") {
			result << repo->name(idProperty);
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
	QStringList parentsPropertiesWithDefaultValues = getPropertiesFromParents(id, "defaultName", HasProperty());
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
	return IdList();
}

QList<ListenerInterface *> InterpreterEditorManager::listeners() const
{
	return QList<ListenerInterface *>();
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
		foreach (Id const &edit,  repo->elementsByType("MetamodelDiagram")) {
			if (editor == repo->name(edit) && repo->isLogicalElement(edit)) {
				foreach (Id const &diagram, repo->children(edit)) {
					foreach (Id const &elem, repo->children(diagram)) {
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
		foreach (Id const &edit,  repo->elementsByType("MetamodelDiagram")) {
			if (editor == repo->name(edit) && repo->isLogicalElement(edit)) {
				foreach (Id const &editChild, repo->children(edit)) {
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

QList<QPair<QPair<QString, QString>, QPair<bool, QString> > > InterpreterEditorManager::getPossibleEdges(QString const &editor, QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	QPair<qrRepo::RepoApi*, Id> repoAndElement = getRepoAndElement(editor, element);
	qrRepo::RepoApi const * const repo = repoAndElement.first;
	Id const elem = repoAndElement.second;
	foreach (Id const &prop, repo->children(elem)) {
		if (repo->name(prop) == "possibleEdge") {
			foreach (Id const &propChild, repo->children(prop)) {
				bool bparam = false;
				if (repo->stringProperty(propChild, "directed") == "true") {
					bparam = true;
				}
				result.append(qMakePair(qMakePair(repo->stringProperty(propChild, "beginName"),
						repo->stringProperty(propChild, "endName")), qMakePair(bparam,repo->name(elem))));
			}
		}
	}
	return result;
}

QStringList InterpreterEditorManager::elements(QString const &editor, QString const &diagram) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> repoAndDiagram = getRepoAndDiagram(editor, diagram);
	qrRepo::RepoApi const * const repo = repoAndDiagram.first;
	Id const diag = repoAndDiagram.second;
	foreach (Id const &element, repo->children(diag)) {
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
	return isParentOf(Id(editor, childDiagram, childElement),
					  Id(editor, parentDiagram, parentElement));
}

QString InterpreterEditorManager::diagramName(QString const &editor, QString const &diagram) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndDiagram = getRepoAndDiagram(editor, diagram);
	return repoAndDiagram.first->stringProperty(repoAndDiagram.second, "displayedName");
}

QString InterpreterEditorManager::diagramNodeName(QString const &editor, QString const &diagram) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndDiagram = getRepoAndDiagram(editor, diagram);
	return repoAndDiagram.first->stringProperty(repoAndDiagram.second, "nodeName");
}

bool InterpreterEditorManager::isInterpretationMode() const
{
	return true;
}

bool InterpreterEditorManager::isParentProperty(Id const &id, QString const &propertyName) const
{
	QStringList propertiesFromParents = getPropertiesFromParents(id, "MetaEntity_Attribute", GetProperty());
	return propertiesFromParents.contains(propertyName);
}

void InterpreterEditorManager::deletePropertyInElement(qrRepo::RepoApi *repo, Id const &editor,
										Id const &diagram, QString const &propDisplayedName) const
{
	foreach (Id const &element, repo->children(diagram)) {
		foreach (Id const &property, repo->children(element)) {
			if (property.element() == "MetaEntity_Attribute") {
				Id const &elementModel = Id(repo->name(editor), repo->name(diagram), repo->name(element));
				if (propertyDisplayedName(elementModel, repo->name(property)) == propDisplayedName) {
					repo->removeChild(element, property);
				}
			}
		}
	}
}

void InterpreterEditorManager::deleteProperty(QString const &propDisplayedName) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (Id const &editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (Id const &diagram, repo->children(editor)) {
				if (repo->isLogicalElement(diagram)) {
					deletePropertyInElement(repo, editor, diagram, propDisplayedName);
				}
			}
		}
	}
}

void InterpreterEditorManager::addProperty(Id const &id, QString const &propDisplayedName) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	Id newId = Id(repoAndMetaId.second.editor(), repoAndMetaId.second.diagram(),
		"MetaEntity_Attribute", QUuid::createUuid().toString());
	repoAndMetaId.first->addChild(repoAndMetaId.second, newId);
	repoAndMetaId.first->setProperty(newId, "name", propDisplayedName);
	repoAndMetaId.first->setProperty(newId, "displayedName", propDisplayedName);
}

void InterpreterEditorManager::setProperty(qrRepo::RepoApi* repo, Id const &id,
		QString const &property, QVariant const &propertyValue) const
{
	repo->setProperty(id, property, propertyValue);
}

void InterpreterEditorManager::updateProperties(Id const &id, QString const &property, QString const &propertyType,
		QString const &propertyDefaultValue, QString const &propertyDisplayedName) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	Id propertyMetaId;
	foreach (Id prop, repoAndMetaId.first->children(repoAndMetaId.second)) {
		if (prop.element() == "MetaEntity_Attribute" && repoAndMetaId.first->name(prop) == property) {
			propertyMetaId = prop;
		}
	}
	setProperty(repoAndMetaId.first, propertyMetaId, "attributeType", propertyType);
	setProperty(repoAndMetaId.first, propertyMetaId, "defaultValue", propertyDefaultValue);
	setProperty(repoAndMetaId.first, propertyMetaId, "displayedName", propertyDisplayedName);
}

QString InterpreterEditorManager::getPropertyNameByDisplayedName(Id const &id, QString const &displayedPropertyName) const
{
	QString propertyName = "";
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	foreach (Id const &property, repoAndMetaId.first->children(repoAndMetaId.second)) {
		if (property.element() == "MetaEntity_Attribute"
			&& repoAndMetaId.first->stringProperty(property, "displayedName") == displayedPropertyName) {
			propertyName = repoAndMetaId.first->name(property);
		}
	}
	return propertyName;
}

IdList InterpreterEditorManager::getChildren(Id const &parent) const {
	IdList result;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(parent);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	foreach (Id const &link , repo->outgoingLinks(metaId)) {
		if (link.element() == "Inheritance") {
			Id metaChild = repo->otherEntityFromLink(link, metaId);
			QPair<Id, Id> editorAndDiagram = getEditorAndDiagram(repo, metaChild);
			Id child = Id(repo->name(editorAndDiagram.first), repo->name(editorAndDiagram.second), repo->name(metaChild));
			result << child;
			result << getChildren(child);
		}
	}
	return result;
}

QString InterpreterEditorManager::getShape(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	if (metaId.element() == "MetaEntityNode") {
		return repo->stringProperty(metaId, "shape");
	}
	return "";
}

void InterpreterEditorManager::updateShape(Id const &id, QString const &graphics) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	if (repoAndMetaId.second.element() == "MetaEntityNode") {
		repoAndMetaId.first->setProperty(repoAndMetaId.second, "shape", graphics);
	}
}

void InterpreterEditorManager::deleteElement(MainWindow *mainWindow, Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	IdList logicalIdList = mainWindow->models()->logicalRepoApi().logicalElements(id.type());
	foreach (Id const &logicalId, logicalIdList) {
		QModelIndex index = mainWindow->models()->logicalModelAssistApi().indexById(logicalId);
			mainWindow->models()->logicalModel()->removeRow(index.row(), index.parent());
	}
	repo->removeChild(repo->parent(metaId), metaId);
	repo->removeElement(metaId);
}

bool InterpreterEditorManager::isRootDiagramNode(Id const &id) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId = getRepoAndMetaId(id);
	qrRepo::RepoApi const * const repo = repoAndMetaId.first;
	Id const metaId = repoAndMetaId.second;
	QPair<Id, Id> editorAndDiagram = getEditorAndDiagram(repo, metaId);
	return repo->stringProperty(editorAndDiagram.second, "nodeName") == repo->name(metaId);
}

void InterpreterEditorManager::setStandartConfigurations(qrRepo::RepoApi *repo, Id const &id, Id const &parent, const QString &name) const
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
	QString const shape = "<graphics>\n    <picture sizex=\"168\" sizey=\"111\">\n    <rectangle fill=\"#ffffff\" stroke-style=\"solid\" stroke=\"#000000\" y1=\"0\" stroke-width=\"0\" x1=\"1\" y2=\"107\" fill-style=\"none\" x2=\"125\"/>\n    </picture>\n    <labels>\n    <label x=\"41\" y=\"43\" textBinded=\"name\"/>\n    </labels>\n    <ports>\n    <pointPort x=\"2\" y=\"52\"/>\n    <pointPort x=\"134\" y=\"56\"/>\n    <linePort>\n    <start startx=\"29\" starty=\"110\"/>\n    <end endx=\"95\" endy=\"111\"/>\n    </linePort>\n    </ports>\n</graphics>\n";
	QPair<qrRepo::RepoApi*, Id> repoAndDiagram = getRepoAndDiagram(diagram.editor(), diagram.diagram());
	qrRepo::RepoApi *repo = repoAndDiagram.first;
	Id const diag = repoAndDiagram.second;
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

void InterpreterEditorManager::addEdgeElement(Id const &diagram, QString const &name, QString const &labelText, QString const &labelType,
					QString const &lineType, QString const &beginType, QString const &endType) const
{
	QPair<qrRepo::RepoApi*, Id> repoAndDiagram = getRepoAndDiagram(diagram.editor(), diagram.diagram());
	qrRepo::RepoApi * const repo = repoAndDiagram.first;
	Id const diag = repoAndDiagram.second;
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

	repo->setProperty(associationId, "name", name + "Association");
	repo->setProperty(associationId, "beginType", beginType);
	repo->setProperty(associationId, "endType", endType);
}

QPair<Id, Id> InterpreterEditorManager::createEditorAndDiagram(QString const &name) const
{
	Id editor("MetaEditor", "MetaEditor", "MetamodelDiagram",
						QUuid::createUuid().toString());
	Id diagram("MetaEditor", "MetaEditor", "MetaEditorDiagramNode",
			QUuid::createUuid().toString());
	qrRepo::RepoApi * const repo = mEditorRepoApi.value("test");
	repo->addChild(Id::rootId(), editor);
	repo->setProperty(editor, "name", name);
	repo->setProperty(editor, "displayedName", name);
	repo->addChild(editor, diagram);
	repo->setProperty(diagram, "name", name);
	repo->setProperty(diagram, "displayedName", name);
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
	return qMakePair(Id(repo->name(editor)),  Id(repo->name(editor), repo->name(diagram)));
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

QString InterpreterEditorManager::saveMetamodelFilePath() const {
	return mMetamodelFile;
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

QString InterpreterEditorManager::paletteGroupDescription(Id const &editor, const Id &diagram, const QString &group) const
{
	return "";
}
