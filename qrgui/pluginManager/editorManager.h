#pragma once

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtCore/QStringList>
#include <QtGui/QIcon>

#include <qrkernel/ids.h>
#include <qrkernel/settingsManager.h>
#include <qrrepo/graphicalRepoApi.h>
#include <qrrepo/logicalRepoApi.h>

#include "pluginManager/listenerManager.h"
#include "pluginManager/editorManagerInterface.h"
#include "pluginManager/pattern.h"
#include "pluginManager/patternParser.h"
#include "editorPluginInterface/editorInterface.h"

namespace qReal {

class Element;

class EditorManager : public QObject, public EditorManagerInterface
{
	Q_OBJECT

public:
	explicit EditorManager(QObject *parent = NULL);

	~EditorManager();

	virtual IdList editors() const;
	virtual IdList diagrams(Id const &editor) const;
	virtual IdList groups(Id const &diagram);
	virtual Pattern getPatternByName (QString const &str) const;
	virtual QList<QString> getPatternNames() const;
	virtual QStringList paletteGroups(Id const &editor, Id const &diagram) const;
	virtual QStringList paletteGroupList(Id const &editor,Id const &diagram, QString const &group) const;
	virtual QString paletteGroupDescription(Id const &editor, const Id &diagram, const QString &group) const;
	virtual IdList elements(Id const &diagram) const;
	virtual bool loadPlugin(QString const &pluginName);
	virtual bool unloadPlugin(QString const &pluginName);

	virtual QString mouseGesture(Id const &id) const;
	virtual QString friendlyName(Id const &id) const;
	virtual QString description(Id const &id) const;
	virtual QString propertyDescription(Id const &id, QString const &propertyName) const;
	virtual QString propertyDisplayedName(Id const &id, QString const &propertyName) const;
	virtual QIcon icon(Id const &id) const;
	virtual QSize iconSize(Id const &id) const;
	virtual ElementImpl* elementImpl(Id const &id) const;

	virtual IdList containedTypes(const Id &id) const;
	virtual QList<Explosion> explosions(Id const &source) const;
	virtual QStringList enumValues(Id const &id, const QString &name) const;
	virtual QString typeName(Id const &id, const QString &name) const;
	virtual QStringList allChildrenTypesOf(Id const &parent) const;

	virtual bool isEditor(Id const &id) const;
	virtual bool isDiagram(Id const &id) const;
	virtual bool isElement(Id const &id) const;

	virtual QStringList propertyNames(Id const &id) const;
	virtual QStringList portTypes(Id const &id) const;
	virtual QStringList referenceProperties(Id const &id) const;
	virtual QString defaultPropertyValue(Id const &id, QString name) const;
	virtual QStringList propertiesWithDefaultValues(Id const &id) const;

	virtual IdList checkNeededPlugins(qrRepo::LogicalRepoApi const &logicalApi
			, qrRepo::GraphicalRepoApi const &graphicalApi) const;
	virtual bool hasElement(Id const &element) const;

	virtual Id findElementByType(QString const &type) const;
	virtual QList<ListenerInterface *> listeners() const;

	virtual bool isDiagramNode(Id const &id) const;

	virtual bool isParentOf(Id const &child, Id const &parent) const;
	virtual bool isGraphicalElementNode(const Id &id) const;

	/// Returns diagram id if only one diagram loaded or Id() otherwise
	virtual Id theOnlyDiagram() const;
	virtual QString diagramNodeNameString(Id const &editor, Id const &diagram) const;

	virtual QList<StringPossibleEdge> possibleEdges(QString const &editor, QString const &element) const;
	virtual QStringList elements(QString const &editor, QString const &diagram) const;
	virtual int isNodeOrEdge(QString const &editor, QString const &element) const;
	virtual bool isParentOf(QString const &editor, QString const &parentDiagram, QString const &parentElement
			, QString const &childDiagram, QString const &childElement) const;
	virtual QString diagramName(QString const &editor, QString const &diagram) const;
	virtual QString diagramNodeName(QString const &editor, QString const &diagram) const;
	virtual bool isInterpretationMode() const;
	virtual bool isParentProperty(Id const &id, QString const &propertyName) const;
	virtual void deleteProperty(QString const &propDisplayedName) const;
	virtual void addProperty(Id const &id, QString const &propDisplayedName) const;
	virtual void updateProperties(Id const &id, QString const &property, QString const &propertyType
			, QString const &propertyDefaultValue, QString const &propertyDisplayedName) const;
	virtual QString propertyNameByDisplayedName(Id const &id, QString const &displayedPropertyName) const;
	virtual IdList children(Id const &parent) const;
	virtual QString shape(Id const &id) const;
	virtual void updateShape(Id const &id, QString const &graphics) const;
	virtual void resetIsHidden(Id const &id) const;
	virtual QString getIsHidden(Id const &id) const;
	virtual void deleteElement(MainWindow *mainWindow, Id const &id) const;
	virtual bool isRootDiagramNode(Id const &id) const;
	virtual void addNodeElement(Id const &diagram, QString const &name, bool isRootDiagramNode) const;
	virtual void addEdgeElement(Id const &diagram, QString const &name, QString const &labelText
			, QString const &labelType, QString const &lineType, QString const &beginType
			, QString const &endType) const;
	virtual QPair<Id, Id> createEditorAndDiagram(QString const &name) const;
	virtual void saveMetamodel(QString const &newMetamodelFileName);
	virtual QString saveMetamodelFilePath() const;

private:
	QStringList mPluginsLoaded;
	QMap<QString, QString> mPluginFileName;
	QList<Pattern> mDiagramGroups;
	QMap<QString, Pattern> mGroups;
	QMap<QString, EditorInterface *> mPluginIface;
	QMap<QString, QPluginLoader *> mLoaders;

	QDir mPluginsDir;
	QStringList mPluginFileNames;

	EditorInterface* editorInterface(QString const &editor) const;
	void checkNeededPluginsRecursive(qrRepo::CommonRepoApi const &api, Id const &id, IdList &result) const;

	bool isParentOf(EditorInterface const *plugin, QString const &childDiagram, QString const &child
			, QString const &parentDiagram, QString const &parent) const;
};

}
