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

#include <QtCore/QMap>
#include <QtGui/QIcon>

#include <qrkernel/ids.h>
#include <qrkernel/version.h>
#include <qrkernel/settingsManager.h>

#include "qrgui/plugins/pluginManager/pattern.h"
#include "qrgui/plugins/pluginManager/details/patternParser.h"

namespace qReal {

class ElementType;
class Explosion;
class Metamodel;

namespace gui {
namespace editor {
class Element;
}
}

class EditorManagerInterface
{
public:
	virtual ~EditorManagerInterface() {}

	virtual IdList editors() const = 0;
	virtual IdList diagrams(const Id &editor) const = 0;
	virtual IdList elements(const Id &diagram) const = 0;
	virtual Version version(const Id &editor) const = 0;

	/// @returns Error message if something went wrong or empty string if everything was ok.
	virtual QString loadPlugin(const QString &pluginName) = 0;

	/// @returns Error message if something went wrong or empty string if everything was ok.
	virtual QString unloadPlugin(const QString &pluginName) = 0;

	/// @returns True if plugins unloading was successfull or false otherwise.
	virtual bool unloadAllPlugins() = 0;

	/// Appends \a metamodel to a list of loaded ones.
	/// Takes ownership on \a metamodel.
	virtual void loadMetamodel(Metamodel &metamodel) = 0;

	virtual QString mouseGesture(const Id &id) const = 0;
	virtual QString friendlyName(const Id &id) const = 0;
	virtual QString description(const Id &id) const = 0;
	virtual QString propertyDescription(const Id &id, const QString &propertyName) const = 0;
	virtual QString propertyDisplayedName(const Id &id, const QString &propertyName) const = 0;
	virtual QIcon icon(const Id &id) const = 0;
	virtual ElementType &elementType(const Id &id) const = 0;

	virtual IdList containedTypes(const Id &id) const = 0;
	virtual QList<const Explosion *> explosions(const Id &source) const = 0;
	virtual bool isEnumEditable(const Id &id, const QString &name) const = 0;
	virtual QList<QPair<QString, QString>> enumValues(const Id &id, const QString &name) const = 0;
	virtual QString typeName(const Id &id, const QString &name) const = 0;
	virtual QStringList allChildrenTypesOf(const Id &parent) const = 0;

	virtual bool isEditor(const Id &id) const = 0;
	virtual bool isDiagram(const Id &id) const = 0;
	virtual bool isElement(const Id &id) const = 0;

	/// Updates generation rule for given element.
	/// @param id - element id.
	/// @param newRule - new generation rule.
	virtual void updateGenerationRule(const Id &id, const QString &newRule) const = 0;

	/// Returns rule for given element.
	/// @param id - element id.
	virtual QString generationRule(const Id &id) const = 0;

	virtual QStringList propertyNames(const Id &id) const = 0;
	virtual QStringList portTypes(const Id &id) const = 0;
	virtual QString defaultPropertyValue(const Id &id, QString name) const = 0;

	virtual bool hasElement(const Id &element) const = 0;

	virtual Id findElementByType(const QString &type) const = 0;

	virtual bool isDiagramNode(const Id &id) const = 0;

	virtual bool isParentOf(const Id &child, const Id &parent) const = 0;
	virtual bool isGraphicalElementNode(const Id &id) const = 0;

	/// Returns diagram id if only one diagram loaded or Id() otherwise
	virtual Id theOnlyDiagram() const = 0;
	virtual QString diagramNodeNameString(const Id &editor, const Id &diagram) const = 0;

//	virtual QList<StringPossibleEdge> possibleEdges(const QString &editor, const QString &element) const = 0;
	virtual int isNodeOrEdge(const Id &id) const = 0;
	virtual bool isParentOf(const QString &editor, const QString &parentDiagram, const QString &parentElement
			, const QString &childDiagram, const QString &childElement) const = 0;
	virtual QString diagramName(const QString &editor, const QString &diagram) const = 0;
	virtual QString diagramNodeName(const QString &editor, const QString &diagram) const = 0;

	virtual bool isInterpretationMode() const = 0;
	virtual void setInterpretationMode(bool enabled) = 0;

	virtual bool isParentProperty(const Id &id, const QString &propertyName) const = 0;
	virtual void deleteProperty(const QString &propDisplayedName) const = 0;
	virtual void addProperty(const Id &id, const QString &propDisplayedName) const = 0;
	virtual void updateProperties(const Id &id, const QString &property, const QString &propertyType
			, const QString &propertyDefaultValue, const QString &propertyDisplayedName) const = 0;
	virtual QString propertyNameByDisplayedName(const Id &id, const QString &displayedPropertyName) const = 0;
	virtual IdList children(const Id &parent) const = 0;
	virtual QString shape(const Id &id) const = 0;
	virtual void updateShape(const Id &id, const QDomElement &graphicsSdf) const = 0;
	virtual void resetIsHidden(const Id &id) const = 0;
	virtual bool isHidden(const Id &id) const = 0;
	virtual void deleteElement(const Id &id) const = 0;
	virtual void addNodeElement(const Id &diagram, const QString &name, const QString &displayedName
			, bool isRootDiagramNode) const = 0;
	virtual void addEdgeElement(const Id &diagram, const QString &name, const QString &displayedName
			, const QString &labelText, const QString &labelType, const QString &lineType
			, const QString &beginType, const QString &endType) const = 0;
	virtual void createEditorAndDiagram(const QString &name) = 0;
	virtual void saveMetamodel(const QString &newMetamodelFileName) = 0;
	virtual QString saveMetamodelFilePath() const = 0;
	virtual QStringList paletteGroups(const Id &editor, const Id &diagram) const = 0;
	virtual QStringList paletteGroupList(const Id &editor,const Id &diagram, const QString &group) const = 0;
	virtual QString paletteGroupDescription(const Id &editor, const Id &diagram, const QString &group) const = 0;
	virtual bool shallPaletteBeSorted(const Id &editor, const Id &diagram) const = 0;
	virtual QStringList referenceProperties(const Id &id) const = 0;
	virtual Pattern parsePattern(const Id &id) const = 0;
	virtual QSize iconSize(const Id &id) const = 0;

	virtual IdList elementsWithTheSameName(const Id &diagram, const QString &name, const QString &type) const = 0;
	virtual IdList propertiesWithTheSameName(const Id &id, const QString &propCurrentName
			, const QString &propNewName) const = 0;

	virtual QStringList getPropertiesInformation(const Id &id) const = 0;
	virtual QStringList getSameNamePropertyParams(const Id &propertyId, const QString &propertyName) const = 0;
	virtual void restoreRemovedProperty(const Id &propertyId, const QString &previousName) const = 0;
	virtual void restoreRenamedProperty(const Id &propertyId, const QString &previousName) const = 0;

	/// Includes or excludes element from metamodel.
	virtual void setElementEnabled(const Id &type, bool enabled) = 0;
};


}
