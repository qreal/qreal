#include "editorManager.h"

#include <QtCore/QCoreApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>

#include <QtCore/QtDebug>

#include "../../qrkernel/ids.h"

#include "../../qrrepo/repoApi.h"
#include "../umllib/nodeElement.h"
#include "../umllib/edgeElement.h"
#include "../../qrkernel/exception/exception.h"

using namespace qReal;

EditorManager::EditorManager(QObject *parent)
	: QObject(parent)
{
	mPluginsDir = QDir(qApp->applicationDirPath());

	while (!mPluginsDir.isRoot() && !mPluginsDir.entryList(QDir::Dirs).contains("plugins")) {
		mPluginsDir.cdUp();
	}

	mPluginsDir.cd("plugins");

	foreach (QString fileName, mPluginsDir.entryList(QDir::Files)) {
		QPluginLoader *loader  = new QPluginLoader(mPluginsDir.absoluteFilePath(fileName));
		mLoaders.insert(fileName, loader);
		QObject *plugin = loader->instance();

		if (plugin) {
			EditorInterface *iEditor = qobject_cast<EditorInterface *>(plugin);
			if (iEditor) {
				mPluginsLoaded += iEditor->id();
				mPluginFileName.insert(iEditor->id(), fileName);
				mPluginIface[iEditor->id()] = iEditor;
			}
		} else {
			qDebug() << "Plugin loading failed: " << loader->errorString();
			// Keep silent.
			// QMessageBox::warning(0, "QReal Plugin", loader->errorString() );
		}
	}
}

bool EditorManager::loadPlugin(const QString &pluginName)
{
	QPluginLoader *loader = new QPluginLoader(mPluginsDir.absoluteFilePath(pluginName));
	mLoaders.insert(pluginName, loader);
	QObject *plugin = loader->instance();

	if (plugin) {
		EditorInterface *iEditor = qobject_cast<EditorInterface *>(plugin);
		if (iEditor) {
			mPluginsLoaded += iEditor->id();
			mPluginFileName.insert(iEditor->id(), pluginName);
			mPluginIface[iEditor->id()] = iEditor;
			return true;
		}
	}
	QMessageBox::warning(0, "QReal Plugin", loader->errorString());
	return false;
}

bool EditorManager::unloadPlugin(const QString &pluginName)
{
	QPluginLoader *loader = mLoaders[mPluginFileName[pluginName]];
	if (loader != NULL) {
		if (!(loader->unload())) {
			return false;
		}
		mPluginsLoaded.removeAll(pluginName);
		mPluginFileName.remove(pluginName);
		return true;
	}
	return false;
}

IdList EditorManager::editors() const
{
	IdList editors;
	foreach (QString e, mPluginsLoaded) {
		editors.append(Id(e));
	}
	return editors;
}

IdList EditorManager::diagrams(const Id &editor) const
{
	IdList diagrams;
	Q_ASSERT(mPluginsLoaded.contains(editor.editor()));

	foreach (QString e, mPluginIface[editor.editor()]->diagrams()) {
		diagrams.append(Id(editor, e));
	}
	return diagrams;
}

IdList EditorManager::elements(const Id &diagram) const
{
	IdList elements;
	Q_ASSERT(mPluginsLoaded.contains(diagram.editor()));

	foreach (QString e, mPluginIface[diagram.editor()]->elements(diagram.diagram()))
		elements.append(Id(diagram.editor(), diagram.diagram(), e));
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
		return mPluginIface[id.editor()]->elementName(id.diagram(), id.element());
	default:
		Q_ASSERT(!"Malformed Id");
		return "";
	}
}

QString EditorManager::description(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	if (id.idSize() != 3)
		return "";
	return mPluginIface[id.editor()]->elementDescription(id.diagram(), id.element());
}

QString EditorManager::propertyDescription(const Id &id, const QString &propertyName) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));

	if (id.idSize() != 4)
		return "";
	return mPluginIface[id.editor()]->propertyDescription(id.diagram(), id.element(), propertyName);
}

QString EditorManager::propertyDisplayedName(Id const &id, QString const &propertyName) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));

	if (id.idSize() != 4)
		return "";
	return mPluginIface[id.editor()]->propertyDisplayedName(id.diagram(), id.element(), propertyName);
}

QString EditorManager::mouseGesture(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	if (id.idSize() != 3)
		return "";
	return mPluginIface[id.editor()]->elementMouseGesture(id.diagram(), id.element());
}

QIcon EditorManager::icon(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	SdfIconEngineV2 *engine = new SdfIconEngineV2(":/generated/shapes/" + id.element() + "Class.sdf");
	// QIcon will take ownership of engine, no need for us to delete
	return mPluginIface[id.editor()]->getIcon(engine);
}

Element* EditorManager::graphicalObject(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	ElementImpl *impl = mPluginIface[id.editor()]->getGraphicalObject(id.diagram(), id.element());
	if( !impl ){
		qDebug() << "no impl";
		return 0;
	}
	if (impl->isNode())
		return new NodeElement(impl);

	return  new EdgeElement(impl);
}

QStringList EditorManager::getPropertyNames(const Id &id) const
{
	Q_ASSERT(id.idSize() == 3); // Applicable only to element types
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return mPluginIface[id.editor()]->getPropertyNames(id.diagram(), id.element());
}

IdList EditorManager::getContainedTypes(const Id &id) const
{
	Q_ASSERT(id.idSize() == 3);  // Applicable only to element types
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));

	IdList result;
	foreach (QString type, mPluginIface[id.editor()]->getTypesContainedBy(id.element()))
	{
		result.append(Id(type));
	}
	return result;
}

IdList EditorManager::getConnectedTypes(const Id &id) const
{
	Q_ASSERT(id.idSize() == 3);  // Applicable only to element types

	Q_ASSERT(mPluginsLoaded.contains(id.editor()));

	IdList result;
	foreach (QString type, mPluginIface[id.editor()]->getConnectedTypes(id.element()))
		// a hack caused by absence  of ID entity in editors generator
		result.append(Id("?", "?", type));

	return result;
}

IdList EditorManager::getUsedTypes(const Id &id) const
{
	Q_ASSERT(id.idSize() == 3);  // Applicable only to element types

	Q_ASSERT(mPluginsLoaded.contains(id.editor()));

	IdList result;
	foreach (QString type, mPluginIface[id.editor()]->getUsedTypes(id.element()))
		result.append(Id("?", "?", type));

	return result;
}

QStringList EditorManager::getEnumValues(Id const &id, const QString &name) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	QString typeName = mPluginIface[id.editor()]->getPropertyType(id.element(), name);
	return mPluginIface[id.editor()]->getEnumValues(typeName);
}

QString EditorManager::getTypeName(const Id &id, const QString &name) const
{
	return mPluginIface[id.editor()]->getPropertyType(id.element(), name);
}

QString EditorManager::getDefaultPropertyValue(Id const &id, QString name) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return mPluginIface[id.editor()]->getPropertyDefaultValue(id.element(), name);
}

QStringList EditorManager::getPropertiesWithDefaultValues(Id const &id) const
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
		if (!result.contains(missingEditor))
			result.append(missingEditor);
	}

	foreach (Id child, api.children(id)) {
		checkNeededPluginsRecursive(api, child, result);
	}
}

bool EditorManager::hasElement(Id const &elementId) const
{
	Q_ASSERT(elementId.idSize() == 3);
	if (!mPluginsLoaded.contains(elementId.editor()))
		return false;
	EditorInterface *editor = mPluginIface[elementId.editor()];
	foreach (QString diagram, editor->diagrams())
		foreach (QString element, editor->elements(diagram))
			if (elementId.diagram() == diagram && elementId.element() == element)
				return true;
	return false;
}

Id EditorManager::findElementByType(QString const &type) const
{
	foreach (EditorInterface *editor, mPluginIface.values())
		foreach (QString diagram, editor->diagrams())
			foreach (QString element, editor->elements(diagram))
				if (type == element)
					return Id(editor->id(), diagram, element);
	throw Exception("No type " + type + " in loaded plugins");
}

QList<ListenerInterface*> EditorManager::listeners() const
{
	QList<ListenerInterface*> result;
	foreach (EditorInterface *editor, mPluginIface.values())
		result << editor->listeners();
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
	if (!plugin)
		return false;

	QString parentDiagram;
	QString parentElement;
	if (parent.idSize() == 1) { // seems like it came from plugin's getTypesContainedBy()
		parentDiagram = child.diagram();
		parentElement = parent.editor();
	}

	return isParentOf(plugin, child.diagram(), child.element(), parentDiagram, parentElement);
}

bool EditorManager::isParentOf(EditorInterface const *plugin, QString const &childDiagram
							   , QString const &child, QString const &parentDiagram, QString const &parent) const
{
	if (child == parent && childDiagram == parentDiagram)
		return true;

	typedef QPair<QString, QString> StringPair;
	QList<QPair<QString, QString> > list = plugin->getParentsOf(childDiagram, child);

	bool res = false;
	foreach (StringPair const pair, list)
	{
		if (pair.second == parent && pair.first == parentDiagram)
			return true;

		res = res || isParentOf(plugin, pair.first, pair.second, parentDiagram, parent);
	}

	return res;

}
