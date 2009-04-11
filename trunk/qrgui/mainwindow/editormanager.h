#pragma once

class QIcon;
class EditorInterface;

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>

#include "../umllib/uml_element.h"
#include "../kernel/ids.h"

namespace qReal {

	class EditorManager : public QObject
	{
		Q_OBJECT

	public:
		EditorManager(QObject *parent = 0);

		QList<Id> editors() const;
		QList<Id> diagrams(const Id &editor) const;
		QList<Id> elements(const Id &diagram) const;

		QString friendlyName(const Id &id) const;
		QIcon icon(const Id &id) const;
		UML::Element* graphicalObject(const Id &id) const;

		bool isEditor(const Id &id) const;
		bool isDiagram(const Id &id) const;
		bool isElement(const Id &id) const;

	private:
		QStringList mPluginsLoaded;
		QMap<QString, QString> mPluginFileName;
		QMap<QString, EditorInterface *> mPluginIface;

		QDir mPluginsDir;
		QStringList mPluginFileNames;

		const Id mRoot;
	};

}
