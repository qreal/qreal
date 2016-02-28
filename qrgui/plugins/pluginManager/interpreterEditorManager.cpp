/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "interpreterEditorManager.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QtDebug>
#include <QtCore/QUuid>
#include <QtWidgets/QMessageBox>
#include <QtGui/QIcon>

#include <qrkernel/ids.h>
#include <qrkernel/exception/exception.h>
#include <qrrepo/repoApi.h>
#include <qrutils/outFile.h>

#include "details/interpreterElementImpl.h"
#include "editor/nodeElement.h"
#include "editor/edgeElement.h"

using namespace qReal;
using namespace utils;

InterpreterEditorManager::InterpreterEditorManager(const QString &fileName, QObject *parent)
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

Id InterpreterEditorManager::element(const Id &id, const qrRepo::RepoApi * const repo, const Id &diagram) const
{
	foreach (const Id &element, repo->children(diagram)) {
		if (id.element() == repo->name(element) && repo->isLogicalElement(element)) {
			return element;
		}
	}

	return Id();
}

Id InterpreterEditorManager::diagramOrElement(const Id &id, const qrRepo::RepoApi * const repo, const Id &editor) const
{
	foreach (const Id &diagram, repo->children(editor)) {
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

QPair<qrRepo::RepoApi*, Id> InterpreterEditorManager::repoAndMetaId(const Id &id) const
{
	foreach (qrRepo::RepoApi *repo, mEditorRepoApi.values()) {
		foreach (const Id &editor,  repo->elementsByType("MetamodelDiagram")) {
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
		foreach (const Id &editor, repo->elementsByType("MetaEditorDiagramNode")) {
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
	foreach (const qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
		foreach (const Id &edit, repo->elementsByType("MetamodelDiagram")) {
			if (editor.editor() != repo->name(edit) || !repo->isLogicalElement(edit)) {
				continue;
			}

			foreach (const Id &diagram, repo->children(edit)) {
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
		foreach (const Id &editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (const Id &editorChild, repo->children(editor)) {
				if (diagram.diagram() != repo->name(editorChild) || !repo->isLogicalElement(editorChild)) {
					continue;
				}

				foreach (const Id &diagramChild, repo->children(editorChild)) {
					if (diagramChild.element() == "MetaEntityEdge") {
						if (repo->hasProperty(diagramChild,"abstract")) {
							if (repo->stringProperty(diagramChild, "abstract") == "true") {
								repo->setProperty(diagramChild, "isHidden", "true");
							}
						}

						if (!repo->hasProperty(diagramChild, "isHidden")) {
							repo->setProperty(diagramChild, "isHidden", "false");
						}

						if (repo->stringProperty(diagramChild, "isHidden") != "true") {
							result << Id(repo->name(editor), repo->name(editorChild), repo->name(diagramChild));
						}
					} else if (repo->hasProperty(diagramChild, "shape")) {
						if (!repo->stringProperty(diagramChild, "shape").isEmpty()) {
							if (repo->hasProperty(diagramChild,"abstract")) {
								if (repo->stringProperty(diagramChild, "abstract") == "true") {
									repo->setProperty(diagramChild, "isHidden", "true");
								}
							}

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
	}

	return result;
}

Version InterpreterEditorManager::version(const Id &editor) const
{
	/// @todo: Support metamodel versions
	Q_UNUSED(editor)
	return Version();
}

QString InterpreterEditorManager::friendlyName(const Id &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	if (repoAndMetaIdPair.first) {
		if (repoAndMetaIdPair.first->hasProperty(repoAndMetaIdPair.second, "displayedName")
				&& !repoAndMetaIdPair.first->stringProperty(repoAndMetaIdPair.second, "displayedName").isEmpty())
		{
			return repoAndMetaIdPair.first->stringProperty(repoAndMetaIdPair.second, "displayedName");
		}

		return repoAndMetaIdPair.first->name(repoAndMetaIdPair.second);
	}

	return QString();
}

bool InterpreterEditorManager::hasElement(const Id &elementId) const
{
	return repoAndMetaId(elementId).first != nullptr;
}

QString InterpreterEditorManager::propertyDescription(const Id &id, const QString &propertyName) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	return repoAndMetaIdPair.first->stringProperty(repoAndMetaIdPair.second, propertyName);
}

QStringList InterpreterEditorManager::allChildrenTypesOf(const Id &parent) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(parent);
	foreach (const Id &child, repoAndMetaIdPair.first->children(repoAndMetaIdPair.second)) {
		result << child.element();
	}

	return result;
}

QList<Explosion> InterpreterEditorManager::explosions(const Id &source) const
{
	// TODO: implement me
	Q_UNUSED(source)
	return QList<Explosion>();
}

bool InterpreterEditorManager::isParentOf(const Id &child, const Id &parent) const
{
	const qrRepo::RepoApi * const repoMetaModelChild = repoAndMetaId(child).first;
	const Id repoChild = repoAndMetaId(child).second;

	const qrRepo::RepoApi * const repoMetaModelParent = repoAndMetaId(parent).first;
	const Id repoParent = repoAndMetaId(parent).second;

	if (repoChild == repoParent && repoMetaModelChild == repoMetaModelParent) {
		return true;
	}

	if (repoMetaModelChild != repoMetaModelParent) {
		return false;
	}

	foreach (const Id &link , repoMetaModelChild->incomingLinks(repoChild)) {
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

QString InterpreterEditorManager::mouseGesture(const Id &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	if (repoAndMetaIdPair.first->hasProperty(repoAndMetaIdPair.second, "path")) {
		return repoAndMetaIdPair.first->stringProperty(repoAndMetaIdPair.second, "path");
	}

	return "";
}


/// @todo Replace this with lambdas.
class InterpreterEditorManager::CheckPropertyForParent
{
public:
	virtual ~CheckPropertyForParent() {}
	virtual QString stringProperty(const qrRepo::RepoApi * const repo, const Id &parentProperty
			, const QString &propertyName) const = 0;
};

class InterpreterEditorManager::CompareProperty: public InterpreterEditorManager::CheckPropertyForParent
{
public:
	CompareProperty(QString value)
			: mValue(value)
	{
	}

	virtual QString stringProperty(const qrRepo::RepoApi * const repo, const Id &parentProperty
			, const QString &propertyName) const
	{
		if (repo->name(parentProperty) == propertyName) {
			return repo->stringProperty(parentProperty, mValue);
		}

		return "";
	}

private:
	QString mValue;
};

class InterpreterEditorManager::GetProperty: public InterpreterEditorManager::CheckPropertyForParent
{
public:
	virtual QString stringProperty(const qrRepo::RepoApi * const repo, const Id &parentProperty
			, const QString &propertyName) const
	{
		if (parentProperty.element() == propertyName) {
			return repo->name(parentProperty);
		}

		return "";
	}
};

class InterpreterEditorManager::HasProperty: public InterpreterEditorManager::CheckPropertyForParent
{
public:
	virtual QString stringProperty(const qrRepo::RepoApi * const repo, const Id &parentProperty
			, const QString &propertyName) const
	{
		if (repo->hasProperty(parentProperty, propertyName)) {
			return repo->name(parentProperty);
		}

		return "";
	}
};

QStringList InterpreterEditorManager::propertiesFromParents(const Id &id
		, const QString &propertyName, const CheckPropertyForParent &checker) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id &metaId = repoAndMetaIdPair.second;
	foreach (const Id &link , repo->incomingLinks(metaId)) {
		if (link.element() == "Inheritance") {
			foreach (const Id &parentProperty, repo->children(repo->otherEntityFromLink(link, metaId))) {
				if (!repo->hasProperty(parentProperty, "isHidden")) {
					repo->setProperty(parentProperty, "isHidden", "false");
				}

				if (repo->stringProperty(parentProperty, "isHidden") == "false") {
					const QString strProperty = checker.stringProperty(repo, parentProperty, propertyName);
					if (!strProperty.isEmpty()) {
						result << strProperty;
					}
				}
			}

			const Id metaChildParent = repo->otherEntityFromLink(link, metaId);
			if (metaChildParent != Id::rootId()) {
				QPair<Id, Id> const editorAndDiagramPair = editorAndDiagram(repo, metaChildParent);
				result << propertiesFromParents(Id(repo->name(editorAndDiagramPair.first)
						, repo->name(editorAndDiagramPair.second), repo->name(metaChildParent)), propertyName, checker);
			}
		}
	}

	return result;
}

QString InterpreterEditorManager::valueOfProperty(const Id &id, const QString &propertyName, const QString &value) const
{
	QString valueOfProperty = "";
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;
	const QStringList parentsProperties =  propertiesFromParents(id, propertyName, CompareProperty(value));
	if (!parentsProperties.isEmpty()) {
		valueOfProperty = parentsProperties.first();
	}

	foreach (const Id &property, repo->children(metaId)) {
		if (repo->name(property) == propertyName) {
			if (repo->hasProperty(property, value)) {
				valueOfProperty = repo->stringProperty(property, value);
			}
		}
	}

	return valueOfProperty;
}

QString InterpreterEditorManager::propertyDisplayedName(const Id &id, const QString &propertyName) const
{
	return valueOfProperty(id, propertyName, "displayedName");
}

QString InterpreterEditorManager::defaultPropertyValue(const Id &id, QString name) const
{
	return valueOfProperty(id, name, "defaultValue");
}

QString InterpreterEditorManager::typeName(const Id &id, const QString &name) const
{
	return valueOfProperty(id, name, "attributeType");
}

Id InterpreterEditorManager::findElementByType(const QString &type) const
{
	foreach (const qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
		foreach (const Id &editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (const Id &diagram, repo->children(editor)) {
				foreach (const Id &element, repo->children(diagram)) {
					if (type == repo->name(element)) {
						return Id(repo->name(editor), repo->name(diagram), repo->name(element));
					}
				}
			}
		}
	}

	throw Exception("No type " + type);
}

QString InterpreterEditorManager::loadPlugin(const QString &pluginName)
{
	Q_UNUSED(pluginName);
	return QString();
}

QString InterpreterEditorManager::unloadPlugin(const QString &pluginName)
{
	Q_UNUSED(pluginName);
	return QString();
}

QString InterpreterEditorManager::description(const Id &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	if (repoAndMetaIdPair.first->hasProperty(repoAndMetaIdPair.second, "description")) {
		return repoAndMetaIdPair.first->stringProperty(repoAndMetaIdPair.second, "description");
	}

	return "";
}

QIcon InterpreterEditorManager::icon(const Id &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;
	QDomDocument classDoc;
	QDomElement sdfElement;
	if (metaId.element() == "MetaEntityEdge") {
		sdfElement = classDoc.createElement("picture");
		sdfElement.setAttribute("sizex", 100);
		sdfElement.setAttribute("sizey", 60);
		QDomElement lineElement = classDoc.createElement("line");
		lineElement.setAttribute("fill", "#000000");
		const QString lineType = repo->stringProperty(metaId, "lineType").remove("Line");
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

ElementType &InterpreterEditorManager::elementType(const Id &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	InterpreterElementImpl * const type = new InterpreterElementImpl(repoAndMetaIdPair.first, repoAndMetaIdPair.second);
//	Q_ASSERT(type);
//	return *type;
}

IdList InterpreterEditorManager::containedTypes(const Id &id) const
{
	IdList containedTypes;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;
	foreach (const Id &link, repo->outgoingLinks(metaId)) {
		if (link.element() == "Container") {
			const Id metaIdTo = repo->otherEntityFromLink(link, metaId);
			QPair<Id, Id> const editorAndDiagramPair = editorAndDiagram(repo, metaIdTo);
			containedTypes << Id(repo->name(editorAndDiagramPair.first)
					, repo->name(editorAndDiagramPair.second), repo->name(metaIdTo));
		}
	}

	return containedTypes;
}

bool InterpreterEditorManager::isEnumEditable(const Id &id, const QString &name) const
{
	/// @todo: Support it
	Q_UNUSED(id)
	Q_UNUSED(name)
	return false;
}

QList<QPair<QString, QString>> InterpreterEditorManager::enumValues(const Id &id, const QString &name) const
{
	QList<QPair<QString, QString>> result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;

	IdList listOfEnum =  repo->elementsByType("MetaEntityEnum");
	for (const Id &currentEnum : listOfEnum) {
		for (const Id &property: repo->children(metaId)) {
			if (repo->name(property) == name && repo->isLogicalElement(property)) {
				const QString nameOfEnum = repo->name(currentEnum);
				const QString attributeType = repo->stringProperty(property, "attributeType");
				if (nameOfEnum == attributeType) {
					IdList children = repo->children(currentEnum);
					for (const Id &current : children) {
						if (repo->isLogicalElement(current)) {
							if (repo->hasProperty(current, "valueName")) {
								const QString valueName = repo->stringProperty(current, "valueName");
								const QString displayedName = repo->stringProperty(current, "displayedName");
								result.append(qMakePair(valueName, displayedName));
							}
						}
					}
				}
			}
		}
	}

	return result;
}

QStringList InterpreterEditorManager::propertyNames(const Id &id) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;
	const QStringList propertiesFromParentsList = propertiesFromParents(id, "MetaEntity_Attribute", GetProperty());
	if (!propertiesFromParentsList.isEmpty()) {
		result << propertiesFromParentsList;
	}

	foreach (const Id &idProperty, repo->children(metaId)) {
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

QStringList InterpreterEditorManager::portTypes(const Id &id) const
{
	Q_UNUSED(id);
	return QStringList("NonTyped");
}

QStringList InterpreterEditorManager::propertiesWithDefaultValues(const Id &id) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;
	const QStringList parentsPropertiesWithDefaultValues = propertiesFromParents(id, "defaultName", HasProperty());
	if (!parentsPropertiesWithDefaultValues.isEmpty()) {
		result << parentsPropertiesWithDefaultValues;
	}

	for (const Id &property : repo->children(metaId)) {
		if (repo->hasProperty(property, "defaultName")) {
			result << repo->name(property);
		}
	}

	for (const Id &property : repo->children(metaId)) {
		if (repo->hasProperty(property, "name")) {
			result << repo->name(property);
		}
	}

	return result;
}

bool InterpreterEditorManager::isDiagramNode(const Id &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	const Id diagram = repoAndMetaIdPair.second;
	return diagram.element() == "MetaEditorDiagramNode";
}

bool InterpreterEditorManager::isGraphicalElementNode(const Id &id) const
{
	Q_UNUSED(id);
	return false;
}

Id InterpreterEditorManager::theOnlyDiagram() const
{
	const IdList allEditors(editors());
	return (allEditors.length() == 1 && diagrams(allEditors[0]).length() == 1)
			? diagrams(allEditors[0])[0] : Id();
}

QString InterpreterEditorManager::diagramNodeNameString(const Id &editor, const Id &diagram) const
{
	const QString diagramNodeNameString = diagramNodeName(editor.editor(), diagram.diagram());
	return QString("qrm:/%1/%2/%3").arg(editor.editor(), diagram.diagram(), diagramNodeNameString);
}

QPair<qrRepo::RepoApi*, Id> InterpreterEditorManager::repoAndElement(const QString &editor
		, const QString &element) const
{
	foreach (qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
		foreach (const Id &edit,  repo->elementsByType("MetamodelDiagram")) {
			if (editor != repo->name(edit) || !repo->isLogicalElement(edit)) {
				continue;
			}

			foreach (const Id &diagram, repo->children(edit)) {
				foreach (const Id &elem, repo->children(diagram)) {
					if (element == repo->name(elem)) {
						return qMakePair(repo, elem);
					}
				}
			}
		}
	}

	return QPair<qrRepo::RepoApi*, Id>();
}

QPair<qrRepo::RepoApi*, Id> InterpreterEditorManager::repoAndDiagram(const QString &editor
		, const QString &diagram) const
{
	foreach (qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
		foreach (const Id &edit, repo->elementsByType("MetamodelDiagram")) {
			if (editor != repo->name(edit) || !repo->isLogicalElement(edit)) {
				continue;
			}

			foreach (const Id &editChild, repo->children(edit)) {
				if (diagram == repo->name(editChild)) {
					return qMakePair(repo, editChild);
				}
			}
		}
	}

	return QPair<qrRepo::RepoApi*, Id>();
}

QPair<Id, Id> InterpreterEditorManager::editorAndDiagram(const qrRepo::RepoApi * const repo, const Id &element) const
{
	foreach (const Id &edit,  repo->elementsByType("MetamodelDiagram")) {
		foreach (const Id &diagram, repo->children(edit)) {
			foreach (const Id &elem, repo->children(diagram)) {
				if (element == elem) {
					return qMakePair(edit, diagram);
				}
			}
		}
	}

	return QPair<Id, Id>();
}

//QList<StringPossibleEdge> InterpreterEditorManager::possibleEdges(const QString &editor
//		, const QString &elementName) const
//{
//	QList<StringPossibleEdge> result;
//	QPair<qrRepo::RepoApi*, Id> const repoAndElementPair = repoAndElement(editor, elementName);
//	const qrRepo::RepoApi * const repo = repoAndElementPair.first;
//	const Id element = repoAndElementPair.second;
//	foreach (const Id &child, repo->children(element)) {
//		if (repo->name(child) == "possibleEdge") {
//			foreach (const Id &grandChild, repo->children(child)) {
//				const bool directed = repo->stringProperty(grandChild, "directed") == "true";
//				result.append(qMakePair(qMakePair(repo->stringProperty(grandChild, "beginName")
//						, repo->stringProperty(grandChild, "endName")), qMakePair(directed, repo->name(element))));
//			}
//		}
//	}

//	return result;
//}

QStringList InterpreterEditorManager::elements(const QString &editor, const QString &diagram) const
{
	QStringList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndDiagramPair = repoAndDiagram(editor, diagram);
	const qrRepo::RepoApi * const repo = repoAndDiagramPair.first;
	const Id diag = repoAndDiagramPair.second;
	for (const auto &element: repo->children(diag)) {
		if (element.element() != "MetaEntityEnum") {
			result.append(repo->name(element));
		}
	}

	return result;
}

int InterpreterEditorManager::isNodeOrEdge(const QString &editor, const QString &element) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndElementPair = repoAndElement(editor, element);
	const Id elem = repoAndElementPair.second;
	if (elem.element() == "MetaEntityEdge") {
		return -1;
	}

	if (elem.element() == "MetaEntityNode") {
		return 1;
	}

	return 0;
}

bool InterpreterEditorManager::isParentOf(const QString &editor, const QString &parentDiagram
		, const QString &parentElement, const QString &childDiagram, const QString &childElement) const
{
	return isParentOf(Id(editor, childDiagram, childElement)
			, Id(editor, parentDiagram, parentElement));
}

QString InterpreterEditorManager::diagramName(const QString &editor, const QString &diagram) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndDiagramPair = repoAndDiagram(editor, diagram);
	return repoAndDiagramPair.first->stringProperty(repoAndDiagramPair.second, "displayedName");
}

QString InterpreterEditorManager::diagramNodeName(const QString &editor, const QString &diagram) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndDiagramPair = repoAndDiagram(editor, diagram);
	return repoAndDiagramPair.first->stringProperty(repoAndDiagramPair.second, "nodeName");
}

bool InterpreterEditorManager::isInterpretationMode() const
{
	return true;
}

bool InterpreterEditorManager::isParentProperty(const Id &id, const QString &propertyName) const
{
	const QStringList propertiesFromParentsList = propertiesFromParents(id, "MetaEntity_Attribute", GetProperty());
	return propertiesFromParentsList.contains(propertyName);
}

void InterpreterEditorManager::deletePropertyInElement(qrRepo::RepoApi *repo
		, const Id &diagram, const QString &propertyName) const
{
	foreach (const Id &element, repo->children(diagram)) {
		foreach (const Id &property, repo->children(element)) {
			if (property.element() == "MetaEntity_Attribute") {
				if (repo->name(property) == propertyName) {
					repo->setProperty(property, "isHidden", "true");
					//repo->removeChild(element, property);
				}
			}
		}
	}
}

void InterpreterEditorManager::deleteProperty(const QString &propertyName) const
{
	foreach (qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
		foreach (const Id &editor, repo->elementsByType("MetamodelDiagram")) {
			foreach (const Id &diagram, repo->children(editor)) {
				if (repo->isLogicalElement(diagram)) {
					deletePropertyInElement(repo, diagram, propertyName);
				}
			}
		}
	}
}

void InterpreterEditorManager::addProperty(const Id &id, const QString &propDisplayedName) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	const Id newId = Id(repoAndMetaIdPair.second.editor(), repoAndMetaIdPair.second.diagram()
			, "MetaEntity_Attribute", QUuid::createUuid().toString());
	repoAndMetaIdPair.first->addChild(repoAndMetaIdPair.second, newId);
	repoAndMetaIdPair.first->setProperty(newId, "name", propDisplayedName);
	repoAndMetaIdPair.first->setProperty(newId, "displayedName", propDisplayedName);
	QStringList propertyNames;
	propertyNames << propDisplayedName;
	repoAndMetaIdPair.first->setProperty(newId, "maskedNames", propertyNames);
	repoAndMetaIdPair.first->setProperty(newId, "isHidden", "false");
}

IdList InterpreterEditorManager::elementsWithTheSameName(
		const Id &diagram
		, const QString &name
		, const QString type
		) const
{
	IdList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndDiagramPair = repoAndDiagram(diagram.editor(), diagram.diagram());
	qrRepo::RepoApi * const repo = repoAndDiagramPair.first;
	const Id diag = repoAndDiagramPair.second;

	for (const auto &element: repo->children(diag)) {
		if (repo->stringProperty(element, "displayedName") == name && element.element() == type
				&& repo->isLogicalElement(element)) {
			const QPair<Id, Id> editorAndDiagramPair = editorAndDiagram(repo, element);
			result << Id(repo->name(editorAndDiagramPair.first), repo->name(editorAndDiagramPair.second)
					, repo->name(element));
		}
	}

	return result;
}

IdList InterpreterEditorManager::propertiesWithTheSameName(
		const Id &id
		, const QString &propertyCurrentName
		, const QString &propertyNewName
		) const
{
	if (propertyDisplayedName(id, propertyCurrentName) == propertyNewName) {
		return IdList();
	}

	IdList result;
	const QPair<qrRepo::RepoApi*, Id> repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;

	for (const Id &idProperty : repo->children(metaId)) {
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

QStringList InterpreterEditorManager::getPropertiesInformation(const Id &id) const
{
	QStringList result;

	QStringList propertyNamesList = propertyNames(id);
	for (const QString &property: propertyNamesList) {
		result << propertyDisplayedName(id, property);
		result << typeName(id, property);
		result << defaultPropertyValue(id, property);
	}

	return result;
}

QStringList InterpreterEditorManager::getSameNamePropertyParams(const Id &propertyId, const QString &propertyName) const
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

void InterpreterEditorManager::restoreRemovedProperty(const Id &propertyId, const QString &previousName) const
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

void InterpreterEditorManager::restoreRenamedProperty(const Id &propertyId, const QString &previousName) const
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

void InterpreterEditorManager::setElementEnabled(const Id &type, bool enabled)
{
	Q_UNUSED(type)
	Q_UNUSED(enabled)
}

QMap<QString, qrRepo::RepoApi*> InterpreterEditorManager::listOfMetamodels() const
{
	return mEditorRepoApi;
}

void InterpreterEditorManager::setProperty(qrRepo::RepoApi *repo, const Id &id
		, const QString &property, const QVariant &propertyValue) const
{
	repo->setProperty(id, property, propertyValue);
}

void InterpreterEditorManager::updateProperties(const Id &id, const QString &property, const QString &propertyType
		, const QString &propertyDefaultValue, const QString &propertyDisplayedName) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	Id propertyMetaId;
	foreach (const Id &propertyId, repo->children(repoAndMetaIdPair.second)) {
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

QString InterpreterEditorManager::propertyNameByDisplayedName(const Id &id, const QString &displayedPropertyName) const
{
	QString propertyName = "";
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	foreach (const Id &property, repoAndMetaIdPair.first->children(repoAndMetaIdPair.second)) {
		if (property.element() == "MetaEntity_Attribute"
			&& repoAndMetaIdPair.first->stringProperty(property, "displayedName") == displayedPropertyName)
		{
			propertyName = repoAndMetaIdPair.first->name(property);
		}
	}

	return propertyName;
}

IdList InterpreterEditorManager::children(const Id &parent) const {
	IdList result;
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(parent);
	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;
	foreach (const Id &link , repo->outgoingLinks(metaId)) {
		if (link.element() == "Inheritance") {
			const Id metaChild = repo->otherEntityFromLink(link, metaId);
			QPair<Id, Id> const editorAndDiagramPair = editorAndDiagram(repo, metaChild);
			const Id child = Id(repo->name(editorAndDiagramPair.first), repo->name(editorAndDiagramPair.second)
					, repo->name(metaChild));
			result << child;
			result << children(child);
		}
	}

	return result;
}

QString InterpreterEditorManager::shape(const Id &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;
	if (metaId.element() == "MetaEntityNode") {
		return repo->stringProperty(metaId, "shape");
	}

	return "";
}

void InterpreterEditorManager::updateShape(const Id &id, const QString &graphics) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	if (repoAndMetaIdPair.second.element() == "MetaEntityNode") {
		repoAndMetaIdPair.first->setProperty(repoAndMetaIdPair.second, "shape", graphics);
	}
}

void InterpreterEditorManager::resetIsHidden(const Id &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	repoAndMetaIdPair.first->setProperty(repoAndMetaIdPair.second, "isHidden", "false");
}

QString InterpreterEditorManager::getIsHidden(const Id &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;
	return repo->stringProperty(metaId, "isHidden");
}

void InterpreterEditorManager::deleteElement(const Id &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;
	repo->setProperty(metaId, "isHidden", "true");
}

bool InterpreterEditorManager::isRootDiagramNode(const Id &id) const
{
	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;
	QPair<Id, Id> const editorAndDiagramPair = editorAndDiagram(repo, metaId);
	return repo->stringProperty(editorAndDiagramPair.second, "nodeName") == repo->name(metaId);
}

void InterpreterEditorManager::setStandartConfigurations(qrRepo::RepoApi *repo, const Id &id, const Id &parent
		, const QString &name) const
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


void InterpreterEditorManager::addNodeElement(const Id &diagram, const QString &name
		, const QString &displayedName, bool isRootDiagramNode) const
{
	const QString shape =
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
	const Id diag = repoAndDiagramPair.second;
	const Id nodeId("MetaEditor", "MetaEditor", "MetaEntityNode", QUuid::createUuid().toString());
	repo->addChild(diag, nodeId);

	if (isRootDiagramNode) {
		repo->setProperty(diag, "nodeName", name);
	}

	repo->setProperty(nodeId, "name", name);
	repo->setProperty(nodeId, "displayedName", displayedName);
	repo->setProperty(nodeId, "shape", shape);
	repo->setProperty(nodeId, "isResizeable", "true");
	repo->setProperty(nodeId, "isAction", "false");
	repo->setProperty(nodeId, "links", IdListHelper::toVariant(IdList()));
	repo->setProperty(nodeId, "createChildrenFromMenu", "false");
	repo->setProperty(nodeId, "isHidden", "false");

	foreach (const Id &elem, repo->children(diag)) {
		if (repo->name(elem) == "AbstractNode" && repo->isLogicalElement(elem)) {
			const Id inheritanceLink("MetaEditor", "MetaEditor", "Inheritance", QUuid::createUuid().toString());
			setStandartConfigurations(repo, inheritanceLink, Id::rootId(), "Inheritance");
			repo->setFrom(inheritanceLink, elem);
			repo->setTo(inheritanceLink, nodeId);

			const Id containerLink("MetaEditor", "MetaEditor", "Container", QUuid::createUuid().toString());
			setStandartConfigurations(repo, containerLink, Id::rootId(), "Container");
			repo->setFrom(containerLink, nodeId);
			repo->setTo(containerLink, elem);
		}
	}
}

void InterpreterEditorManager::addEdgeElement(const Id &diagram, const QString &name
		, const QString &displayedName, const QString &labelText, const QString &labelType
		, const QString &lineType, const QString &beginType, const QString &endType) const
{
	const QPair<qrRepo::RepoApi*, Id> repoAndDiagramPair = repoAndDiagram(diagram.editor(), diagram.diagram());
	qrRepo::RepoApi * const repo = repoAndDiagramPair.first;
	const Id diag = repoAndDiagramPair.second;
	Id edgeId("MetaEditor", "MetaEditor", "MetaEntityEdge", QUuid::createUuid().toString());
	repo->addChild(diag, edgeId);
	Id associationId("MetaEditor", "MetaEditor", "MetaEntityAssociation", QUuid::createUuid().toString());
	repo->addChild(edgeId, associationId);

	repo->setProperty(edgeId, "name", name);
	repo->setProperty(edgeId, "displayedName", displayedName);
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

void InterpreterEditorManager::updateGenerationRule(const Id &id, const QString &newRule) const
{
	const QPair<qrRepo::RepoApi*, Id> repoAndMetaIdPair = repoAndMetaId(id);
	if (repoAndMetaIdPair.second.element() == "MetaEntityNode") {
		repoAndMetaIdPair.first->setProperty(repoAndMetaIdPair.second, "generationRule", newRule);
	}
}

QString InterpreterEditorManager::generationRule(const Id &id) const
{
	const QPair<qrRepo::RepoApi*, Id> repoAndMetaIdPair = repoAndMetaId(id);
	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
	const Id metaId = repoAndMetaIdPair.second;
	if (metaId.element() == "MetaEntityNode") {
		return repo->stringProperty(metaId, "generationRule");
	}

	return "";
}

QPair<Id, Id> InterpreterEditorManager::createEditorAndDiagram(const QString &name) const
{
	const Id editor("MetaEditor", "MetaEditor", "MetamodelDiagram", QUuid::createUuid().toString());
	const Id diagram("MetaEditor", "MetaEditor", "MetaEditorDiagramNode", QUuid::createUuid().toString());
	qrRepo::RepoApi * const repo = mEditorRepoApi.value("test");
	repo->addChild(Id::rootId(), editor);
	repo->setProperty(editor, "name", name);
	repo->setProperty(editor, "displayedName", name);
	repo->addChild(editor, diagram);
	repo->setProperty(diagram, "name", name);
	repo->setProperty(diagram, "displayedName", name);
	repo->setProperty(diagram, "nodeName", name);
	const Id nodeId("MetaEditor", "MetaEditor", "MetaEntityNode", QUuid::createUuid().toString());
	repo->addChild(diagram, nodeId);
	repo->setProperty(nodeId, "name", "AbstractNode");
	repo->setProperty(nodeId, "displayedName", "AbstractNode");
	repo->setProperty(nodeId, "shape", "");
	repo->setProperty(nodeId, "links", IdListHelper::toVariant(IdList()));
	repo->setProperty(nodeId, "isResizeable", "true");
	repo->setProperty(nodeId, "isAction", "false");
	const Id containerLink("MetaEditor", "MetaEditor", "Container", QUuid::createUuid().toString());
	setStandartConfigurations(repo, containerLink, Id::rootId(), "Container");
	repo->setFrom(containerLink, nodeId);
	repo->setTo(containerLink, nodeId);
	return qMakePair(Id(repo->name(editor)), Id(repo->name(editor), repo->name(diagram)));
}

void InterpreterEditorManager::saveMetamodel(const QString &newMetamodelFileName)
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

QStringList InterpreterEditorManager::paletteGroups(const Id &editor, const Id &diagram) const
{
	Q_UNUSED(editor);
	Q_UNUSED(diagram);
	return QStringList();
}

QStringList InterpreterEditorManager::paletteGroupList(const Id &editor,const Id &diagram, const QString &group) const
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

QString InterpreterEditorManager::paletteGroupDescription(const Id &editor, const Id &diagram
		, const QString &group) const
{
	Q_UNUSED(editor);
	Q_UNUSED(diagram);
	Q_UNUSED(group);
	return "";
}

bool InterpreterEditorManager::shallPaletteBeSorted(const Id &editor, const Id &diagram) const
{
	Q_UNUSED(editor);
	Q_UNUSED(diagram);
	return true;
}

IdList InterpreterEditorManager::groups(const Id &diagram)
{
	Q_UNUSED(diagram);
	return IdList();
}

Pattern InterpreterEditorManager::getPatternByName (const QString &str) const
{
	Q_UNUSED(str);
	return Pattern();
}

QList<QString> InterpreterEditorManager::getPatternNames() const
{
	return QList<QString>();
}

QSize InterpreterEditorManager::iconSize(const Id &id) const
{
	Q_UNUSED(id);
	return QSize();
}
