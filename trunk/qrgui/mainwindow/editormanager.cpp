#include "editormanager.h"

#include <QtCore/QPluginLoader>
#include <QtCore/QCoreApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>

#include <QtCore/QtDebug>

#include "editorinterface.h"
#include "../kernel/ids.h"

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
				foreach (QString d, iEditor->diagrams())
					foreach (QString e, iEditor->elements(d))
					;

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
	return mPluginIface[id.editor()]->getGraphicalObject(id.diagram(), id.element());
}
