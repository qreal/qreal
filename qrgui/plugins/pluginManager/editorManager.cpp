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

#include "editorManager.h"

#include <QtCore/QCoreApplication>
#include <QtGui/QIcon>

#include <qrkernel/ids.h>
#include <qrkernel/logging.h>
#include <qrkernel/platformInfo.h>
#include <qrkernel/exception/exception.h>
#include <qrrepo/repoApi.h>
#include <metaMetaModel/metamodel.h>

#include "editor/nodeElement.h"
#include "editor/edgeElement.h"

using namespace qReal;

EditorManager::EditorManager(const QString &path)
	: mPluginManager(path)
{
	init();
}

EditorManager::EditorManager(QObject *parent)
	: QObject(parent)
	, mPluginManager(PlatformInfo::invariantSettingsPath("pathToEditorPlugins"))
{
	init();
}

EditorManager::~EditorManager()
{
	for (const QString &pluginName : mPluginIface.keys()) {
		unloadPlugin(pluginName);
	}
}

void EditorManager::init()
{
	const auto pluginsList = mPluginManager.loadAllPlugins<Metamodel>();

	for (Metamodel * const iEditor : pluginsList) {
		const QString pluginName = mPluginManager.fileName(iEditor);

		if (iEditor) {
			mPluginsLoaded += iEditor->id();
			mPluginFileName.insert(iEditor->id(), pluginName);
			mPluginIface[iEditor->id()] = iEditor;
		}
	}
}

QString EditorManager::loadPlugin(const QString &pluginName)
{
	Metamodel *iEditor = mPluginManager.pluginLoadedByName<Metamodel>(pluginName).first;
	const QString error = mPluginManager.pluginLoadedByName<Metamodel>(pluginName).second;

	if (iEditor) {
		mPluginsLoaded += iEditor->id();
		mPluginFileName.insert(iEditor->id(), pluginName);
		mPluginIface[iEditor->id()] = iEditor;
		QLOG_INFO() << "Plugin" << pluginName << "loaded. Version: " << iEditor->version();
		return QString();
	}

	QLOG_WARN() << "Editor plugin" << pluginName << "loading failed: " << error;
	return error;
}

QString EditorManager::unloadPlugin(const QString &pluginName)
{
	QString resultOfUnloading = "";
	if (!mPluginFileName[pluginName].isEmpty()) {
		resultOfUnloading = mPluginManager.unloadPlugin(mPluginFileName[pluginName]);
	} else {
		const QList<QString> namesOfPlugins = mPluginManager.namesOfPlugins();
		const QString tempName = pluginName.toLower();
		QString newPluginName = "";

		for (const QString &element : namesOfPlugins) {
			if (element.contains(tempName) && !element.contains(".a")) {
				newPluginName = element;
				break;
			}
		}

		resultOfUnloading = mPluginManager.unloadPlugin(newPluginName);
	}

	if (mPluginIface.keys().contains(pluginName)) {
		mPluginIface.remove(pluginName);
		mPluginFileName.remove(pluginName);
		mPluginsLoaded.removeAll(pluginName);

		if (!resultOfUnloading.isEmpty()) {
			QLOG_WARN() << "Editor plugin" << pluginName << "unloading failed: " + resultOfUnloading;
		}

		QLOG_INFO() << "Plugin" << pluginName << "unloaded";
	}

	return resultOfUnloading;
}

IdList EditorManager::editors() const
{
	IdList editors;
	foreach (const QString &editor, mPluginsLoaded) {
		editors.append(Id(editor));
	}
	return editors;
}

IdList EditorManager::diagrams(const Id &editor) const
{
	IdList diagrams;
	Q_ASSERT(mPluginsLoaded.contains(editor.editor()));

	foreach (const QString &diagram, mPluginIface[editor.editor()]->diagrams()) {
		diagrams.append(Id(editor, diagram));
	}

	return diagrams;
}

QStringList EditorManager::paletteGroups(const Id &editor, const Id &diagram) const
{
	Q_ASSERT(mPluginsLoaded.contains(diagram.editor()));
	return mPluginIface[editor.editor()]->diagramPaletteGroups(diagram.diagram());
}

QStringList EditorManager::paletteGroupList(const Id &editor, const Id &diagram, const QString &group) const
{
	return mPluginIface[editor.editor()]->diagramPaletteGroupList(diagram.diagram(), group);
}

QString EditorManager::paletteGroupDescription(const Id &editor, const Id &diagram, const QString &group) const
{
	return mPluginIface[editor.editor()]->diagramPaletteGroupDescription(diagram.diagram(), group);
}

bool EditorManager::shallPaletteBeSorted(const Id &editor, const Id &diagram) const
{
	return mPluginIface[editor.editor()]->shallPaletteBeSorted(diagram.diagram());
}

IdList EditorManager::elements(const Id &diagram) const
{
	IdList elements;
	Q_ASSERT(mPluginsLoaded.contains(diagram.editor()));

	for (const QString &e : mPluginIface[diagram.editor()]->elements(diagram.diagram())) {
		const Id candidate = Id(diagram.editor(), diagram.diagram(), e);
		if (!mDisabledElements.contains(candidate)) {
			elements.append(candidate);
		}
	}

	return elements;
}

Version EditorManager::version(const Id &editor) const
{
	Q_ASSERT(mPluginsLoaded.contains(editor.editor()));
	return Version::fromString(mPluginIface[editor.editor()]->version());
}

bool EditorManager::isEditor(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return id.idSize() == 1;
}

bool EditorManager::isDiagram(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return id.idSize() == 2;
}

bool EditorManager::isElement(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return id.idSize() == 3;
}

QString EditorManager::friendlyName(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));

	switch (id.idSize()) {
	case 1:
		return mPluginIface[id.editor()]->editorName();
	case 2:
		return mPluginIface[id.editor()]->diagramName(id.diagram());
	case 3:
		if (mGroups.keys().contains(id.element())) {
			return id.element();
		} else {
			return mPluginIface[id.editor()]->elementName(id.diagram(), id.element());
		}
	default:
		Q_ASSERT(!"Malformed Id");
		return "";
	}
}

QString EditorManager::description(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	if (id.idSize() != 3) {
		return "";
	}
	if (mGroups.keys().contains(id.element())) {
		return id.element();
	}
	return mPluginIface[id.editor()]->elementDescription(id.diagram(), id.element());
}

QString EditorManager::propertyDescription(const Id &id, const QString &propertyName) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));

	if (id.idSize() < 3) {
		return "";
	}
	return mPluginIface[id.editor()]->propertyDescription(id.diagram(), id.element(), propertyName);
}

QString EditorManager::propertyDisplayedName(const Id &id, const QString &propertyName) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));

	if (id.idSize() != 4) {
		return "";
	}
	return mPluginIface[id.editor()]->propertyDisplayedName(id.diagram(), id.element(), propertyName);
}

QString EditorManager::mouseGesture(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	if (id.idSize() != 3) {
		return "";
	}
	return mPluginIface[id.editor()]->elementMouseGesture(id.diagram(), id.element());
}

QIcon EditorManager::icon(const Id &id) const
{
	if (!mPluginsLoaded.contains(id.editor())) {
		return QIcon();
	}

	return SdfIconLoader::iconOf(":/generated/shapes/" + id.element() + "Class.sdf");
}

QSize EditorManager::iconSize(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return SdfIconLoader::preferedSizeOf(":/generated/shapes/" + id.element() + "Class.sdf");
}

ElementType &EditorManager::elementType(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	ElementType * const type = mPluginIface[id.editor()]->getGraphicalObject(id.diagram(), id.element());
	Q_ASSERT(type);
	return *type;
}

QStringList EditorManager::propertyNames(const Id &id) const
{
	Q_ASSERT(id.idSize() == 3); // Applicable only to element types
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return mPluginIface[id.editor()]->getPropertyNames(id.diagram(), id.element());
}

QStringList EditorManager::portTypes(const Id &id) const
{
	Q_ASSERT(id.idSize() == 3); // Applicable only to element types
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return mPluginIface[id.editor()]->getPortTypes(id.diagram(), id.element());
}

QStringList EditorManager::referenceProperties(const Id &id) const
{
	Q_ASSERT(id.idSize() == 3); // Applicable only to element types
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return mPluginIface[id.editor()]->getReferenceProperties(id.diagram(), id.element());
}

IdList EditorManager::containedTypes(const Id &id) const
{
	Q_ASSERT(id.idSize() == 3);  // Applicable only to element types
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));

	IdList result;
	foreach (const QString &type, mPluginIface[id.editor()]->getTypesContainedBy(id.element())) {
		result.append(Id(id.editor(), id.diagram(), type));
	}

	typedef QPair<QString, QString> StringPair;
	QList<StringPair> const parents = mPluginIface[id.editor()]->getParentsOf(id.diagram(), id.element());

	foreach (const StringPair &pair, parents) {
		result.append(containedTypes(Id(id.editor(), pair.first, pair.second)));
	}

	return result;
}

bool EditorManager::isEnumEditable(const Id &id, const QString &name) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	const QString typeName = mPluginIface[id.editor()]->getPropertyType(id.element(), name);
	return mPluginIface[id.editor()]->isEnumEditable(typeName);
}

QList<QPair<QString, QString>> EditorManager::enumValues(const Id &id, const QString &name) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	const QString typeName = mPluginIface[id.editor()]->getPropertyType(id.element(), name);
	return mPluginIface[id.editor()]->getEnumValues(typeName);
}

QString EditorManager::typeName(const Id &id, const QString &name) const
{
	return mPluginIface[id.editor()]->getPropertyType(id.element(), name);
}

QString EditorManager::defaultPropertyValue(const Id &id, QString name) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return mPluginIface[id.editor()]->getPropertyDefaultValue(id.element(), name);
}

QStringList EditorManager::propertiesWithDefaultValues(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return mPluginIface[id.editor()]->getPropertiesWithDefaultValues(id.element());
}

bool EditorManager::hasElement(const Id &elementId) const
{
	Q_ASSERT(elementId.idSize() == 3);
	if (!mPluginsLoaded.contains(elementId.editor()))
		return false;
	Metamodel *editor = mPluginIface[elementId.editor()];
	foreach (const QString &diagram, editor->diagrams()) {
		foreach (const QString &element, editor->elements(diagram)) {
			if (elementId.diagram() == diagram && elementId.element() == element) {
				return true;
			}
		}
	}
	return false;
}

Id EditorManager::findElementByType(const QString &type) const
{
	foreach (Metamodel *editor, mPluginIface.values()) {
		foreach (const QString &diagram, editor->diagrams()) {
			foreach (const QString &element, editor->elements(diagram)) {
				if (type == element) {
					return Id(editor->id(), diagram, element);
				}
			}
		}
	}
	throw Exception("No type " + type + " in loaded plugins");
}

Metamodel* EditorManager::metamodel(const QString &editor) const
{
	return mPluginIface[editor];
}

bool EditorManager::isDiagramNode(const Id &id) const
{
	return id.element() == metamodel(id.editor())->diagramNodeName(id.diagram());
}

bool EditorManager::isParentOf(const Id &child, const Id &parent) const // child — EnginesForware, parent — AbstractNode
{
	const Metamodel *plugin = mPluginIface[child.editor()];
	if (!plugin) {
		return false;
	}

	QString parentDiagram = parent.diagram();
	QString parentElement = parent.element();
	if (parent.idSize() == 1) { // seems like it came from plugin's getTypesContainedBy()
		parentDiagram = child.diagram();
		parentElement = parent.editor();
	}

	return isParentOf(plugin, child.diagram(), child.element(), parentDiagram, parentElement);
}

bool EditorManager::isParentOf(const Metamodel *plugin, const QString &childDiagram
		, const QString &child, const QString &parentDiagram, const QString &parent) const
{
	if (child == parent && childDiagram == parentDiagram) {
		return true;
	}

	typedef QPair<QString, QString> StringPair;
	QList<QPair<QString, QString> > list = plugin->getParentsOf(childDiagram, child);

	bool res = false;
	foreach (const StringPair &pair, list) {
		if (pair.second == parent && pair.first == parentDiagram) {
			return true;
		}
		res = res || isParentOf(plugin, pair.first, pair.second, parentDiagram, parent);
	}

	return res;
}

QStringList EditorManager::allChildrenTypesOf(const Id &parent) const
{
	const Metamodel *plugin = mPluginIface[parent.editor()];
	if (!plugin) {
		return QStringList();
	}

	QStringList result;

	foreach (const Id &id, elements(parent)) {
		if (isParentOf(id, parent)) {
			result << id.element();
		}
	}
	return result;
}

QList<Explosion> EditorManager::explosions(const Id &source) const
{
	Q_ASSERT(mPluginsLoaded.contains(source.editor()));
	const Metamodel *plugin = mPluginIface[source.editor()];
	QList<Explosion> result;
	QList<Metamodel::ExplosionData> const rawExplosions =
			plugin->explosions(source.diagram(), source.element());
	foreach (const Metamodel::ExplosionData &rawExplosion, rawExplosions) {
		const Id target(source.editor(), rawExplosion.targetDiagram, rawExplosion.targetElement, "");
		result << Explosion(source, target, rawExplosion.isReusable, rawExplosion.requiresImmediateLinkage);
	}
	return result;
}

bool EditorManager::isGraphicalElementNode(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	ElementType *impl = mPluginIface[id.editor()]->getGraphicalObject(id.diagram(), id.element());
	if (!impl) {
		return false;
	}
	return impl->isNode();
}

Id EditorManager::theOnlyDiagram() const
{
	const IdList allEditors(editors());
	return (allEditors.length() == 1 && diagrams(allEditors[0]).length() == 1)
			? diagrams(allEditors[0])[0] : Id();
}

QString EditorManager::diagramNodeNameString(const Id &editor, const Id &diagram) const
{
	const QString diagramNodeName = metamodel(editor.editor())->diagramNodeName(diagram.diagram());
	return QString("qrm:/%1/%2/%3").arg(editor.editor(), diagram.diagram(), diagramNodeName);
}

QList<QString> EditorManager::getPatternNames() const
{
	return mGroups.keys();
}

Pattern EditorManager::getPatternByName(const QString &str) const
{
	return mGroups.value(str);
}

IdList EditorManager::groups(const Id &diagram)
{
	IdList elements;
	PatternParser parser;
	parser.loadXml((mPluginIface.value(diagram.editor()))->getGroupsXML());
	parser.parseGroups(this, diagram.editor(), diagram.diagram());
	for (const Pattern &pattern : parser.patterns()) {
		mGroups.insert(pattern.name(), pattern);
	}

	for (const QString &group : mGroups.keys()) {
		if (diagram.diagram() == mGroups[group].diagram()) {
			elements.append(Id(diagram.editor(), diagram.diagram(), group));
		}
	}

	return elements;
}

//QList<StringPossibleEdge> EditorManager::possibleEdges(const QString &editor, const QString &element) const
//{
//	return metamodel(editor)->getPossibleEdges(element);
//}

QStringList EditorManager::elements(const QString &editor, const QString &diagram) const
{
	return metamodel(editor)->elements(diagram);
}

int EditorManager::isNodeOrEdge(const QString &editor, const QString &element) const
{
	return metamodel(editor)->isNodeOrEdge(element);
}

bool EditorManager::isParentOf(const QString &editor, const QString &parentDiagram, const QString &parentElement
		, const QString &childDiagram, const QString &childElement) const
{
	return metamodel(editor)->isParentOf(parentDiagram, parentElement, childDiagram, childElement);
}

QString EditorManager::diagramName(const QString &editor, const QString &diagram) const
{
	return metamodel(editor)->diagramName(diagram);
}

QString EditorManager::diagramNodeName(const QString &editor, const QString &diagram) const
{
	return metamodel(editor)->diagramNodeName(diagram);
}

bool EditorManager::isInterpretationMode() const
{
	return false;
}

bool EditorManager::isParentProperty(const Id &id, const QString &propertyName) const
{
	Q_UNUSED(id);
	Q_UNUSED(propertyName);
	return false;
}

void EditorManager::deleteProperty(const QString &propDisplayedName) const
{
	Q_UNUSED(propDisplayedName);
}

void EditorManager::addProperty(const Id &id, const QString &propDisplayedName) const
{
	Q_UNUSED(id);
	Q_UNUSED(propDisplayedName);
}

void EditorManager::updateProperties(const Id &id, const QString &property, const QString &propertyType
		, const QString &propertyDefaultValue, const QString &propertyDisplayedName) const
{
	Q_UNUSED(id);
	Q_UNUSED(property);
	Q_UNUSED(propertyType);
	Q_UNUSED(propertyDefaultValue);
	Q_UNUSED(propertyDisplayedName);
}

QString EditorManager::propertyNameByDisplayedName(const Id &id, const QString &displayedPropertyName) const
{
	Q_UNUSED(id);
	Q_UNUSED(displayedPropertyName);
	return "";
}

IdList EditorManager::children(const Id &parent) const
{
	Q_UNUSED(parent);
	return IdList();
}

QString EditorManager::shape(const Id &id) const
{
	Q_UNUSED(id);
	return "";
}

void EditorManager::updateShape(const Id &id, const QString &graphics) const
{
	Q_UNUSED(id);
	Q_UNUSED(graphics);
}

void EditorManager::resetIsHidden(const Id &id) const
{
	Q_UNUSED(id);
}

QString EditorManager::getIsHidden(const Id &id) const
{
	Q_UNUSED(id);
	return "false";
}

void EditorManager::deleteElement(const Id &id) const
{
	Q_UNUSED(id);
}

bool EditorManager::isRootDiagramNode(const Id &id) const
{
	Q_UNUSED(id);
	return false;
}

void EditorManager::addNodeElement(const Id &diagram, const QString &name, const QString &displayedName
		, bool isRootDiagramNode) const
{
	Q_UNUSED(diagram);
	Q_UNUSED(name);
	Q_UNUSED(displayedName);
	Q_UNUSED(isRootDiagramNode);
}

void EditorManager::addEdgeElement(const Id &diagram, const QString &name, const QString &displayedName
		, const QString &labelText, const QString &labelType, const QString &lineType
		, const QString &beginType, const QString &endType) const
{
	Q_UNUSED(diagram);
	Q_UNUSED(name);
	Q_UNUSED(displayedName);
	Q_UNUSED(labelText);
	Q_UNUSED(labelType);
	Q_UNUSED(lineType);
	Q_UNUSED(beginType);
	Q_UNUSED(endType);
}

QPair<Id, Id> EditorManager::createEditorAndDiagram(const QString &name) const
{
	Q_UNUSED(name);
	return QPair<Id, Id>();
}

void EditorManager::saveMetamodel(const QString &newMetamodelFileName)
{
	Q_UNUSED(newMetamodelFileName);
}

QString EditorManager::saveMetamodelFilePath() const
{
	return "";
}

IdList EditorManager::elementsWithTheSameName(const Id &diagram, const QString &name, const QString type) const
{
	Q_UNUSED(diagram);
	Q_UNUSED(name);
	Q_UNUSED(type);
	return IdList();
}

IdList EditorManager::propertiesWithTheSameName(const Id &id, const QString &propertyCurrentName
		, const QString &propertyNewName) const
{
	Q_UNUSED(id);
	Q_UNUSED(propertyCurrentName);
	Q_UNUSED(propertyNewName);
	return IdList();
}

void EditorManager::updateGenerationRule(const Id &id, const QString &newRule) const
{
	Q_UNUSED(id);
	Q_UNUSED(newRule);
}

QString EditorManager::generationRule(const Id &id) const
{
	Q_UNUSED(id);
	return QString();
}

QStringList EditorManager::getPropertiesInformation(const Id &id) const
{
	Q_UNUSED(id);
	return QStringList();
}

QStringList EditorManager::getSameNamePropertyParams(const Id &propertyId, const QString &propertyName) const
{
	Q_UNUSED(propertyId);
	Q_UNUSED(propertyName);
	return QStringList();
}

void EditorManager::restoreRemovedProperty(const Id &propertyId, const QString &previousName) const
{
	Q_UNUSED(propertyId);
	Q_UNUSED(previousName);
}
void EditorManager::restoreRenamedProperty(const Id &propertyId, const QString &previousName) const
{
	Q_UNUSED(propertyId);
	Q_UNUSED(previousName);
}

void EditorManager::setElementEnabled(const Id &type, bool enabled)
{
	if (enabled) {
		mDisabledElements.remove(type);
	} else {
		mDisabledElements.insert(type);
	}
}
