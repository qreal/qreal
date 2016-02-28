/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtGui/QIcon>

#include <qrkernel/ids.h>
#include <qrkernel/settingsManager.h>
#include <qrutils/pluginManagers/pluginManager.h>

#include "qrgui/plugins/pluginManager/pluginsManagerDeclSpec.h"
#include "qrgui/plugins/pluginManager/editorManagerInterface.h"
#include "qrgui/plugins/pluginManager/pattern.h"
#include "qrgui/plugins/pluginManager/details/patternParser.h"

#include "pluginsManagerDeclSpec.h"

namespace qReal {

class Metamodel;

namespace gui {
namespace editor {
class Element;
}
}

class QRGUI_PLUGINS_MANAGER_EXPORT EditorManager : public QObject, public EditorManagerInterface
{
	Q_OBJECT

public:
	explicit EditorManager(const QString &path);
	explicit EditorManager(QObject *parent = nullptr);
	~EditorManager() override;

	IdList editors() const override;
	IdList diagrams(const Id &editor) const override;
	IdList elements(const Id &diagram) const override;
	Version version(const Id &editor) const override;

	IdList groups(const Id &diagram) override;
	Pattern getPatternByName (const QString &str) const override;
	QList<QString> getPatternNames() const override;
	QStringList paletteGroups(const Id &editor, const Id &diagram) const override;
	QStringList paletteGroupList(const Id &editor,const Id &diagram, const QString &group) const override;
	QString paletteGroupDescription(const Id &editor, const Id &diagram, const QString &group) const override;
	bool shallPaletteBeSorted(const Id &editor, const Id &diagram) const override;

	QString loadPlugin(const QString &pluginName) override;
	QString unloadPlugin(const QString &pluginName) override;

	QString mouseGesture(const Id &id) const override;
	QString friendlyName(const Id &id) const override;
	QString description(const Id &id) const override;
	QString propertyDescription(const Id &id, const QString &propertyName) const override;
	QString propertyDisplayedName(const Id &id, const QString &propertyName) const override;
	QIcon icon(const Id &id) const override;
	QSize iconSize(const Id &id) const override;
	ElementType &elementType(const Id &id) const override;

	IdList containedTypes(const Id &id) const override;
	QList<Explosion> explosions(const Id &source) const override;
	bool isEnumEditable(const Id &id, const QString &name) const override;
	QList<QPair<QString, QString>> enumValues(const Id &id, const QString &name) const override;
	QString typeName(const Id &id, const QString &name) const override;
	QStringList allChildrenTypesOf(const Id &parent) const override;

	bool isEditor(const Id &id) const override;
	bool isDiagram(const Id &id) const override;
	bool isElement(const Id &id) const override;

	QStringList propertyNames(const Id &id) const override;
	QStringList portTypes(const Id &id) const override;
	QStringList referenceProperties(const Id &id) const override;
	QString defaultPropertyValue(const Id &id, QString name) const override;
	QStringList propertiesWithDefaultValues(const Id &id) const override;

	bool hasElement(const Id &element) const override;

	Id findElementByType(const QString &type) const override;

	bool isDiagramNode(const Id &id) const override;

	bool isParentOf(const Id &child, const Id &parent) const override;
	bool isGraphicalElementNode(const Id &id) const override;

	/// Returns diagram id if only one diagram loaded or Id() otherwise
	Id theOnlyDiagram() const override;
	QString diagramNodeNameString(const Id &editor, const Id &diagram) const override;

//	QList<StringPossibleEdge> possibleEdges(const QString &editor, const QString &element) const override;
	QStringList elements(const QString &editor, const QString &diagram) const override;
	int isNodeOrEdge(const QString &editor, const QString &element) const override;
	bool isParentOf(const QString &editor, const QString &parentDiagram, const QString &parentElement
			, const QString &childDiagram, const QString &childElement) const override;
	QString diagramName(const QString &editor, const QString &diagram) const override;
	QString diagramNodeName(const QString &editor, const QString &diagram) const override;
	bool isInterpretationMode() const override;
	bool isParentProperty(const Id &id, const QString &propertyName) const override;
	void deleteProperty(const QString &propDisplayedName) const override;
	void addProperty(const Id &id, const QString &propDisplayedName) const override;
	void updateProperties(const Id &id, const QString &property, const QString &propertyType
			, const QString &propertyDefaultValue, const QString &propertyDisplayedName) const override;
	QString propertyNameByDisplayedName(const Id &id, const QString &displayedPropertyName) const override;
	IdList children(const Id &parent) const override;
	QString shape(const Id &id) const override;
	void updateShape(const Id &id, const QString &graphics) const override;
	virtual void resetIsHidden(const Id &id) const;
	virtual QString getIsHidden(const Id &id) const;
	void deleteElement(const Id &id) const override;
	bool isRootDiagramNode(const Id &id) const override;
	void addNodeElement(const Id &diagram, const QString &name, const QString &displayedName
			, bool isRootDiagramNode) const override;
	void addEdgeElement(const Id &diagram, const QString &name, const QString &displayedName, const QString &labelText
			, const QString &labelType, const QString &lineType, const QString &beginType
			, const QString &endType) const override;
	QPair<Id, Id> createEditorAndDiagram(const QString &name) const override;
	void saveMetamodel(const QString &newMetamodelFileName) override;
	QString saveMetamodelFilePath() const override;

	IdList elementsWithTheSameName(const Id &diagram, const QString &name, const QString type) const override;
	IdList propertiesWithTheSameName(const Id &id
			, const QString &propertyCurrentName, const QString &propertyNewName) const override;

	void updateGenerationRule(const Id &id, const QString &newRule) const override;
	QString generationRule(const Id &id) const override;

	QStringList getPropertiesInformation(const Id &id) const override;
	QStringList getSameNamePropertyParams(const Id &propertyId, const QString &propertyName) const override;
	void restoreRemovedProperty(const Id &propertyId, const QString &previousName) const override;
	void restoreRenamedProperty(const Id &propertyId, const QString &previousName) const override;

	void setElementEnabled(const Id &type, bool enabled) override;

private:
	Metamodel *metamodel(const QString &editor) const;

	void init();

	bool isParentOf(const Metamodel *plugin, const QString &childDiagram, const QString &child
			, const QString &parentDiagram, const QString &parent) const;

	QStringList mPluginsLoaded;
	QMap<QString, QString> mPluginFileName;
	QMap<QString, Pattern> mGroups;
	QMap<QString, Metamodel *> mPluginIface;

	QDir mPluginsDir;
	QStringList mPluginFileNames;

	/// Common part of plugin loaders
	PluginManager mPluginManager;

	QSet<Id> mDisabledElements;
};

}
