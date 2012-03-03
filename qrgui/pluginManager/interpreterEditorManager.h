#pragma once

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtCore/QStringList>
#include <QtGui/QIcon>

#include "listenerManager.h"
#include "../../qrkernel/ids.h"
#include "../editorPluginInterface/editorInterface.h"
#include "../../qrrepo/graphicalRepoApi.h"
#include "../../qrrepo/logicalRepoApi.h"
#include "../../qrkernel/settingsManager.h"
#include "../../qrrepo/repoApi.h"
#include "editorManagerInterface.h"

class Element;

namespace qReal {
	class InterpreterEditorManager : public QObject, public EditorManagerInterface
	{
		Q_OBJECT

	public:
		explicit InterpreterEditorManager(QObject *parent = NULL);

		IdList editors() const;
		IdList diagrams(Id const &editor) const;
		IdList elements(Id const &diagram) const;
		bool loadPlugin(QString const &pluginName);
		bool unloadPlugin(QString const &pluginName);

		QString mouseGesture(Id const &id) const;
		QString friendlyName(Id const &id) const;
		QString description(Id const &id) const;

		QString propertyDescription(Id const &id, QString const &propertyName) const;
		QString propertyDisplayedName(Id const &id, QString const &propertyName) const;
		QIcon icon(Id const &id) const;
		Element* graphicalObject(Id const &id) const;

		IdList getContainedTypes(const Id &id) const;
		IdList getConnectedTypes(const Id &id) const;
		IdList getUsedTypes(const Id &id) const;
		QStringList getEnumValues(Id const &id, const QString &name) const;
		QString getTypeName(Id const &id, const QString &name) const;
		QStringList getAllChildrenTypesOf(Id const &parent) const;

		bool isEditor(Id const &id) const;
		bool isDiagram(Id const &id) const;
		bool isElement(Id const &id) const;

		virtual QStringList getPropertyNames(Id const &id) const;
		virtual QString getDefaultPropertyValue(Id const &id, QString name) const;
		virtual QStringList getPropertiesWithDefaultValues(Id const &id) const;

		IdList checkNeededPlugins(qrRepo::LogicalRepoApi const &logicalApi
				, qrRepo::GraphicalRepoApi const &graphicalApi) const;
		bool hasElement(Id const &element) const;

		Id findElementByType(QString const &type) const;
		QList<ListenerInterface *> listeners() const;

		EditorInterface* editorInterface(QString const &editor) const;

		bool isDiagramNode(Id const &id) const;

		bool isParentOf(Id const &child, Id const &parent) const;

	private:
		QMap<QString, qrRepo::RepoApi*> mEditorRepoApi;

		void checkNeededPluginsRecursive(qrRepo::CommonRepoApi const &api, Id const &id, IdList &result) const;

		bool isParentOf(EditorInterface const *plugin, QString const &childDiagram, QString const &child, QString const &parentDiagram, QString const &parent) const;
	};

}
