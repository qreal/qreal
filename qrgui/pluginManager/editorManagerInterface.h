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

class Element;

namespace qReal {
class EditorManagerInterface : public QObject
{
	Q_OBJECT

public:
	explicit EditorManagerInterface(QObject *parent = NULL);

	virtual IdList editors() const = 0;
	virtual IdList diagrams(Id const &editor) const = 0;
	virtual IdList elements(Id const &diagram) const = 0;
	virtual bool loadPlugin(QString const &pluginName) = 0;
	virtual bool unloadPlugin(QString const &pluginName) = 0;

	virtual QString mouseGesture(Id const &id) const = 0;
	virtual QString friendlyName(Id const &id) const = 0;
	virtual QString description(Id const &id) const = 0;
	virtual QString propertyDescription(Id const &id, QString const &propertyName) const = 0;
	virtual QString propertyDisplayedName(Id const &id, QString const &propertyName) const = 0;
	virtual QIcon icon(Id const &id) const = 0;
	virtual Element* graphicalObject(Id const &id) const = 0;

	virtual IdList getContainedTypes(const Id &id) const = 0;
	virtual IdList getConnectedTypes(const Id &id) const = 0;
	virtual IdList getUsedTypes(const Id &id) const = 0;
	virtual QStringList getEnumValues(Id const &id, const QString &name) const = 0;
	virtual QString getTypeName(Id const &id, const QString &name) const = 0;
	virtual QStringList getAllChildrenTypesOf(Id const &parent) const = 0;

	virtual bool isEditor(Id const &id) const = 0;
	virtual bool isDiagram(Id const &id) const = 0;
	virtual bool isElement(Id const &id) const = 0;

	virtual QStringList getPropertyNames(Id const &id) const = 0;
	virtual QString getDefaultPropertyValue(Id const &id, QString name) const = 0;
	virtual QStringList getPropertiesWithDefaultValues(Id const &id) const = 0;

	virtual IdList checkNeededPlugins(qrRepo::LogicalRepoApi const &logicalApi
							  , qrRepo::GraphicalRepoApi const &graphicalApi) const = 0;
	virtual bool hasElement(Id const &element) const = 0;

	virtual Id findElementByType(QString const &type) const = 0;
	virtual QList<ListenerInterface *> listeners() const = 0;

	virtual EditorInterface* editorInterface(QString const &editor) const = 0;

	virtual bool isDiagramNode(Id const &id) const = 0;

	virtual bool isParentOf(Id const &child, Id const &parent) const = 0;
};

}
