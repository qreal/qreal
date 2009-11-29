#pragma once

class QIcon;

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>

#include "../umllib/uml_element.h"
#include "../kernel/ids.h"

namespace qrRepo {
	class RepoApi;
}

namespace qReal {

	class EditorInterface;

	class EditorManager : public QObject
	{
		Q_OBJECT

	public:
		explicit EditorManager(QObject *parent = NULL);

		QList<Id> editors() const;
		QList<Id> diagrams(Id const &editor) const;
		QList<Id> elements(Id const &diagram) const;

		QString friendlyName(Id const &id) const;
		QIcon icon(Id const &id) const;
		UML::Element* graphicalObject(Id const &id) const;

		bool isEditor(Id const &id) const;
		bool isDiagram(Id const &id) const;
		bool isElement(Id const &id) const;

		QStringList getPropertyNames(Id const &id) const;

		IdList checkNeededPlugins(qrRepo::RepoApi const &api) const;

	private:
		void checkNeededPluginsRecursive(qrRepo::RepoApi const &api, Id const &id, IdList &result) const;

		QStringList mPluginsLoaded;
		QMap<QString, QString> mPluginFileName;
		QMap<QString, EditorInterface *> mPluginIface;

		QDir mPluginsDir;
		QStringList mPluginFileNames;

		const Id mRoot;
	};

}
