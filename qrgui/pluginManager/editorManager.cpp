#include "editorManager.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QMessageBox>
#include <QtGui/QIcon>

#include <qrkernel/ids.h>
#include <qrkernel/exception/exception.h>
#include <qrrepo/repoApi.h>

#include "umllib/nodeElement.h"
#include "umllib/edgeElement.h"

using namespace qReal;

EditorManager::EditorManager(QObject *parent) : QObject(parent)
{
	mPluginsDir = QDir(qApp->applicationDirPath());

	while (!mPluginsDir.isRoot() && !mPluginsDir.entryList(QDir::Dirs).contains("plugins")) {
		mPluginsDir.cdUp();
	}

	mPluginsDir.cd("plugins");

	foreach (QString const &fileName, mPluginsDir.entryList(QDir::Files)) {
		QPluginLoader *loader  = new QPluginLoader(mPluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader->instance();

		if (plugin) {
			EditorInterface *iEditor = qobject_cast<EditorInterface *>(plugin);
			if (iEditor) {
				mPluginsLoaded += iEditor->id();
				mPluginFileName.insert(iEditor->id(), fileName);
				mPluginIface[iEditor->id()] = iEditor;
				mLoaders.insert(fileName, loader);
			} else {
				// TODO: Just does not work under Linux. Seems to be memory corruption when
				// loading, unloading, and then loading .so file again.
				// To reproduce, uncomment this, build VisualInterpreter, and try to launch QReal.
				// With some tool plugins, like MetaEditorSupport or Exterminatus, works fine,
				// also works fine on Windows. Investigation required.
				// loader->unload();
				delete loader;
			}
		} else {
			qDebug() << "Plugin loading failed: " << loader->errorString();
			loader->unload();
			delete loader;
		}
	}
}

EditorManager::~EditorManager()
{
	foreach (QString const &id, mPluginIface.keys()) {
		delete mPluginIface[id];
		mPluginIface.remove(id);
	}

	foreach (QString const &name, mLoaders.keys()) {
		delete mLoaders[name];
		mLoaders.remove(name);
	}
}

bool EditorManager::loadPlugin(QString const &pluginName)
{
	QPluginLoader *loader = new QPluginLoader(mPluginsDir.absoluteFilePath(pluginName));
	loader->load();
	QObject *plugin = loader->instance();

	if (plugin) {
		EditorInterface *iEditor = qobject_cast<EditorInterface *>(plugin);
		if (iEditor) {
			mPluginsLoaded += iEditor->id();
			mPluginFileName.insert(iEditor->id(), pluginName);
			mPluginIface[iEditor->id()] = iEditor;
			mLoaders.insert(pluginName, loader);
			return true;
		}
	}

	QMessageBox::warning(NULL, tr("error"), tr("Plugin loading failed: ") + loader->errorString());
	loader->unload();
	delete loader;
	return false;
}

bool EditorManager::unloadPlugin(QString const &pluginName)
{
	QPluginLoader *loader = mLoaders[mPluginFileName[pluginName]];
	if (loader != NULL) {
		mLoaders.remove(mPluginFileName[pluginName]);
		mPluginIface.remove(pluginName);
		mPluginFileName.remove(pluginName);
		mPluginsLoaded.removeAll(pluginName);
		if (!loader->unload()) {
			QMessageBox::warning(NULL, tr("error"), tr("Plugin unloading failed: ") + loader->errorString());
			delete loader;
			return false;
		}
		delete loader;
		return true;
	}
	return false;
}

IdList EditorManager::editors() const
{
	IdList editors;
	foreach (QString const &editor, mPluginsLoaded) {
		editors.append(Id(editor));
	}
	return editors;
}

IdList EditorManager::diagrams(Id const &editor) const
{
	IdList diagrams;
	Q_ASSERT(mPluginsLoaded.contains(editor.editor()));

	foreach (QString const &diagram, mPluginIface[editor.editor()]->diagrams()) {
		diagrams.append(Id(editor, diagram));
	}

	return diagrams;
}

QStringList EditorManager::paletteGroups(Id const &editor, const Id &diagram) const
{
	Q_ASSERT(mPluginsLoaded.contains(diagram.editor()));
	return mPluginIface[editor.editor()]->diagramPaletteGroups(diagram.diagram());
}

QStringList EditorManager::paletteGroupList(Id const &editor, const Id &diagram, QString const &group) const
{
	return mPluginIface[editor.editor()]->diagramPaletteGroupList(diagram.diagram(), group);
}

QString EditorManager::paletteGroupDescription(Id const &editor, const Id &diagram, QString const &group) const
{
	return mPluginIface[editor.editor()]->diagramPaletteGroupDescription(diagram.diagram(), group);
}

IdList EditorManager::elements(Id const &diagram) const
{
	IdList elements;
	Q_ASSERT(mPluginsLoaded.contains(diagram.editor()));

	foreach (QString const &e, mPluginIface[diagram.editor()]->elements(diagram.diagram())) {
		elements.append(Id(diagram.editor(), diagram.diagram(), e));
	}
	return elements;
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

QString EditorManager::propertyDescription(const Id &id, QString const &propertyName) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));

	if (id.idSize() != 4) {
		return "";
	}
	return mPluginIface[id.editor()]->propertyDescription(id.diagram(), id.element(), propertyName);
}

QString EditorManager::propertyDisplayedName(Id const &id, QString const &propertyName) const
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
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	SdfIconEngineV2 *engine = new SdfIconEngineV2(":/generated/shapes/" + id.element() + "Class.sdf");
	// QIcon will take ownership of engine, no need for us to delete
	return mPluginIface[id.editor()]->getIcon(engine);
}

QSize EditorManager::iconSize(Id const &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	SdfIconEngineV2 *engine = new SdfIconEngineV2(":/generated/shapes/" + id.element() + "Class.sdf");
	return engine->preferedSize();
}

ElementImpl *EditorManager::elementImpl(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	ElementImpl *impl = mPluginIface[id.editor()]->getGraphicalObject(id.diagram(), id.element());
	if (!impl) {
		qDebug() << "no impl";
		return 0;
	}

	return impl;
}

QStringList EditorManager::propertyNames(const Id &id) const
{
	Q_ASSERT(id.idSize() == 3); // Applicable only to element types
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return mPluginIface[id.editor()]->getPropertyNames(id.diagram(), id.element());
}

QStringList EditorManager::portTypes(Id const &id) const
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
	foreach (QString const &type, mPluginIface[id.editor()]->getTypesContainedBy(id.element())) {
		result.append(Id(id.editor(), id.diagram(), type));
	}

	typedef QPair<QString, QString> StringPair;
	QList<StringPair> const parents = mPluginIface[id.editor()]->getParentsOf(id.diagram(), id.element());

	foreach (StringPair const &pair, parents) {
		result.append(containedTypes(Id(id.editor(), pair.first, pair.second)));
	}

	return result;
}

QStringList EditorManager::enumValues(Id const &id, const QString &name) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	QString typeName = mPluginIface[id.editor()]->getPropertyType(id.element(), name);
	return mPluginIface[id.editor()]->getEnumValues(typeName);
}

QString EditorManager::typeName(const Id &id, const QString &name) const
{
	return mPluginIface[id.editor()]->getPropertyType(id.element(), name);
}

QString EditorManager::defaultPropertyValue(Id const &id, QString name) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return mPluginIface[id.editor()]->getPropertyDefaultValue(id.element(), name);
}

QStringList EditorManager::propertiesWithDefaultValues(Id const &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return mPluginIface[id.editor()]->getPropertiesWithDefaultValues(id.element());
}

IdList EditorManager::checkNeededPlugins(qrRepo::LogicalRepoApi const &logicalApi
		, qrRepo::GraphicalRepoApi const &graphicalApi) const
{
	IdList result;
	checkNeededPluginsRecursive(logicalApi, Id::rootId(), result);
	checkNeededPluginsRecursive(graphicalApi, Id::rootId(), result);
	return result;
}

void EditorManager::checkNeededPluginsRecursive(qrRepo::CommonRepoApi const &api
		, Id const &id, IdList &result) const
{
	if (id != Id::rootId() && !mPluginsLoaded.contains(id.editor())) {
		Id missingEditor = Id(id.editor());
		if (!result.contains(missingEditor)) {
			result.append(missingEditor);
		}
	}

	foreach (Id const &child, api.children(id)) {
		checkNeededPluginsRecursive(api, child, result);
	}
}

bool EditorManager::hasElement(Id const &elementId) const
{
	Q_ASSERT(elementId.idSize() == 3);
	if (!mPluginsLoaded.contains(elementId.editor()))
		return false;
	EditorInterface *editor = mPluginIface[elementId.editor()];
	foreach (QString const &diagram, editor->diagrams()) {
		foreach (QString const &element, editor->elements(diagram)) {
			if (elementId.diagram() == diagram && elementId.element() == element) {
				return true;
			}
		}
	}
	return false;
}

Id EditorManager::findElementByType(QString const &type) const
{
	foreach (EditorInterface *editor, mPluginIface.values()) {
		foreach (QString const &diagram, editor->diagrams()) {
			foreach (QString const &element, editor->elements(diagram)) {
				if (type == element) {
					return Id(editor->id(), diagram, element);
				}
			}
		}
	}
	throw Exception("No type " + type + " in loaded plugins");
}

QList<ListenerInterface*> EditorManager::listeners() const
{
	QList<ListenerInterface*> result;
	foreach (EditorInterface *editor, mPluginIface.values()) {
		result << editor->listeners();
	}
	return result;
}

EditorInterface* EditorManager::editorInterface(QString const &editor) const
{
	return mPluginIface[editor];
}

bool EditorManager::isDiagramNode(Id const &id) const
{
	return id.element() == editorInterface(id.editor())->diagramNodeName(id.diagram());
}

bool EditorManager::isParentOf(Id const &child, Id const &parent) const // child — EnginesForware, parent — AbstractNode
{
	EditorInterface const *plugin = mPluginIface[child.editor()];
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

bool EditorManager::isParentOf(EditorInterface const *plugin, QString const &childDiagram
		, QString const &child, QString const &parentDiagram, QString const &parent) const
{
	if (child == parent && childDiagram == parentDiagram) {
		return true;
	}

	typedef QPair<QString, QString> StringPair;
	QList<QPair<QString, QString> > list = plugin->getParentsOf(childDiagram, child);

	bool res = false;
	foreach (StringPair const &pair, list) {
		if (pair.second == parent && pair.first == parentDiagram) {
			return true;
		}
		res = res || isParentOf(plugin, pair.first, pair.second, parentDiagram, parent);
	}

	return res;
}

QStringList EditorManager::allChildrenTypesOf(Id const &parent) const
{
	EditorInterface const *plugin = mPluginIface[parent.editor()];
	if (!plugin) {
		return QStringList();
	}

	QStringList result;

	foreach (Id const &id, elements(parent)) {
		if (isParentOf(id, parent)) {
			result << id.element();
		}
	}
	return result;
}

QList<Explosion> EditorManager::explosions(Id const &source) const
{
	Q_ASSERT(mPluginsLoaded.contains(source.editor()));
	EditorInterface const *plugin = mPluginIface[source.editor()];
	QList<Explosion> result;
	QList<EditorInterface::ExplosionData> const rawExplosions =
			plugin->explosions(source.diagram(), source.element());
	foreach (EditorInterface::ExplosionData const &rawExplosion, rawExplosions) {
		Id const target(source.editor(), rawExplosion.targetDiagram, rawExplosion.targetElement, "");
		result << Explosion(source, target, rawExplosion.isReusable, rawExplosion.requiresImmediateLinkage);
	}
	return result;
}

bool EditorManager::isGraphicalElementNode(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	ElementImpl *impl = mPluginIface[id.editor()]->getGraphicalObject(id.diagram(), id.element());
	if (!impl) {
		return false;
	}
	return impl->isNode();
}

Id EditorManager::theOnlyDiagram() const
{
	IdList const allEditors(editors());
	return (allEditors.length() == 1 && diagrams(allEditors[0]).length() == 1)
			? diagrams(allEditors[0])[0] : Id();
}

QString EditorManager::diagramNodeNameString(Id const &editor, Id const &diagram) const
{
	QString const diagramNodeName = editorInterface(editor.editor())->diagramNodeName(diagram.diagram());
	return QString("qrm:/%1/%2/%3").arg(editor.editor(), diagram.diagram(), diagramNodeName);
}

QList<QString> EditorManager::getPatternNames() const
{
	return mGroups.keys();
}

Pattern EditorManager::getPatternByName(QString const &str) const
{
	return mGroups.value(str);
}

IdList EditorManager::groups(Id const &diagram)
{
	IdList elements;
	PatternParser parser;
	parser.loadXml((mPluginIface.value(diagram.editor()))->getGroupsXML());
	parser.parseGroups(this, diagram.editor(), diagram.diagram());
	foreach(Pattern const &pattern, parser.getPatterns()) {
		mGroups.insert(pattern.name(), pattern);
	}

	foreach (QString const &group, mGroups.keys()) {
		elements.append(Id(diagram.editor(), diagram.diagram(), group));
	}

	return elements;
}

QList<StringPossibleEdge> EditorManager::possibleEdges(QString const &editor, QString const &element) const
{
	return editorInterface(editor)->getPossibleEdges(element);
}

QStringList EditorManager::elements(QString const &editor, QString const &diagram) const
{
	return editorInterface(editor)->elements(diagram);
}

int EditorManager::isNodeOrEdge(QString const &editor, QString const &element) const
{
	return editorInterface(editor)->isNodeOrEdge(element);
}

bool EditorManager::isParentOf(QString const &editor, QString const &parentDiagram, QString const &parentElement
		, QString const &childDiagram, QString const &childElement) const
{
	return editorInterface(editor)->isParentOf(parentDiagram, parentElement, childDiagram, childElement);
}

QString EditorManager::diagramName(QString const &editor, QString const &diagram) const
{
	return editorInterface(editor)->diagramName(diagram);
}

QString EditorManager::diagramNodeName(QString const &editor, QString const &diagram) const
{
	return editorInterface(editor)->diagramNodeName(diagram);
}

bool EditorManager::isInterpretationMode() const
{
	return false;
}

bool EditorManager::isParentProperty(Id const &id, QString const &propertyName) const
{
	Q_UNUSED(id);
	Q_UNUSED(propertyName);
	return false;
}

void EditorManager::deleteProperty(QString const &propDisplayedName) const
{
	Q_UNUSED(propDisplayedName);
}

void EditorManager::addProperty(Id const &id, QString const &propDisplayedName) const
{
	Q_UNUSED(id);
	Q_UNUSED(propDisplayedName);
}

void EditorManager::updateProperties(Id const &id, QString const &property, QString const &propertyType
		, QString const &propertyDefaultValue, QString const &propertyDisplayedName) const
{
	Q_UNUSED(id);
	Q_UNUSED(property);
	Q_UNUSED(propertyType);
	Q_UNUSED(propertyDefaultValue);
	Q_UNUSED(propertyDisplayedName);
}

QString EditorManager::propertyNameByDisplayedName(Id const &id, QString const &displayedPropertyName) const
{
	Q_UNUSED(id);
	Q_UNUSED(displayedPropertyName);
	return "";
}

IdList EditorManager::children(Id const &parent) const
{
	Q_UNUSED(parent);
	return IdList();
}

QString EditorManager::shape(Id const &id) const
{
	Q_UNUSED(id);
	return "";
}

void EditorManager::updateShape(Id const &id, QString const &graphics) const
{
	Q_UNUSED(id);
	Q_UNUSED(graphics);
}

void EditorManager::deleteElement(MainWindow *mainWindow, Id const &id) const
{
	Q_UNUSED(mainWindow);
	Q_UNUSED(id);
}

bool EditorManager::isRootDiagramNode(Id const &id) const
{
	Q_UNUSED(id);
	return false;
}

void EditorManager::addNodeElement(Id const &diagram, QString const &name, bool isRootDiagramNode) const
{
	Q_UNUSED(diagram);
	Q_UNUSED(name);
	Q_UNUSED(isRootDiagramNode);
}

void EditorManager::addEdgeElement(Id const &diagram, QString const &name, QString const &labelText
		, QString const &labelType, QString const &lineType, QString const &beginType, QString const &endType) const
{
	Q_UNUSED(diagram);
	Q_UNUSED(name);
	Q_UNUSED(labelText);
	Q_UNUSED(labelType);
	Q_UNUSED(lineType);
	Q_UNUSED(beginType);
	Q_UNUSED(endType);
}

QPair<Id, Id> EditorManager::createEditorAndDiagram(QString const &name) const
{
	Q_UNUSED(name);
	return QPair<Id, Id>();
}

void EditorManager::saveMetamodel(QString const &newMetamodelFileName)
{
	Q_UNUSED(newMetamodelFileName);
}

QString EditorManager::saveMetamodelFilePath() const
{
	return "";
}
