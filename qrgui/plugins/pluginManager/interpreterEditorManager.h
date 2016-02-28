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
#include <QtCore/QStringList>
#include <QtCore/QPair>
#include <QtGui/QIcon>

#include <qrkernel/ids.h>
#include <qrkernel/settingsManager.h>
#include <qrrepo/repoApi.h>

#include "plugins/pluginManager/pluginsManagerDeclSpec.h"
#include "plugins/pluginManager/editorManagerInterface.h"
#include "plugins/pluginManager/interpretedPluginManager.h"

#include "pluginsManagerDeclSpec.h"

namespace qReal {

namespace gui {
namespace editor {
class Element;
}
}

class QRGUI_PLUGINS_MANAGER_EXPORT InterpreterEditorManager : public QObject, public EditorManagerInterface
{
	Q_OBJECT

public:
	explicit InterpreterEditorManager(const QString &fileName, QObject *parent = nullptr);
	~InterpreterEditorManager();

	IdList editors() const override;
	IdList diagrams(const Id &editor) const override;
	IdList elements(const Id &diagram) const override;
	Version version(const Id &editor) const override;

	QString loadPlugin(const QString &pluginName) override;
	QString unloadPlugin(const QString &pluginName) override;

	QString mouseGesture(const Id &id) const override;
	QString friendlyName(const Id &id) const override;
	QString description(const Id &id) const override;
	QString propertyDescription(const Id &id, const QString &propertyName) const override;
	QString propertyDisplayedName(const Id &id, const QString &propertyName) const override;
	QIcon icon(const Id &id) const override;
	ElementType &elementType(const Id &id) const override;

	IdList containedTypes(const Id &id) const override;
	bool isEnumEditable(const Id &id, const QString &name) const override;
	QList<QPair<QString, QString>> enumValues(const Id &id, const QString &name) const override;
	QString typeName(const Id &id, const QString &name) const override;
	QStringList allChildrenTypesOf(const Id &parent) const override;

	QList<Explosion> explosions(const Id &source) const override;

	bool isEditor(const Id &id) const override;
	bool isDiagram(const Id &id) const override;
	bool isElement(const Id &id) const override;

	QStringList propertyNames(const Id &id) const override;
	QStringList portTypes(const Id &id) const override;
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

//	QList<StringPossibleEdge> possibleEdges(const QString &editor, const QString &elementName) const override;
	QStringList elements(const QString &editor, const QString &diagram) const override;
	int isNodeOrEdge(const QString &editor, const QString &element) const override;
	bool isParentOf(const QString &editor, const QString &parentDiagram, const QString &parentElement
			, const QString &childDiagram, const QString &childElement) const override;
	QString diagramName(const QString &editor, const QString &diagram) const override;
	QString diagramNodeName(const QString &editor, const QString &diagram) const override;
	bool isInterpretationMode() const override;

	bool isParentProperty(const Id &id, const QString &propertyName) const override;
	void deleteProperty(const QString &propertyName) const override;
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
	void addEdgeElement(
			const Id &diagram
			, const QString &name
			, const QString &displayedName
			, const QString &labelText
			, const QString &labelType
			, const QString &lineType
			, const QString &beginType
			, const QString &endType
			) const override;

	void updateGenerationRule(const Id &id, const QString &newRule) const override;
	QString generationRule(const Id &id) const override;

	QPair<Id, Id> createEditorAndDiagram(const QString &name) const override;
	void saveMetamodel(const QString &newMetamodelFileName) override;
	QString saveMetamodelFilePath() const override;
	QStringList paletteGroups(const Id &editor, const Id &diagram) const override;
	QStringList paletteGroupList(const Id &editor,const Id &diagram, const QString &group) const override;
	QString paletteGroupDescription(const Id &editor, const Id &diagram, const QString &group) const override;
	bool shallPaletteBeSorted(const Id &editor, const Id &diagram) const override;
	QStringList referenceProperties(const Id &id) const override;
	IdList groups(const Id &diagram) override;
	Pattern getPatternByName (const QString &str) const override;
	QList<QString> getPatternNames() const override;
	QSize iconSize(const Id &id) const override;

	IdList elementsWithTheSameName(const Id &diagram, const QString &name, const QString type) const override;
	IdList propertiesWithTheSameName(
			const Id &id
			, const QString &propCurrentName
			, const QString &propNewName) const override;

	QStringList getPropertiesInformation(const Id &id) const override;
	QStringList getSameNamePropertyParams(const Id &propertyId, const QString &propertyName) const override;
	void restoreRemovedProperty(const Id &propertyId, const QString &previousName) const override;
	void restoreRenamedProperty(const Id &propertyId, const QString &previousName) const override;

	void setElementEnabled(const Id &type, bool enabled) override;

	/// Returns list of metamodels for interpreted plugins.
	QMap<QString, qrRepo::RepoApi*> listOfMetamodels() const;

private:
	class CheckPropertyForParent;
	class CompareProperty;
	class GetProperty;
	class HasProperty;

	QMap<QString, qrRepo::RepoApi*> mEditorRepoApi;  // Has ownership.
	QString mMetamodelFile;

	void setProperty(qrRepo::RepoApi* repo, const Id &id, const QString &property, const QVariant &propertyValue) const;
	Id element(const Id &id, const qrRepo::RepoApi * const repo, const Id &diagram) const;
	Id diagramOrElement(const Id &id, const qrRepo::RepoApi * const repo, const Id &editor) const;
	void setStandartConfigurations(qrRepo::RepoApi *repo, const Id &id, const Id &parent, const QString &name) const;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId(const Id &id) const;
	QPair<qrRepo::RepoApi*, Id> repoAndElement(const QString &editor, const QString &element) const;
	QPair<qrRepo::RepoApi*, Id> repoAndDiagram(const QString &editor, const QString &diagram) const;
	QPair<Id, Id> editorAndDiagram(const qrRepo::RepoApi * const repo, const Id &element) const;
	QStringList propertiesFromParents(const Id &id, const QString &propertyName
			, const CheckPropertyForParent &checker) const;
	QString valueOfProperty(const Id &id, const QString &propertyName, const QString &value) const;
	void deletePropertyInElement(qrRepo::RepoApi *repo, const Id &diagram, const QString &propDisplayedName) const;
};

}
