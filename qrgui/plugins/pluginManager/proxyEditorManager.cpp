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

#include "proxyEditorManager.h"

using namespace qReal;

ProxyEditorManager::ProxyEditorManager(EditorManagerInterface *editorManagerInterface)
		: mProxiedEditorManager(editorManagerInterface)
{
}

ProxyEditorManager::~ProxyEditorManager()
{
	delete mProxiedEditorManager;
}

IdList ProxyEditorManager::editors() const
{
	return mProxiedEditorManager->editors();
}

IdList ProxyEditorManager::diagrams(const Id &editor) const
{
	return mProxiedEditorManager->diagrams(editor);
}

IdList ProxyEditorManager::elements(const Id &diagram) const
{
	return mProxiedEditorManager->elements(diagram);
}

Version ProxyEditorManager::version(const Id &editor) const
{
	return mProxiedEditorManager->version(editor);
}

QString ProxyEditorManager::loadPlugin(const QString &pluginName)
{
	return mProxiedEditorManager->loadPlugin(pluginName);
}

QString ProxyEditorManager::unloadPlugin(const QString &pluginName)
{
	return mProxiedEditorManager->unloadPlugin(pluginName);
}

QString ProxyEditorManager::mouseGesture(const Id &id) const
{
	return mProxiedEditorManager->mouseGesture(id);
}

QString ProxyEditorManager::friendlyName(const Id &id) const
{
	return mProxiedEditorManager->friendlyName(id);
}

QString ProxyEditorManager::description(const Id &id) const
{
	return mProxiedEditorManager->description(id);
}

QString ProxyEditorManager::propertyDescription(const Id &id, const QString &propertyName) const
{
	return mProxiedEditorManager->propertyDescription(id, propertyName);
}

QString ProxyEditorManager::propertyDisplayedName(const Id &id, const QString &propertyName) const
{
	return mProxiedEditorManager->propertyDisplayedName(id, propertyName);
}

QIcon ProxyEditorManager::icon(const Id &id) const
{
	return mProxiedEditorManager->icon(id);
}

ElementType &ProxyEditorManager::elementType(const Id &id) const
{
	return mProxiedEditorManager->elementType(id);
}

IdList ProxyEditorManager::containedTypes(const Id &id) const
{
	return mProxiedEditorManager->containedTypes(id);
}

bool ProxyEditorManager::isEnumEditable(const Id &id, const QString &name) const
{
	return mProxiedEditorManager->isEnumEditable(id, name);
}

QList<QPair<QString, QString>> ProxyEditorManager::enumValues(const Id &id, const QString &name) const
{
	return mProxiedEditorManager->enumValues(id, name);
}

QString ProxyEditorManager::typeName(const Id &id, const QString &name) const
{
	return mProxiedEditorManager->typeName(id, name);
}

QStringList ProxyEditorManager::allChildrenTypesOf(const Id &parent) const
{
	return mProxiedEditorManager->allChildrenTypesOf(parent);
}

QList<Explosion> ProxyEditorManager::explosions(const Id &source) const
{
	return mProxiedEditorManager->explosions(source);
}

bool ProxyEditorManager::isEditor(const Id &id) const
{
	return mProxiedEditorManager->isEditor(id);
}

bool ProxyEditorManager::isDiagram(const Id &id) const
{
	return mProxiedEditorManager->isDiagram(id);
}

bool ProxyEditorManager::isElement(const Id &id) const
{
	return mProxiedEditorManager->isElement(id);
}

QStringList ProxyEditorManager::propertyNames(const Id &id) const
{
	return mProxiedEditorManager->propertyNames(id);
}

QStringList ProxyEditorManager::portTypes(const Id &id) const
{
	return mProxiedEditorManager->portTypes(id);
}

QString ProxyEditorManager::defaultPropertyValue(const Id &id, QString name) const
{
	return mProxiedEditorManager->defaultPropertyValue(id, name);
}

QStringList ProxyEditorManager::propertiesWithDefaultValues(const Id &id) const
{
	return mProxiedEditorManager->propertiesWithDefaultValues(id);
}

bool ProxyEditorManager::hasElement(const Id &element) const
{
	return mProxiedEditorManager->hasElement(element);
}

Id ProxyEditorManager::findElementByType(const QString &type) const
{
	return mProxiedEditorManager->findElementByType(type);
}

bool ProxyEditorManager::isDiagramNode(const Id &id) const
{
	return mProxiedEditorManager->isDiagramNode(id);
}

bool ProxyEditorManager::isParentOf(const Id &child, const Id &parent) const
{
	return mProxiedEditorManager->isParentOf(child, parent);
}

bool ProxyEditorManager::isGraphicalElementNode(const Id &id) const
{
	return mProxiedEditorManager->isGraphicalElementNode(id);
}

Id ProxyEditorManager::theOnlyDiagram() const
{
	return mProxiedEditorManager->theOnlyDiagram();
}

QString ProxyEditorManager::diagramNodeNameString(const Id &editor, const Id &diagram) const
{
	return mProxiedEditorManager->diagramNodeNameString(editor, diagram);
}

//QList<StringPossibleEdge> ProxyEditorManager::possibleEdges(const QString &editor, const QString &element) const
//{
//	return mProxiedEditorManager->possibleEdges(editor, element);
//}

QStringList ProxyEditorManager::elements(const QString &editor, const QString &diagram) const
{
	return mProxiedEditorManager->elements(editor, diagram);
}

int ProxyEditorManager::isNodeOrEdge(const QString &editor, const QString &element) const
{
	return mProxiedEditorManager->isNodeOrEdge(editor, element);
}

bool ProxyEditorManager::isParentOf(const QString &editor, const QString &parentDiagram
		, const QString &parentElement, const QString &childDiagram, const QString &childElement) const
{
	return mProxiedEditorManager->isParentOf(editor, parentDiagram, parentElement, childDiagram, childElement);
}

QString ProxyEditorManager::diagramName(const QString &editor, const QString &diagram) const
{
	return mProxiedEditorManager->diagramName(editor, diagram);
}

QString ProxyEditorManager::diagramNodeName(const QString &editor, const QString &diagram) const
{
	return mProxiedEditorManager->diagramNodeName(editor, diagram);
}

void ProxyEditorManager::setProxyManager(EditorManagerInterface *editorManagerInterface)
{
	delete mProxiedEditorManager;
	mProxiedEditorManager = editorManagerInterface;
}

bool ProxyEditorManager::isInterpretationMode() const
{
	return mProxiedEditorManager->isInterpretationMode();
}

bool ProxyEditorManager::isParentProperty(const Id &id, const QString &propertyName) const
{
	return mProxiedEditorManager->isParentProperty(id, propertyName);
}

void ProxyEditorManager::deleteProperty(const QString &propDisplayedName) const
{
	mProxiedEditorManager->deleteProperty(propDisplayedName);
}

void ProxyEditorManager::addProperty(const Id &id, const QString &propDisplayedName) const
{
	mProxiedEditorManager->addProperty(id, propDisplayedName);
}

void ProxyEditorManager::updateProperties(const Id &id, const QString &property, const QString &propertyType
		, const QString &propertyDefaultValue, const QString &propertyDisplayedName) const
{
	mProxiedEditorManager->updateProperties(id, property, propertyType, propertyDefaultValue, propertyDisplayedName);
}

QString ProxyEditorManager::propertyNameByDisplayedName(const Id &id, const QString &displayedPropertyName) const
{
	return mProxiedEditorManager->propertyNameByDisplayedName(id, displayedPropertyName);
}

IdList ProxyEditorManager::children(const Id &parent) const
{
	return mProxiedEditorManager->children(parent);
}

QString ProxyEditorManager::shape(const Id &id) const
{
	return mProxiedEditorManager->shape(id);
}

void ProxyEditorManager::updateShape(const Id &id, const QString &graphics) const
{
	mProxiedEditorManager->updateShape(id, graphics);
}

void ProxyEditorManager::resetIsHidden(const Id &id) const
{
	mProxiedEditorManager->resetIsHidden(id);
}

QString ProxyEditorManager::getIsHidden(const Id &id) const
{
	return mProxiedEditorManager->getIsHidden(id);
}

void ProxyEditorManager::deleteElement(const Id &id) const
{
	mProxiedEditorManager->deleteElement(id);
}

bool ProxyEditorManager::isRootDiagramNode(const Id &id) const
{
	return mProxiedEditorManager->isRootDiagramNode(id);
}

void ProxyEditorManager::addNodeElement(const Id &diagram, const QString &name
		, const QString &displayedName, bool isRootDiagramNode) const
{
	mProxiedEditorManager->addNodeElement(diagram, name, displayedName, isRootDiagramNode);
}

void ProxyEditorManager::addEdgeElement(const Id &diagram, const QString &name
		, const QString &displayedName, const QString &labelText, const QString &labelType
		, const QString &lineType, const QString &beginType, const QString &endType) const
{
	mProxiedEditorManager->addEdgeElement(diagram, name, displayedName, labelText, labelType
			, lineType, beginType, endType);
}

void ProxyEditorManager::updateGenerationRule(const Id &id, const QString &newRule) const
{
	mProxiedEditorManager->updateGenerationRule(id, newRule);
}

QString ProxyEditorManager::generationRule(const Id &id) const
{
	return mProxiedEditorManager->generationRule(id);
}

QPair<Id, Id> ProxyEditorManager::createEditorAndDiagram(const QString &name) const
{
	return mProxiedEditorManager->createEditorAndDiagram(name);
}

void ProxyEditorManager::saveMetamodel(const QString &newMetamodelFileName)
{
	mProxiedEditorManager->saveMetamodel(newMetamodelFileName);
}

QString ProxyEditorManager::saveMetamodelFilePath() const
{
	return mProxiedEditorManager->saveMetamodelFilePath();
}

QStringList ProxyEditorManager::paletteGroups(const Id &editor, const Id &diagram) const
{
	return mProxiedEditorManager->paletteGroups(editor, diagram);
}

QStringList ProxyEditorManager::paletteGroupList(const Id &editor,const Id &diagram, const QString &group) const
{
	return mProxiedEditorManager->paletteGroupList(editor, diagram, group);
}

QString ProxyEditorManager::paletteGroupDescription(const Id &editor, const Id &diagram, const QString &group) const
{
	return mProxiedEditorManager->paletteGroupDescription(editor, diagram, group);
}

bool ProxyEditorManager::shallPaletteBeSorted(const Id &editor, const Id &diagram) const
{
	return mProxiedEditorManager->shallPaletteBeSorted(editor, diagram);
}

QStringList ProxyEditorManager::referenceProperties(const Id &id) const
{
	return mProxiedEditorManager->referenceProperties(id);
}

IdList ProxyEditorManager::groups(const Id &diagram)
{
	return mProxiedEditorManager->groups(diagram);
}

Pattern ProxyEditorManager::getPatternByName (const QString &str) const
{
	return mProxiedEditorManager->getPatternByName(str);
}

QList<QString> ProxyEditorManager::getPatternNames() const
{
	return mProxiedEditorManager->getPatternNames();
}

QSize ProxyEditorManager::iconSize(const Id &id) const
{
	return mProxiedEditorManager->iconSize(id);
}

IdList ProxyEditorManager::elementsWithTheSameName(const Id &diagram, const QString &name, const QString type) const
{
	return mProxiedEditorManager->elementsWithTheSameName(diagram, name, type);
}

IdList ProxyEditorManager::propertiesWithTheSameName(const Id &id
		, const QString &propCurrentName, const QString &propNewName) const
{
	return mProxiedEditorManager->propertiesWithTheSameName(id, propCurrentName, propNewName);
}

QStringList ProxyEditorManager::getPropertiesInformation(const Id &id) const
{
	return mProxiedEditorManager->getPropertiesInformation(id);
}

QStringList ProxyEditorManager::getSameNamePropertyParams(const Id &propertyId, const QString &propertyName) const
{
	return mProxiedEditorManager->getSameNamePropertyParams(propertyId, propertyName);
}

void ProxyEditorManager::restoreRemovedProperty(const Id &propertyId, const QString &previousName) const
{
	mProxiedEditorManager->restoreRemovedProperty(propertyId, previousName);
}

void ProxyEditorManager::restoreRenamedProperty(const Id &propertyId, const QString &previousName) const
{
	mProxiedEditorManager->restoreRenamedProperty(propertyId, previousName);
}

void ProxyEditorManager::setElementEnabled(const Id &type, bool enabled)
{
	mProxiedEditorManager->setElementEnabled(type, enabled);
}

EditorManagerInterface* ProxyEditorManager::proxiedEditorManager() const
{
	return mProxiedEditorManager;
}
