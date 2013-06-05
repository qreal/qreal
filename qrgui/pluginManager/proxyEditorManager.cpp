#include "proxyEditorManager.h"

using namespace qReal;

ProxyEditorManager::ProxyEditorManager(EditorManagerInterface *editorManagerInterface)
		: mProxyManager(editorManagerInterface)
{}

IdList ProxyEditorManager::editors() const
{
	return mProxyManager->editors();
}

IdList ProxyEditorManager::diagrams(Id const &editor) const
{
	return mProxyManager->diagrams(editor);
}

IdList ProxyEditorManager::elements(Id const &diagram) const
{
	return mProxyManager->elements(diagram);
}

bool ProxyEditorManager::loadPlugin(QString const &pluginName)
{
	return mProxyManager->loadPlugin(pluginName);
}

bool ProxyEditorManager::unloadPlugin(QString const &pluginName)
{
	return mProxyManager->unloadPlugin(pluginName);
}

QString ProxyEditorManager::mouseGesture(Id const &id) const
{
	return mProxyManager->mouseGesture(id);
}

QString ProxyEditorManager::friendlyName(Id const &id) const
{
	return mProxyManager->friendlyName(id);
}

QString ProxyEditorManager::description(Id const &id) const
{
	return mProxyManager->description(id);
}

QString ProxyEditorManager::propertyDescription(Id const &id, QString const &propertyName) const
{
	return mProxyManager->propertyDescription(id, propertyName);
}

QString ProxyEditorManager::propertyDisplayedName(Id const &id, QString const &propertyName) const
{
	return mProxyManager->propertyDisplayedName(id, propertyName);
}

QIcon ProxyEditorManager::icon(Id const &id) const
{
	return mProxyManager->icon(id);
}

Element* ProxyEditorManager::graphicalObject(Id const &id) const
{
	return mProxyManager->graphicalObject(id);
}

IdList ProxyEditorManager::containedTypes(const Id &id) const
{
	return mProxyManager->containedTypes(id);
}

IdList ProxyEditorManager::connectedTypes(const Id &id) const
{
	return mProxyManager->connectedTypes(id);
}

IdList ProxyEditorManager::usedTypes(const Id &id) const
{
	return mProxyManager->usedTypes(id);
}

QStringList ProxyEditorManager::enumValues(Id const &id, const QString &name) const
{
	return mProxyManager->enumValues(id, name);
}

QString ProxyEditorManager::typeName(Id const &id, const QString &name) const
{
	return mProxyManager->typeName(id, name);
}

QStringList ProxyEditorManager::allChildrenTypesOf(Id const &parent) const
{
	return mProxyManager->allChildrenTypesOf(parent);
}

bool ProxyEditorManager::isEditor(Id const &id) const
{
	return mProxyManager->isEditor(id);
}

bool ProxyEditorManager::isDiagram(Id const &id) const
{
	return mProxyManager->isDiagram(id);
}

bool ProxyEditorManager::isElement(Id const &id) const
{
	return mProxyManager->isElement(id);
}

QStringList ProxyEditorManager::propertyNames(Id const &id) const
{
	return mProxyManager->propertyNames(id);
}

QString ProxyEditorManager::defaultPropertyValue(Id const &id, QString name) const
{
	return mProxyManager->defaultPropertyValue(id, name);
}

QStringList ProxyEditorManager::propertiesWithDefaultValues(Id const &id) const
{
	return mProxyManager->propertiesWithDefaultValues(id);
}

IdList ProxyEditorManager::checkNeededPlugins(qrRepo::LogicalRepoApi const &logicalApi
		, qrRepo::GraphicalRepoApi const &graphicalApi) const
{
	return mProxyManager->checkNeededPlugins(logicalApi, graphicalApi);
}

bool ProxyEditorManager::hasElement(Id const &element) const
{
	return mProxyManager->hasElement(element);
}

Id ProxyEditorManager::findElementByType(QString const &type) const
{
	return mProxyManager->findElementByType(type);
}

QList<ListenerInterface *> ProxyEditorManager::listeners() const
{
	return mProxyManager->listeners();
}

bool ProxyEditorManager::isDiagramNode(Id const &id) const
{
	return mProxyManager->isDiagramNode(id);
}

bool ProxyEditorManager::isParentOf(Id const &child, Id const &parent) const
{
	return mProxyManager->isParentOf(child, parent);
}

bool ProxyEditorManager::isGraphicalElementNode(const Id &id) const
{
	return mProxyManager->isGraphicalElementNode(id);
}

//new methods:
QList<StringPossibleEdge> ProxyEditorManager::possibleEdges(QString const &editor, QString const &element) const
{
	return mProxyManager->possibleEdges(editor, element);
}

QStringList ProxyEditorManager::elements(QString const &editor, QString const &diagram) const
{
	return mProxyManager->elements(editor, diagram);
}

int ProxyEditorManager::isNodeOrEdge(QString const &editor, QString const &element) const
{
	return mProxyManager->isNodeOrEdge(editor, element);
}

bool ProxyEditorManager::isParentOf(QString const &editor, QString const &parentDiagram
		, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	return mProxyManager->isParentOf(editor, parentDiagram, parentElement, childDiagram, childElement);
}

QString ProxyEditorManager::diagramName(QString const &editor, QString const &diagram) const
{
	return mProxyManager->diagramName(editor, diagram);
}

QString ProxyEditorManager::diagramNodeName(QString const &editor, QString const &diagram) const
{
	return mProxyManager->diagramNodeName(editor, diagram);
}

void ProxyEditorManager::setProxyManager(EditorManagerInterface *editorManagerInterface)
{
	delete mProxyManager;
	mProxyManager = editorManagerInterface;
}

bool ProxyEditorManager::isInterpretationMode() const
{
	return mProxyManager->isInterpretationMode();
}

bool ProxyEditorManager::isParentProperty(Id const &id, QString const &propertyName) const
{
	return mProxyManager->isParentProperty(id, propertyName);
}

void ProxyEditorManager::deleteProperty(QString const &propDisplayedName) const
{
	mProxyManager->deleteProperty(propDisplayedName);
}

void ProxyEditorManager::addProperty(Id const &id, QString const &propDisplayedName) const
{
	mProxyManager->addProperty(id, propDisplayedName);
}

void ProxyEditorManager::updateProperties(Id const &id, QString const &property, QString const &propertyType
		, QString const &propertyDefaultValue, QString const &propertyDisplayedName) const
{
	mProxyManager->updateProperties(id, property, propertyType, propertyDefaultValue, propertyDisplayedName);
}

QString ProxyEditorManager::propertyNameByDisplayedName(Id const &id, QString const &displayedPropertyName) const
{
	return mProxyManager->propertyNameByDisplayedName(id, displayedPropertyName);
}

IdList ProxyEditorManager::children(Id const &parent) const
{
	return mProxyManager->children(parent);
}

QString ProxyEditorManager::shape(Id const &id) const
{
	return mProxyManager->shape(id);
}

void ProxyEditorManager::updateShape(Id const &id, QString const &graphics) const
{
	mProxyManager->updateShape(id, graphics);
}

void ProxyEditorManager::deleteElement(MainWindow *mainWindow, Id const &id) const
{
	mProxyManager->deleteElement(mainWindow, id);
}

bool ProxyEditorManager::isRootDiagramNode(Id const &id) const
{
	return mProxyManager->isRootDiagramNode(id);
}

void ProxyEditorManager::addNodeElement(Id const &diagram, QString const &name, bool isRootDiagramNode) const
{
	mProxyManager->addNodeElement(diagram, name, isRootDiagramNode);
}

void ProxyEditorManager::addEdgeElement(Id const &diagram, QString const &name, QString const &labelText, QString const &labelType
		, QString const &lineType, QString const &beginType, QString const &endType) const
{
	mProxyManager->addEdgeElement(diagram, name, labelText, labelType, lineType, beginType, endType);
}

QPair<Id, Id> ProxyEditorManager::createEditorAndDiagram(QString const &name) const
{
	return mProxyManager->createEditorAndDiagram(name);
}

void ProxyEditorManager::saveMetamodel(QString const &newMetamodelFileName)
{
	mProxyManager->saveMetamodel(newMetamodelFileName);
}

QString ProxyEditorManager::saveMetamodelFilePath() const
{
	return mProxyManager->saveMetamodelFilePath();
}

//unsupported methods:
QStringList ProxyEditorManager::paletteGroups(Id const &editor, Id const &diagram) const
{
	return mProxyManager->paletteGroups(editor, diagram);
}

QStringList ProxyEditorManager::paletteGroupList(Id const &editor,Id const &diagram, QString const &group) const
{
	return mProxyManager->paletteGroupList(editor, diagram, group);
}

QString ProxyEditorManager::paletteGroupDescription(Id const &editor, const Id &diagram, const QString &group) const
{
	return mProxyManager->paletteGroupDescription(editor, diagram, group);
}

QStringList ProxyEditorManager::referenceProperties(const Id &id) const
{
	return mProxyManager->referenceProperties(id);
}

IdList ProxyEditorManager::groups(Id const &diagram)
{
	return mProxyManager->groups(diagram);
}

Pattern ProxyEditorManager::getPatternByName (QString const &str) const
{
	return mProxyManager->getPatternByName(str);
}

QList<QString> ProxyEditorManager::getPatternNames() const
{
	return mProxyManager->getPatternNames();
}

QSize ProxyEditorManager::iconSize(Id const &id) const
{
	return mProxyManager->iconSize(id);
}
