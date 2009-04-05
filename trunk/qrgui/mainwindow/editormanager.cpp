#include <QtCore/QUrl>
#include <QtCore/QPluginLoader>
#include <QtCore/QCoreApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>

#include <QtCore/QtDebug>

#include "editormanager.h"
#include "editorinterface.h"

EditorManager::EditorManager(QObject *parent)
	: QObject(parent), root("qrm:/")
{
	//    foreach (QObject *plugin, QPluginLoader::staticInstances())
	//        populateMenus(plugin);

	pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
	if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
		pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
	if (pluginsDir.dirName() == "MacOS") {
		pluginsDir.cdUp();
		pluginsDir.cdUp();
		pluginsDir.cdUp();
	}
#endif
	pluginsDir.cd("plugins");

	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();

		if (plugin) {
			EditorInterface *iEditor = qobject_cast<EditorInterface *>(plugin);
			if (iEditor) {
				foreach (QString d, iEditor->diagrams())
					foreach (QString e, iEditor->elements(d))
					;

				pluginsLoaded += iEditor->id();
				pluginIface[iEditor->id()] = iEditor;
			}
		} else {
			QMessageBox::warning(0, "QReal Plugin", loader.errorString() );
		}
	}
}

QList<QUrl> EditorManager::editors() const
{
	QList<QUrl> editors;
	foreach (QString e, pluginsLoaded) {
		QUrl u = root;
		u.setPath(u.path() + e);
		editors.append(u);
	}
	return editors;
}

QList<QUrl> EditorManager::diagrams(const QUrl &editor) const
{
	QList<QUrl> diagrams;
	QStringList path = editor.path().split('/');
	Q_ASSERT ( path.size() == 2 );
	Q_ASSERT ( pluginsLoaded.contains(path[1]) );

	foreach (QString e, pluginIface[path[1]]->diagrams()) {
		QUrl u = editor;
		u.setPath(u.path() + "/" + e);
		diagrams.append(u);
	}
	return diagrams;
}

QList<QUrl> EditorManager::elements(const QUrl &diagram) const
{
	QList<QUrl> elements;
	QStringList path = diagram.path().split('/');
	Q_ASSERT ( path.size() == 3 );
	Q_ASSERT ( pluginsLoaded.contains(path[1]) );

	foreach (QString e, pluginIface[path[1]]->elements(path[2])) {
		QUrl u = diagram;
		u.setPath(u.path() + "/" + e);
		elements.append(u);
	}
	return elements;
}

bool EditorManager::isEditor(const QUrl &url) const
{
	Q_ASSERT( url.scheme() == "qrm" );
	QStringList path = url.path().split('/');
	Q_ASSERT( pluginsLoaded.contains(path[1]) );

	return path.size() == 2;
}

bool EditorManager::isDiagram(const QUrl &url) const
{
	Q_ASSERT( url.scheme() == "qrm" );
	QStringList path = url.path().split('/');
	Q_ASSERT( pluginsLoaded.contains(path[1]) );

	return path.size() == 2;
}

bool EditorManager::isElement(const QUrl &url) const
{
	Q_ASSERT( url.scheme() == "qrm" );
	QStringList path = url.path().split('/');
	Q_ASSERT( pluginsLoaded.contains(path[1]) );

	return path.size() == 2;
}


QString EditorManager::friendlyName(const QUrl &url) const
{
	Q_ASSERT( url.scheme() == "qrm" );
	QStringList path = url.path().split('/');
	Q_ASSERT( pluginsLoaded.contains(path[1]) );

	switch ( path.size() ) {
		case 2:		return pluginIface[path[1]]->editorName();
					break;
		case 3:		return pluginIface[path[1]]->diagramName(path[2]);
					break;
		case 4:		return pluginIface[path[1]]->elementName(path[2], path[3]);
					break;
		default:	Q_ASSERT( true );
					return "";
	}
}

QIcon EditorManager::icon(const QUrl &url) const
{
	Q_ASSERT( url.scheme() == "qrm" );
	QStringList path = url.path().split('/');
	Q_ASSERT( pluginsLoaded.contains(path[1]) );
	Q_ASSERT(path.size() == 4);
	return pluginIface[path[1]]->getIcon(path[2], path[3]);
}

UML::Element* EditorManager::graphicalObject(const QUrl &typeId) const
{
	Q_ASSERT(typeId.scheme() == "qrm");
	QStringList path = typeId.path().split('/');
	Q_ASSERT(pluginsLoaded.contains(path[1]));
	Q_ASSERT(path.size() == 4);
	return pluginIface[path[1]]->getGraphicalObject(path[2], path[3]);
}
