#include "editorManager.h"

#include <QtCore/QPluginLoader>
#include <QtCore/QCoreApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>

#include <QtCore/QtDebug>

#include "editorInterface.h"
#include "../kernel/ids.h"
#include "../kernel/exception/exception.h"
#include "../../qrrepo/repoApi.h"
#include "../umllib/uml_nodeelement.h"
#include "../umllib/uml_edgeelement.h"

using namespace qReal;

EditorManager::EditorManager(QObject *parent)
	: QObject(parent), mRoot()
{
	//    foreach (QObject *plugin, QPluginLoader::staticInstances())
	//        populateMenus(plugin);

	mPluginsDir = QDir(qApp->applicationDirPath());

	while (!mPluginsDir.isRoot() && !mPluginsDir.entryList(QDir::Dirs).contains("plugins")) {
		mPluginsDir.cdUp();
	}

	mPluginsDir.cd("plugins");

	foreach (QString fileName, mPluginsDir.entryList(QDir::Files)) {
		QPluginLoader loader(mPluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();

		if (plugin) {
			EditorInterface *iEditor = qobject_cast<EditorInterface *>(plugin);
			if (iEditor) {
				mPluginsLoaded += iEditor->id();
				mPluginIface[iEditor->id()] = iEditor;
			}
		} else {
			qDebug() << "Plugin loading failed: " << loader.errorString();
			// Keep silent.
			// QMessageBox::warning(0, "QReal Plugin", loader.errorString() );
		}
	}
}

QList<Id> EditorManager::editors() const
{
	QList<Id> editors;
	foreach (QString e, mPluginsLoaded) {
		editors.append(Id(e));
	}
	return editors;
}

QList<Id> EditorManager::diagrams(const Id &editor) const
{
	QList<Id> diagrams;
	Q_ASSERT(mPluginsLoaded.contains(editor.editor()));

	foreach (QString e, mPluginIface[editor.editor()]->diagrams()) {
		diagrams.append(Id(editor, e));
	}
	return diagrams;
}

QList<Id> EditorManager::elements(const Id &diagram) const
{
	QList<Id> elements;
	Q_ASSERT(mPluginsLoaded.contains(diagram.editor()));

	foreach (QString e, mPluginIface[diagram.editor()]->elements(diagram.diagram())) {
		elements.append(Id(diagram, e));
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
			return mPluginIface[id.editor()]->elementName(id.diagram(), id.element());
		default:
			Q_ASSERT(!"Malformed Id");
			return "";
	}
}

QIcon EditorManager::icon(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return mPluginIface[id.editor()]->getIcon(id.diagram(), id.element());
}

UML::Element* EditorManager::graphicalObject(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	UML::ElementImpl *impl = mPluginIface[id.editor()]->getGraphicalObject(id.diagram(), id.element());
	if( !impl ){
		qDebug() << "no impl";
		return 0;
	}	
	UML::Element *el;
	if (impl->isNode()) 
		el = new UML::NodeElement(impl);
	else
		el = new UML::EdgeElement(impl);
	return el;	
}

QStringList EditorManager::getPropertyNames(const Id &id) const
{
	Q_ASSERT(id.idSize() == 3);  // Операция применима только к типам элементов
	Q_ASSERT(mPluginsLoaded.contains(id.editor()));
	return mPluginIface[id.editor()]->getPropertyNames(id.diagram(), id.element());
}

IdList EditorManager::getContainedTypes(const Id &id) const
{
	Q_ASSERT(id.idSize() == 3);  // Операция применима только к типам элементов
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
	Q_ASSERT(id.idSize() == 3);  // Операция применима только к типам элементов

	Q_ASSERT(mPluginsLoaded.contains(id.editor()));

	IdList result;
	foreach (QString type, mPluginIface[id.editor()]->getConnectedTypes(id.element()))
		// Хак, связанный с отсутствием понятия "Id" в генераторе редакторов.
		result.append(Id("?", "?", type));

	return result;
}

IdList EditorManager::checkNeededPlugins(qrRepo::RepoApi const &api) const
{
	IdList result;
	checkNeededPluginsRecursive(api, ROOT_ID, result);
	return result;
}

void EditorManager::checkNeededPluginsRecursive(qrRepo::RepoApi const &api, Id const &id, IdList &result) const
{
	if (id != ROOT_ID && !mPluginsLoaded.contains(id.editor())) {
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

QList<Listener*> EditorManager::listeners() const
{
	QList<Listener*> result;
	foreach (EditorInterface *editor, mPluginIface.values())
		result.append(editor->listeners());
	return result;
}
