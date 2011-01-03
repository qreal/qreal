#pragma once

class QIcon;

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtCore/QStringList>

#include "listenerManager.h"
#include "../umllib/uml_element.h"
#include "../kernel/ids.h"
#include "../../qrmc/plugins/pluginInterface.h"

namespace qrRepo {
	class RepoApi;
}

namespace qReal {
	class EditorManager : public QObject
	{
		Q_OBJECT

	public:
		explicit EditorManager(QObject *parent = NULL);

		IdList editors() const;
		IdList diagrams(Id const &editor) const;
		IdList elements(Id const &diagram) const;
		IdList elementsOnDiagram(Id const &diagramNode) const;
		bool loadPlugin(QString const &pluginName);
		bool unloadPlugin(QString const &pluginName);

		QString mouseGesture(Id const &id) const;
		QString friendlyName(Id const &id) const;
		QString description(Id const &id) const;
		QString propertyDescription(Id const &id, QString const &propertyName) const;
		QIcon icon(Id const &id) const;
		UML::Element* graphicalObject(Id const &id) const;

		IdList getContainedTypes(const Id &id) const;
		IdList getConnectedTypes(const Id &id) const;
		IdList getUsedTypes(const Id &id) const;
		QStringList getEnumValues(Id const &id, const QString &name) const;
		QString getTypeName(Id const &id, const QString &name) const;

		bool isEditor(Id const &id) const;
		bool isDiagram(Id const &id) const;
		bool isElement(Id const &id) const;

		virtual QStringList getPropertyNames(Id const &id) const;
		virtual QString getDefaultPropertyValue(Id const &id, QString name) const;
		virtual QStringList getPropertiesWithDefaultValues(Id const &id) const;

		IdList checkNeededPlugins(qrRepo::RepoApi const &api) const;
		bool hasElement(Id const &element) const;

		Id findElementByType(QString const &type) const;
		QList<Listener *> listeners() const;

				EditorInterface* getEditorInterface(QString editor) const;
	private:
		void checkNeededPluginsRecursive(qrRepo::RepoApi const &api, Id const &id, IdList &result) const;

		QStringList mPluginsLoaded;
		QMap<QString, QString> mPluginFileName;
		QMap<QString, EditorInterface *> mPluginIface;
		QMap<QString, QPluginLoader *> mLoaders;

		QDir mPluginsDir;
		QStringList mPluginFileNames;

		const Id mRoot;
	};

}
