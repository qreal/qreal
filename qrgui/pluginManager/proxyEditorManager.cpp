#include "proxyEditorManager.h"

using namespace qReal;

ProxyEditorManager::ProxyEditorManager(EditorManagerInterface *editorManagerInter)
	: mProxyManager(editorManagerInter)
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

IdList ProxyEditorManager::getContainedTypes(const Id &id) const
{
	return mProxyManager->getContainedTypes(id);
}

IdList ProxyEditorManager::getConnectedTypes(const Id &id) const
{
	return mProxyManager->getConnectedTypes(id);
}

IdList ProxyEditorManager::getUsedTypes(const Id &id) const
{
	return mProxyManager->getUsedTypes(id);
}

QStringList ProxyEditorManager::getEnumValues(Id const &id, const QString &name) const
{
	return mProxyManager->getEnumValues(id, name);
}

QString ProxyEditorManager::getTypeName(Id const &id, const QString &name) const
{
	return mProxyManager->getTypeName(id, name);
}

QStringList ProxyEditorManager::getAllChildrenTypesOf(Id const &parent) const
{
	return mProxyManager->getAllChildrenTypesOf(parent);
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

QStringList ProxyEditorManager::getPropertyNames(Id const &id) const
{
	return mProxyManager->getPropertyNames(id);
}

QString ProxyEditorManager::getDefaultPropertyValue(Id const &id, QString name) const
{
	return mProxyManager->getDefaultPropertyValue(id, name);
}

QStringList ProxyEditorManager::getPropertiesWithDefaultValues(Id const &id) const
{
	return mProxyManager->getPropertiesWithDefaultValues(id);
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
QList<QPair<QPair<QString, QString>, QPair<bool, QString> > > ProxyEditorManager::getPossibleEdges(QString const &editor, QString const &element) const
{
	return mProxyManager->getPossibleEdges(editor, element);
}

QStringList ProxyEditorManager::elements(QString const &editor, QString const &diagram) const
{
	return mProxyManager->elements(editor, diagram);
}

int ProxyEditorManager::isNodeOrEdge(QString const &editor, QString const &element) const
{
	return mProxyManager->isNodeOrEdge(editor, element);
}

bool ProxyEditorManager::isParentOf(QString const &editor, QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
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

void ProxyEditorManager::setProxyManager(EditorManagerInterface *editorManagerInter)
{
	mProxyManager = editorManagerInter;
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
