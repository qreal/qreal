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
#include <QtCore/QString>

const QString endline = "\n";
const QString nodeIndent = "\t\t\t";

const QString generatedDir = "../qrmc/plugins";
const QString shapesDir = "shapes";
const QString generatedShapesDir = "generated";
const QString pluginHeaderName = "pluginInterface.h";
const QString pluginSourceName = "pluginInterface.cpp";
const QString elementsFileName = "elements.h";
const QString resourceFileName = "plugin.qrc";
const QString pluginsProjectFileName = "plugin.pro";

const QString templatesDir = "templates";
const QString pluginHeaderTemplate = "pluginInterface.h.template";
const QString pluginSourceTemplate = "pluginInterface.cpp.template";
const QString elementsHeaderTemplate = "elements.h.template";
const QString nodeClassTemplate = "node.template";
const QString edgeClassTemplate = "edge.template";
const QString utilsTemplate = "utils.template";
const QString resourceTemplate = "plugin.qrc.template";
const QString projectTemplate = "pro.template";
const QString pluginsProjectTemplate = "plugins.pro.template";

const QString subdirsTag = "@@Subdirs@@";
const QString metamodelNameTag = "@@MetamodelName@@";
const QString initDiagramNameMapLineTag = "@@InitDiagramNameMapLine@@";
const QString initDiagramNodeNameMapLineTag = "@@InitDiagramNodeNameMapLine@@";
const QString initPropertyDisplayedNamesTag = "@@InitPropertyNamesMapLine@@";
const QString dispNameTag = "@@DispName@@";
const QString initElementNameMapLineTag = "@@InitElementNameMapLine@@";
const QString initParentsMapLineTag = "@@InitParentsMapLine@@";
const QString initPropertyTypesMapLineTag = "@@InitPropertyTypesMapLine@@";
const QString initMouseGesturesMapLineTag = "@@InitMouseGesturesMapLine@@";
const QString getGraphicalObjectLineTag = "@@GetGraphicalObjectLine@@";
const QString initPropertyDefaultMapLineTag = "@@InitPropertyDefaultMapLine@@";
const QString getContainersLineTag = "@@GetContainedByLine@@";
const QString getReferencePropertiesLineTag = "@@GetReferencePropertiesLine@@";

const QString getPortTypesLineTag = "@@GetPortTypesLine@@";
const QString portTypesListTag = "@@PortTypesList@@";

const QString getPropertyNameTag = "@@GetPropertyNamesLine@@";
const QString getConnectionsLineTag = "@@GetConnectedTypesLine@@";
const QString getUsagesLineTag = "@@GetUsedTypesLine@@";
const QString getIsNodeOrEdgeLineTag = "@@IsNodeOrEdgeLine@@";
const QString getEnumsLineTag = "@@GetEnumValuesLine@@";
const QString getPossibleEdgesLineTag = "@@GetPossibleEdgesLine@@";
const QString sdfPortsTag = "@@PortsSdf@@";
const QString linePortTag = "@@LinePort@@";
const QString pointPortTag = "@@PointPort@@";
const QString portsTag = "@@Ports@@";
const QString nodeUnusedTag = "@@NodeUnusedArgs@@";
const QString nodeLoadShapeRendererTag = "@@NodeLoadShapeRenderer@@";
const QString nodeLoadPortsRendererTag = "@@NodeLoadPortsRenderer@@";
const QString nodeContentsTag = "@@NodeSetContents@@";
const QString nodePointPortInitTag = "@@NodeInitPointPort@@";
const QString nodeLinePortInitTag = "@@NodeInitLinePort@@";
const QString nodeInitPortsTag = "@@NodeInitPorts@@";
const QString nodeInitTag = "@@NodeInit@@";
const QString edgeInitTag = "@@EdgeInit@@";
const QString updateDataTag = "@@UpdateData@@";
const QString nodeBorderTag = "@@NodeBorder@@";
const QString nodeValidBorderTag = "@@NodeValidBorder@@";
const QString nodeInvalidBorderTag = "@@NodeInvalidBorder@@";
const QString labelDefinitionTag = "@@LabelDefinition@@";
const QString nodeConstructorTag = "@@NodeConstructor@@";
const QString contextMenuItems = "@@ContextMenuItems@@";
const QString itemsList = "@@ItemsList@@";
const QString itemsValidList = "@@ItemsValidList@@";
const QString itemsInvalidList = "@@ItemsInvalidList@@";

const QString diagramNameTag = "@@DiagramName@@";
const QString diagramNodeNameTag = "@@DiagramNodeName@@";
const QString diagramDisplayedNameTag = "@@DiagramDisplayedName@@";
const QString elementNameTag = "@@ElementName@@";
const QString elementDisplayedNameTag = "@@ElementDisplayedName@@";
const QString propertyDisplayedNameTag = "@@PropertyDisplayedName@@";
const QString gesturePathTag = "@@GesturePath@@";
const QString propertyNameTag = "@@PropertyName@@";
const QString propertyTypeTag = "@@PropertyType@@";
const QString childNameTag = "@@ChildElementName@@";
const QString parentNameTag = "@@ParentElementName@@";
const QString propertyDefaultTag = "@@PropertyDefaultValue@@";
const QString containersListTag = "@@ContainedByList@@";
const QString connectionsListTag = "@@ConnectedToList@@";
const QString referencePropertiesListTag = "@@ReferencePropertiesList@@";
const QString elementDescriptionMapTag = "@@ElementDescriptionMapLine@@";
const QString descriptionTag = "@@Description@@";
const QString propertyNameListTag = "@@PropertiesList@@";
const QString usagesListTag = "@@UsedTypesList@@";
const QString isNodeOrEdgeTag = "@@IsNodeOrEdgeValue@@";
const QString enumsListTag = "@@EnumValuesList@@";
const QString possibleEdgesListTag = "@@PossibleEdgesList@@";
const QString startXTag = "@@StartX@@";
const QString startYTag = "@@StartY@@";
const QString endXTag = "@@EndX@@";
const QString endYTag = "@@EndY@@";
const QString pointXTag = "@@PointX@@";
const QString pointYTag = "@@PointY@@";
const QString nodeWidthTag = "@@NodeWidth@@";
const QString nodeHeightTag = "@@NodeHeight@@";
const QString startXScalabilityTag = "@@NodeLinePortStartXScalable@@";
const QString startYScalabilityTag = "@@NodeLinePortStartYScalable@@";
const QString endXScalabilityTag = "@@NodeLinePortEndXScalable@@";
const QString endYScalabilityTag = "@@NodeLinePortEndYScalable@@";
const QString labelRotationTag = "@@Rotation@@";
const QString portsForFromPortTypesTag = "@@PortsForFromPortTypes@@";
const QString labelXTag = "@@LabelX@@";
const QString labelYTag = "@@LabelY@@";
const QString labelNameTag = "@@LabelName@@";
const QString labelReadonlyTag = "@@LabelReadonly@@";
const QString labelIndexTag = "@@Count@@";
const QString nameRoleTag ="@@NameRole@@";
const QString customRoleTag ="@@CustomRole@@";
const QString updateRoleTag ="@@UpdateRole@@";
const QString isNodeTag = "@@IsNode@@";
const QString isContainerTag = "@@IsContainer@@";
const QString sortingContainerTag = "@@IsSortContainer@@";
const QString minimizeToChildrenTag = "@@MinimizesToChildren@@";
const QString maximizeChildrenTag = "@@MaximizesChildren@@";
const QString hasMovableChildrenTag = "@@HasMovableChildren@@";
const QString forestallingSizeTag = "@@SizeOfForestalling@@";
const QString childrenForestallingSizeTag = "@@SizeOfChildrenForestalling@@";
const QString lineSdfTag = "@@LineSdf@@";
const QString lineTypeTag = "@@LineType@@";
const QString isResizeable = "@@IsResizeable@@";
const QString xCoordIsScalable = "@@XCoordIsScalable@@";
const QString yCoordIsScalable = "@@YCoordIsScalable@@";

const QString nodesListTag = "@@NodesList@@";
const QString edgesListTag = "@@EdgesList@@";
const QString sdfFileTag = "@@SdfFile@@";
const QString fileNameTag = "@@Filename@@";

const QString arrowTemplateTag = "@@Arrow@@";
const QString openArrowTemplateTag = "@@OpenArrow@@";
const QString rhombTemplateTag = "@@Rhomb@@";
const QString complexArrowTemplateTag = "@@ComplexArrow@@";
const QString filledArrowColorTag = "@@FilledArrowColor@@";
const QString emptyArrowColorTag = "@@EmptyArrowColor@@";
const QString crossedLineTemplateTag = "@@CrossedLine@@";
const QString emptyCircleTemplateTag = "@@EmptyCircle@@";
const QString signalTemplateTag = "@@Signal@@";
const QString timerTemplateTag = "@@Timer@@";
const QString beginArrowBrushColorTag = "@@BeginArrowBrushColor@@";
const QString endArrowBrushColorTag = "@@EndArrowBrushColor@@";
const QString beginArrowCustomizationTag = "@@BeginArrowCustomization@@";
const QString endArrowCustomizationTag = "@@EndArrowCustomization@@";

const QString metamodelDiagram = "MetamodelDiagram";
const QString metaEditorDiagramNode = "MetaEditorDiagramNode";
const QString metaEntityNode = "MetaEntityNode";
const QString metaEntityEdge ="MetaEntityEdge";
const QString metaEntityImport = "MetaEntityImport";
const QString metaEntityEnum = "MetaEntityEnum";
const QString metaEntityValue = "MetaEntityValue";
const QString metaEntityPossibleEdge = "MetaEntityPossibleEdge";
const QString metaEntityPropertiesAsContainer = "MetaEntityPropertiesAsContainer";
const QString metaEntityParent = "MetaEntityParent";
const QString metaEntityAttribute = "MetaEntity_Attribute";
const QString metaEntityConnection = "MetaEntityConnection";
const QString metaEntityUsage = "MetaEntityUsage";
const QString metaEntityContextMenuField = "MetaEntityContextMenuField";
const QString metaEntityAssociation = "MetaEntityAssociation";

const QString nameOfTheDirectory = "name of the directory";

const QString qMakeLineTag = "@@qMakeLine@@";


