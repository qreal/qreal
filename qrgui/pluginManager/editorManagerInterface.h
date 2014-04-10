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
#include "editorPluginInterface/editorInterface.h"
#include "pluginManager/pattern.h"
#include "pluginManager/patternParser.h"
#include "pluginManager/explosion.h"

namespace qReal {

class Element;

class MainWindow;

class EditorManagerInterface
{
public:
	virtual ~EditorManagerInterface() {}

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
	virtual ElementImpl* elementImpl(Id const &id) const = 0;

	virtual IdList containedTypes(const Id &id) const = 0;
	virtual QList<Explosion> explosions(Id const &source) const = 0;
	virtual QStringList enumValues(Id const &id, const QString &name) const = 0;
	virtual QString typeName(Id const &id, const QString &name) const = 0;
	virtual QStringList allChildrenTypesOf(Id const &parent) const = 0;

	virtual bool isEditor(Id const &id) const = 0;
	virtual bool isDiagram(Id const &id) const = 0;
	virtual bool isElement(Id const &id) const = 0;

	virtual QStringList propertyNames(Id const &id) const = 0;
	virtual QStringList portTypes(Id const &id) const = 0;
	virtual QString defaultPropertyValue(Id const &id, QString name) const = 0;
	virtual QStringList propertiesWithDefaultValues(Id const &id) const = 0;

	virtual IdList checkNeededPlugins(qrRepo::LogicalRepoApi const &logicalApi
							  , qrRepo::GraphicalRepoApi const &graphicalApi) const = 0;
	virtual bool hasElement(Id const &element) const = 0;

	virtual Id findElementByType(QString const &type) const = 0;
	virtual QList<ListenerInterface *> listeners() const = 0;

	virtual bool isDiagramNode(Id const &id) const = 0;

	virtual bool isParentOf(Id const &child, Id const &parent) const = 0;
	virtual bool isGraphicalElementNode(const Id &id) const = 0;

	/// Returns diagram id if only one diagram loaded or Id() otherwise
	virtual Id theOnlyDiagram() const = 0;
	virtual QString diagramNodeNameString(Id const &editor, Id const &diagram) const = 0;

	virtual QList<StringPossibleEdge> possibleEdges(QString const &editor, QString const &element) const = 0;
	virtual QStringList elements(QString const &editor, QString const &diagram) const = 0;
	virtual int isNodeOrEdge(QString const &editor, QString const &element) const = 0;
	virtual bool isParentOf(QString const &editor, QString const &parentDiagram, QString const &parentElement
			, QString const &childDiagram, QString const &childElement) const = 0;
	virtual QString diagramName(QString const &editor, QString const &diagram) const = 0;
	virtual QString diagramNodeName(QString const &editor, QString const &diagram) const = 0;
	virtual bool isInterpretationMode() const = 0;
	virtual bool isParentProperty(Id const &id, QString const &propertyName) const = 0;
	virtual void deleteProperty(QString const &propDisplayedName) const = 0;
	virtual void addProperty(Id const &id, QString const &propDisplayedName) const = 0;
	virtual void updateProperties(Id const &id, QString const &property, QString const &propertyType
			, QString const &propertyDefaultValue, QString const &propertyDisplayedName) const = 0;
	virtual QString propertyNameByDisplayedName(Id const &id, QString const &displayedPropertyName) const = 0;
	virtual IdList children(Id const &parent) const = 0;
	virtual QString shape(Id const &id) const = 0;
	virtual void updateShape(Id const &id, QString const &graphics) const = 0;
	virtual void resetIsHidden(Id const &id) const = 0;
	virtual QString getIsHidden(Id const &id) const = 0;
	virtual void deleteElement(MainWindow *mainWindow, Id const &id) const = 0;
	virtual bool isRootDiagramNode(Id const &id) const = 0;
	virtual void addNodeElement(Id const &diagram, QString const &name, QString const &displayedName
			, bool isRootDiagramNode) const = 0;
	virtual void addEdgeElement(Id const &diagram, QString const &name, QString const &displayedName
			, QString const &labelText, QString const &labelType, QString const &lineType
			, QString const &beginType, QString const &endType) const = 0;
	virtual QPair<Id, Id> createEditorAndDiagram(QString const &name) const = 0;
	virtual void saveMetamodel(QString const &newMetamodelFileName) = 0;
	virtual QString saveMetamodelFilePath() const = 0;
	virtual QStringList paletteGroups(Id const &editor, Id const &diagram) const = 0;
	virtual QStringList paletteGroupList(Id const &editor,Id const &diagram, QString const &group) const = 0;
	virtual QString paletteGroupDescription(Id const &editor, const Id &diagram, const QString &group) const = 0;
	virtual bool shallPaletteBeSorted(Id const &editor, Id const &diagram) const = 0;
	virtual QStringList referenceProperties(Id const &id) const = 0;
	virtual IdList groups(Id const &diagram) = 0;
	virtual Pattern getPatternByName (QString const &str) const = 0;
	virtual QList<QString> getPatternNames() const = 0;
	virtual QSize iconSize(Id const &id) const = 0;

	virtual IdList elementsWithTheSameName(Id const &diagram, QString const &name, QString const type) const = 0;
	virtual IdList propertiesWithTheSameName(Id const &id, QString const &propCurrentName
			, QString const &propNewName) const = 0;

	virtual QStringList getPropertiesInformation(Id const &id) const = 0;
	virtual QStringList getSameNamePropertyParams(Id const &propertyId, QString const &propertyName) const = 0;
	virtual void restoreRemovedProperty(Id const &propertyId, QString const &previousName) const = 0;
	virtual void restoreRenamedProperty(Id const &propertyId, QString const &previousName) const = 0;
};

}
