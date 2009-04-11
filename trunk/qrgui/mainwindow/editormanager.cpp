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
	: QObject(parent), mRoot(IdParser::createId())
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
		editors.append(IdParser::append(mRoot, e));
	}
	return editors;
}

QList<Id> EditorManager::diagrams(const Id &editor) const
{
	QList<Id> diagrams;
	QString editorName = IdParser::getEditor(editor);
	Q_ASSERT(mPluginsLoaded.contains(editorName));

	foreach (QString e, mPluginIface[editorName]->diagrams()) {
		diagrams.append(IdParser::append(editor, e));
	}
	return diagrams;
}

QList<Id> EditorManager::elements(const Id &diagram) const
{
	QList<Id> elements;
	QString editor = IdParser::getEditor(diagram);
	Q_ASSERT(mPluginsLoaded.contains(editor));

	foreach (QString e, mPluginIface[editor]->elements(IdParser::getDiagram(diagram))) {
		elements.append(IdParser::append(diagram, e));
	}
	return elements;
}

bool EditorManager::isEditor(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(IdParser::getEditor(id)));
	return IdParser::getIdSize(id) == 1;
}

bool EditorManager::isDiagram(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(IdParser::getEditor(id)));
	return IdParser::getIdSize(id) == 2;
}

bool EditorManager::isElement(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(IdParser::getEditor(id)));
	return IdParser::getIdSize(id) == 3;
}

QString EditorManager::friendlyName(const Id &id) const
{
	QString editor = IdParser::getEditor(id);
	Q_ASSERT(mPluginsLoaded.contains(editor));

	switch (IdParser::getIdSize(id)) {
		case 2:
			return mPluginIface[editor]->editorName();
		case 3:
			return mPluginIface[editor]->diagramName(IdParser::getDiagram(id));
		case 4:
			return mPluginIface[editor]->elementName(IdParser::getDiagram(id), IdParser::getElement(id));
		default:
			Q_ASSERT(!"Malformed Id");
			return "";
	}
}

QIcon EditorManager::icon(const Id &id) const
{
	Q_ASSERT(mPluginsLoaded.contains(IdParser::getEditor(id)));
	return mPluginIface[IdParser::getEditor(id)]->getIcon(IdParser::getDiagram(id), IdParser::getElement(id));
}

UML::Element* EditorManager::graphicalObject(const Id &typeId) const
{
	Q_ASSERT(mPluginsLoaded.contains(IdParser::getEditor(typeId)));
	return mPluginIface[IdParser::getEditor(typeId)]->getGraphicalObject(IdParser::getDiagram(typeId), IdParser::getElement(typeId));
}
