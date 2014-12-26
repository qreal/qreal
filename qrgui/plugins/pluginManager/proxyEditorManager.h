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

#include <plugins/editorPluginInterface/editorInterface.h>
#include "plugins/pluginManager/pluginsManagerDeclSpec.h"
#include "plugins/pluginManager/editorManagerInterface.h"

#include "pluginsManagerDeclSpec.h"

namespace qReal {

class Element;

/// Proxy for changing between generative and interpretive approach.
class QRGUI_PLUGINS_MANAGER_EXPORT ProxyEditorManager : public EditorManagerInterface
{
public:
	explicit ProxyEditorManager(EditorManagerInterface *editorManagerInterface);
	~ProxyEditorManager() override;

	MetaType metaTypeOfElement(qReal::Id const& element) const;

	IdList editors() const override;
	IdList diagrams(Id const &editor) const override;
	IdList elements(Id const &diagram) const override;
	Version version(Id const &editor) const override;

	QString loadPlugin(QString const &pluginName) override;
	QString unloadPlugin(QString const &pluginName) override;

	QString mouseGesture(Id const &id) const override;
	QString friendlyName(Id const &id) const override;
	QString description(Id const &id) const override;
	QString propertyDescription(Id const &id, QString const &propertyName) const override;
	QString propertyDisplayedName(Id const &id, QString const &propertyName) const override;
	QIcon icon(Id const &id) const override;
	ElementImpl* elementImpl(Id const &id) const override;

	IdList containedTypes(const Id &id) const override;
	bool isEnumEditable(Id const &id, QString const &name) const override;
	QList<QPair<QString, QString>> enumValues(Id const &id, const QString &name) const override;
	QString typeName(Id const &id, const QString &name) const override;
	QStringList allChildrenTypesOf(Id const &parent) const override;

	QList<Explosion> explosions(Id const &source) const override;

	bool isEditor(Id const &id) const override;
	bool isDiagram(Id const &id) const override;
	bool isElement(Id const &id) const override;

	QStringList propertyNames(Id const &id) const override;
	QStringList portTypes(Id const &id) const override;
	QString defaultPropertyValue(Id const &id, QString name) const override;
	QStringList propertiesWithDefaultValues(Id const &id) const override;

	IdList checkNeededPlugins(LogicalModelAssistInterface const &logicalApi
			, GraphicalModelAssistInterface const &graphicalApi) const override;
	bool hasElement(Id const &element) const override;

	Id findElementByType(QString const &type) const override;
	QList<ListenerInterface *> listeners() const override;

	bool isDiagramNode(Id const &id) const override;

	bool isParentOf(Id const &child, Id const &parent) const override;
	bool isGraphicalElementNode(const Id &id) const override;

	/// Returns diagram id if only one diagram loaded or Id() otherwise
	Id theOnlyDiagram() const override;
	QString diagramNodeNameString(Id const &editor, Id const &diagram) const override;

	QList<StringPossibleEdge> possibleEdges(QString const &editor, QString const &element) const override;
	QStringList elements(QString const &editor, QString const &diagram) const override;
	int isNodeOrEdge(QString const &editor, QString const &element) const override;
	bool isParentOf(QString const &editor, QString const &parentDiagram, QString const &parentElement
			, QString const &childDiagram, QString const &childElement) const override;
	QString diagramName(QString const &editor, QString const &diagram) const override;
	QString diagramNodeName(QString const &editor, QString const &diagram) const override;

	// Takes ownership.
	void setProxyManager(EditorManagerInterface *editorManagerInterface);

	bool isInterpretationMode() const override;
	bool isParentProperty(Id const &id, QString const &propertyName) const override;
	void deleteProperty(QString const &propDisplayedName) const override;
	void addProperty(Id const &id, QString const &propDisplayedName) const override;
	void updateProperties(Id const &id, QString const &property, QString const &propertyType
			, QString const &propertyDefaultValue, QString const &propertyDisplayedName) const override;
	QString propertyNameByDisplayedName(Id const &id, QString const &displayedPropertyName) const override;
	IdList children(Id const &parent) const override;
	QString shape(Id const &id) const override;
	void updateShape(Id const &id, QString const &graphics) const override;
	void resetIsHidden(Id const &id) const;
	QString getIsHidden(Id const &id) const;
	void deleteElement(Id const &id) const override;
	bool isRootDiagramNode(Id const &id) const override;
	void addNodeElement(Id const &diagram, QString const &name, QString const &displayedName
			, bool isRootDiagramNode) const override;

	void addEdgeElement(Id const &diagram, QString const &name, QString const &displayedName
			, QString const &labelText, QString const &labelType, QString const &lineType
			, QString const &beginType, QString const &endType) const override;

	QPair<Id, Id> createEditorAndDiagram(QString const &name) const override;
	void saveMetamodel(QString const &newMetamodelFileName) override;
	QString saveMetamodelFilePath() const override;

	QStringList paletteGroups(Id const &editor, Id const &diagram) const override;
	QStringList paletteGroupList(Id const &editor,Id const &diagram, QString const &group) const override;
	QString paletteGroupDescription(Id const &editor, const Id &diagram, const QString &group) const override;
	bool shallPaletteBeSorted(Id const &editor, Id const &diagram) const override;
	virtual QStringList referenceProperties(Id const &id) const override;
	IdList groups(Id const &diagram) override;
	Pattern getPatternByName (QString const &str) const override;
	QList<QString> getPatternNames() const override;
	QSize iconSize(Id const &id) const override;

	IdList elementsWithTheSameName(Id const &diagram, QString const &name, QString const type) const override;
	IdList propertiesWithTheSameName(Id const &id
			, QString const &propCurrentName, QString const &propNewName) const override;

	QStringList getPropertiesInformation(Id const &id) const override;
	QStringList getSameNamePropertyParams(Id const &propertyId, QString const &propertyName) const override;
	void restoreRemovedProperty(Id const &propertyId, QString const &previousName) const override;
	void restoreRenamedProperty(Id const &propertyId, QString const &previousName) const override;

	void setElementEnabled(Id const &type, bool enabled) override;

	/// Returns proxied editor manager.
	EditorManagerInterface *proxiedEditorManager() const;  // doesn't transfer ownership

private:
	EditorManagerInterface *mProxiedEditorManager;  // Has ownership.
};
}
