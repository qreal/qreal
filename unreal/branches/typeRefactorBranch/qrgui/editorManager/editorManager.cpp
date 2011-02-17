#include "editorManager.h"

#include <QtCore/QCoreApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>

#include <QtCore/QtDebug>

#include "../kernel/NewType.h"
#include "../kernel/exception/exception.h"
#include "../../qrrepo/repoApi.h"
#include "../umllib/uml_nodeelement.h"
#include "../umllib/uml_edgeelement.h"

using namespace qReal;

EditorManager::EditorManager(QObject *parent)
	: QObject(parent), mRoot()
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
		mPluginsLoaded.removeAll(pluginName);
		mPluginFileName.remove(pluginName);
		return loader->unload();
	}
	return false;
}

TypeList EditorManager::editors() const
{
        TypeList editors;
	foreach (QString e, mPluginsLoaded) {
                editors.append(NewType(e));
	}
	return editors;
}

TypeList EditorManager::diagrams(const NewType &editor) const
{
        TypeList diagrams;
	Q_ASSERT(mPluginsLoaded.contains(editor.editor()));

	foreach (QString e, mPluginIface[editor.editor()]->diagrams()) {
                diagrams.append(NewType(editor, e));
	}
	return diagrams;
}

TypeList EditorManager::elements(const NewType &diagram) const
{
        TypeList elements;
	Q_ASSERT(mPluginsLoaded.contains(diagram.editor()));

	foreach (QString e, mPluginIface[diagram.editor()]->elements(diagram.diagram())) {
                elements.append(NewType(diagram, e));
	}
	return elements;
}

TypeList EditorManager::elementsOnDiagram(const NewType &diagram) const
{
	Q_ASSERT(mPluginsLoaded.contains(diagram.editor()));

        TypeList elements;

	foreach (QString e, mPluginIface[diagram.editor()]->elements(diagram.diagram()))
                elements.append(NewType(diagram.editor(), diagram.diagram(), e));

	return elements;
}
bool EditorManager::isEditor(const NewType &type) const
{
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));
        return type.typeSize() == 1;
}

bool EditorManager::isDiagram(const NewType &type) const
{
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));
        return type.typeSize() == 2;
}

bool EditorManager::isElement(const NewType &type) const
{
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));
        return type.typeSize() == 3;
}

QString EditorManager::friendlyName(const NewType &type) const
{
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));

        switch (type.typeSize()) {
	case 1:
                return mPluginIface[type.editor()]->editorName();
	case 2:
                return mPluginIface[type.editor()]->diagramName(type.diagram());
	case 3:
                return mPluginIface[type.editor()]->elementName(type.diagram(), type.element());
	default:
		Q_ASSERT(!"Malformed Id");
		return "";
	}
}

QString EditorManager::description(const NewType &type) const
{
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));
        if (type.typeSize() != 3)
		return "";
        return mPluginIface[type.editor()]->elementDescription(type.diagram(), type.element());
}

QString EditorManager::propertyDescription(const NewType &type, const QString &propertyName) const
{
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));

        if (type.typeSize() != 4)
		return "";
        return mPluginIface[type.editor()]->propertyDescription(type.diagram(), type.element(), propertyName);
}

QString EditorManager::mouseGesture(const NewType &type) const
{
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));
        if (type.typeSize() != 3)
		return "";
        return mPluginIface[type.editor()]->elementMouseGesture(type.diagram(), type.element());
}

QIcon EditorManager::icon(const NewType &type) const
{
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));
        SdfIconEngineV2 *engine = new SdfIconEngineV2(":/" + type.element() + "Class.sdf");
	// QIcon will take ownership of engine, no need for us to delete
        return mPluginIface[type.editor()]->getIcon(engine);
}

UML::Element* EditorManager::graphicalObject(const NewType &type) const
{
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));
        UML::ElementImpl *impl = mPluginIface[type.editor()]->getGraphicalObject(type.diagram(), type.element());
	if( !impl ){
		qDebug() << "no impl";
		return 0;
	}
	if (impl->isNode())
		return new UML::NodeElement(impl);

	return  new UML::EdgeElement(impl);
}

QStringList EditorManager::getPropertyNames(const NewType &type) const
{
        Q_ASSERT(type.typeSize() == 3); // Applicable only to element types
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));
        return mPluginIface[type.editor()]->getPropertyNames(type.diagram(), type.element());
}

TypeList EditorManager::getContainedTypes(const NewType &type) const
{
        Q_ASSERT(type.typeSize() == 3);  // Applicable only to element types
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));

        TypeList result;
        foreach (QString typeString, mPluginIface[type.editor()]->getTypesContainedBy(type.element()))
	{
                result.append(NewType(typeString));
	}
	return result;
}

TypeList EditorManager::getConnectedTypes(const NewType &type) const
{
        Q_ASSERT(type.typeSize() == 3);  // Applicable only to element types

        Q_ASSERT(mPluginsLoaded.contains(type.editor()));

        TypeList result;
        foreach (QString typeString, mPluginIface[type.editor()]->getConnectedTypes(type.element()))
		// a hack caused by absence  of ID entity in editors generator
                result.append(NewType("?", "?", typeString));

	return result;
}

TypeList EditorManager::getUsedTypes(const NewType &type) const
{
        Q_ASSERT(type.typeSize() == 3);  // Applicable only to element types

        Q_ASSERT(mPluginsLoaded.contains(type.editor()));

        TypeList result;
        foreach (QString typeString, mPluginIface[type.editor()]->getUsedTypes(type.element()))
                result.append(NewType("?", "?", typeString));

	return result;
}

QStringList EditorManager::getEnumValues(NewType const &type, const QString &name) const
{
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));
        QString typeName = mPluginIface[type.editor()]->getPropertyType(type.element(), name);
        return mPluginIface[type.editor()]->getEnumValues(typeName);
}

QString EditorManager::getTypeName(const NewType &type, const QString &name) const
{
        return mPluginIface[type.editor()]->getPropertyType(type.element(), name);
}

QString EditorManager::getDefaultPropertyValue(NewType const &type, QString name) const
{
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));
        return mPluginIface[type.editor()]->getPropertyDefaultValue(type.element(), name);
}

QStringList EditorManager::getPropertiesWithDefaultValues(NewType const &type) const
{
        Q_ASSERT(mPluginsLoaded.contains(type.editor()));
        return mPluginIface[type.editor()]->getPropertiesWithDefaultValues(type.element());
}

TypeList EditorManager::checkNeededPlugins(qrRepo::RepoApi const &api) const
{
        TypeList result;
        checkNeededPluginsRecursive(api, ROOT_ID, result);
	return result;
}

void EditorManager::checkNeededPluginsRecursive(qrRepo::RepoApi const &api, Id const &id, TypeList &result) const
{
        if (id != ROOT_ID && !mPluginsLoaded.contains(api.type(id).editor())) {
                NewType missingEditor = NewType(api.type(id).editor());
		if (!result.contains(missingEditor))
			result.append(missingEditor);
	}

        foreach (Id child, api.children(id)) {
                checkNeededPluginsRecursive(api, child, result);
	}
}

bool EditorManager::hasElement(NewType const &type) const
{
        Q_ASSERT(type.typeSize() == 3);
        if (!mPluginsLoaded.contains(type.editor()))
		return false;
        EditorInterface *editor = mPluginIface[type.editor()];
	foreach (QString diagram, editor->diagrams())
		foreach (QString element, editor->elements(diagram))
                        if (type.diagram() == diagram && type.element() == element)
				return true;
	return false;
}

NewType EditorManager::findElementByType(QString const &type) const
{
	foreach (EditorInterface *editor, mPluginIface.values())
		foreach (QString diagram, editor->diagrams())
			foreach (QString element, editor->elements(diagram))
				if (type == element)
                                        return NewType(editor->editorName(), diagram, element);
        throw Exception("No type " + type + " in loaded plugins");
}

QList<Listener*> EditorManager::listeners() const
{
	QList<Listener*> result;
	foreach (EditorInterface *editor, mPluginIface.values()){
		QList<ListenerInterface*> tmp = editor->listeners();
		foreach (ListenerInterface *iface, tmp){
			Listener *listener = dynamic_cast<Listener*>(iface);
			result.append(listener);
		}
	}
	return result;
}

EditorInterface* EditorManager::getEditorInterface(QString editor) const
{
	return mPluginIface[editor];
}
