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
class EditorManagerInterface
{
public:
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

	//virtual EditorInterface* editorInterface(QString const &editor) const = 0;

	virtual bool isDiagramNode(Id const &id) const = 0;

	virtual bool isParentOf(Id const &child, Id const &parent) const = 0;
	virtual bool isGraphicalElementNode(const Id &id) const = 0;

	//new methods:
	virtual QList<QPair<QPair<QString, QString>, QPair<bool, QString> > > getPossibleEdges(QString const &editor, QString const &element) const = 0;
	virtual QStringList elements(QString const &editor, QString const &diagram) const = 0;
	virtual int isNodeOrEdge(QString const &editor, QString const &element) const = 0;
	virtual bool isParentOf(QString const &editor, QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const = 0;
	virtual QString diagramName(QString const &editor, QString const &diagram) const = 0;
	virtual QString diagramNodeName(QString const &editor, QString const &diagram) const = 0;
	virtual bool isInterpretationMode() const = 0;
	virtual bool isParentProperty(Id const &id, QString const &propertyName) const = 0;
	virtual void deleteProperty(QString const &propDisplayedName) const = 0;
	virtual void addProperty(Id const &id, QString const &propDisplayedName) const = 0;
	virtual void updateProperties(Id const &id, QString const &property, QString const &propertyType, QString const &propertyDefaultValue, QString const &propertyDisplayedName) const = 0;
	virtual QString getPropertyNameByDisplayedName(Id const &id, QString const &displayedPropertyName) const = 0;
	//unsupported methods:
	virtual QStringList paletteGroups(Id const &editor, Id const &diagram) const = 0;
	virtual QStringList paletteGroupList(Id const &editor,Id const &diagram, QString const &group) const = 0;
	virtual QString paletteGroupDescription(Id const &editor, const Id &diagram, const QString &group) const = 0;
	virtual QStringList getReferenceProperties(Id const &id) const = 0;
};

}
