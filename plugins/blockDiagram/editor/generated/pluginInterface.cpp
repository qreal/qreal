#include "pluginInterface.h"

#include "elements.h"


Q_EXPORT_PLUGIN2(qreal_editors, BlockDiagramMetamodelPlugin)

BlockDiagramMetamodelPlugin::BlockDiagramMetamodelPlugin()
{
	initPlugin();
}

void BlockDiagramMetamodelPlugin::initPlugin()
{
	initNameMap();
	initMouseGestureMap();
	initPropertyMap();
	initPropertyDefaultsMap();
	initDescriptionMap();
	initParentsMap();
}

void BlockDiagramMetamodelPlugin::initNameMap()
{
	diagramNameMap["BlockDiagram"] = QString::fromUtf8("Block Diagram");
	diagramNodeNameMap["BlockDiagram"] = "BlockDiagramNode";

	elementsNameMap["BlockDiagram"]["Action"] = QString::fromUtf8("Action");
	elementsNameMap["BlockDiagram"]["BlockDiagramNode"] = QString::fromUtf8("Block Diagram");
	elementsNameMap["BlockDiagram"]["BlockFinalNode"] = QString::fromUtf8("Final Node");
	elementsNameMap["BlockDiagram"]["ConditionNode"] = QString::fromUtf8("Condition Node");
	elementsNameMap["BlockDiagram"]["ControlFlow"] = QString::fromUtf8("Control Flow");
	elementsNameMap["BlockDiagram"]["InitialNode"] = QString::fromUtf8("Initial Node");
}

void BlockDiagramMetamodelPlugin::initMouseGestureMap()
{
	elementMouseGesturesMap["BlockDiagram"]["BlockDiagramNode"] = "10, 0 : 90, 0 : 9"
			"0, 100 : 10, 100 : 0, 90 : 0, 10 : 100, 10 : 100, 90 : ";
	elementMouseGesturesMap["BlockDiagram"]["BlockFinalNode"] = "29, 15 : 27, 20 : 2"
			"4, 24 : 20, 27 : 15, 28 : 10, 27 : 6, 24 : 3, 20 : 2, 15 : 3, 10 : 6, 6 : 10, 3 "
			": 15, 2 : 20, 3 : 24, 6 : 27, 10 : 29, 15 : 36, 15 : 34, 23 : 29, 29 : 23, 34 : "
			"15, 35 : 7, 34 : 1, 29 : -4, 23 : -5, 15 : -4, 7 : 1, 1 : 7, -4 : 15, -5 : 23, -"
			"4 : 29, 1 : 34, 7 : 36, 15 : ";
	elementMouseGesturesMap["BlockDiagram"]["ConditionNode"] = "30, 2 : 58, 30 : 30,"
			" 58 : 2, 30 : 30, 2 :";
	elementMouseGesturesMap["BlockDiagram"]["InitialNode"] = "36, 15 : 34, 23 : 29, "
			"29 : 23, 34 : 15, 35 : 7, 34 : 1, 29 : -4, 23 : -5, 15 : -4, 7 : 1, 1 : 7, -4 : "
			"15, -5 : 23, -4 : 29, 1 : 34, 7 : 36, 15 : ";
}

void BlockDiagramMetamodelPlugin::initPropertyMap()
{
	propertyTypes["BlockDiagramNode"]["isView"] = "Bool";
	propertyTypes["ControlFlow"]["type"] = "Bool";
}

void BlockDiagramMetamodelPlugin::initPropertyDefaultsMap()
{
	propertyDefault["ConditionNode"]["decision"] = QString::fromUtf8("1");
}

void BlockDiagramMetamodelPlugin::initDescriptionMap()
{
}

void BlockDiagramMetamodelPlugin::initParentsMap()
{
	parentsMap["BlockDiagram"]["BlockDiagramNode"]
		<< qMakePair(QString("BlockDiagram"), QString("Diagram"))
	;
}

QStringList BlockDiagramMetamodelPlugin::diagrams() const
{
	return diagramNameMap.keys();
}

QStringList BlockDiagramMetamodelPlugin::elements(QString const &diagram) const
{
	return elementsNameMap[diagram].keys();
}

QStringList BlockDiagramMetamodelPlugin::getPropertiesWithDefaultValues(QString const &element) const
{
	return propertyDefault[element].keys();
}

QIcon BlockDiagramMetamodelPlugin::getIcon(SdfIconEngineV2Interface *engine) const
{
	return QIcon(engine);
}

QString BlockDiagramMetamodelPlugin::editorName() const
{
	 return "BlockDiagramMetamodel Editor";
}

QString BlockDiagramMetamodelPlugin::diagramName(QString const &diagram) const
{
	return diagramNameMap[diagram];
}

QString BlockDiagramMetamodelPlugin::diagramNodeName(QString const &diagram) const
{
	return diagramNodeNameMap[diagram];
}

QString BlockDiagramMetamodelPlugin::elementName(QString const &diagram, QString const &element) const
{
	return elementsNameMap[diagram][element];
}

QString BlockDiagramMetamodelPlugin::elementDescription(QString const &diagram, QString const &element) const
{
	return elementsDescriptionMap[diagram][element];
}

QString BlockDiagramMetamodelPlugin::propertyDescription(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDescriptionMap[diagram][element][property];
}

QString BlockDiagramMetamodelPlugin::propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDisplayedNamesMap[diagram][element][property];
}

QString BlockDiagramMetamodelPlugin::elementMouseGesture(QString const &diagram, QString const &element) const
{
	return elementMouseGesturesMap[diagram][element];
}

ElementImpl* BlockDiagramMetamodelPlugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const
{
	if (element == "Action")
		return new Action();
	else if (element == "BlockDiagramNode")
		return new BlockDiagramNode();
	else if (element == "BlockFinalNode")
		return new BlockFinalNode();
	else if (element == "ConditionNode")
		return new ConditionNode();
	else if (element == "ControlFlow")
		return new ControlFlow();
	else if (element == "InitialNode")
		return new InitialNode();
	else {
		Q_ASSERT(!"Request for creation of an element with unknown name");
		return NULL;
	}
}

bool BlockDiagramMetamodelPlugin::isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	if (parentsMap[childDiagram][childElement].contains(qMakePair(parentDiagram, parentElement)))
		return true;
	typedef QPair<QString, QString> StringPair;
	foreach (StringPair const &pair, parentsMap[childDiagram][childElement])
		if (isParentOf(parentDiagram, parentElement, pair.first, pair.second))
			return true;
	return false;
}
QList<QPair<QString, QString> > BlockDiagramMetamodelPlugin::getParentsOf(QString const &diagram, QString const &element) const
{
	return parentsMap[diagram][element];
}
QStringList BlockDiagramMetamodelPlugin::getPropertyNames(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "Action")
		result  << "process";
	else if (element == "BlockDiagramNode")
		result  << "isView" << "language";
	else if (element == "BlockFinalNode")
;
	else if (element == "ConditionNode")
		result  << "condition" << "decision";
	else if (element == "ControlFlow")
		result  << "type" << "type_number";
	else if (element == "InitialNode")
;
	return result;
}

QStringList BlockDiagramMetamodelPlugin::getTypesContainedBy(QString const &element) const
{
	QStringList result;
	if (element == "BlockDiagramNode")
		result << "Action" << "FinalNode" << "ControlFlow" << "InitialNode" << "DecisionNode" ;
	return result;
}

QStringList BlockDiagramMetamodelPlugin::getConnectedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QStringList BlockDiagramMetamodelPlugin::getUsedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > BlockDiagramMetamodelPlugin::getPossibleEdges(QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	if (element == "ControlFlow")
		result << qMakePair(qMakePair(QString("Action"),QString("Action")),qMakePair(false,QString("ControlFlow"))) << qMakePair(qMakePair(QString("Action"),QString("BlockFinalNode")),qMakePair(true,QString("ControlFlow"))) << qMakePair(qMakePair(QString("Action"),QString("ConditionNode")),qMakePair(false,QString("ControlFlow"))) << qMakePair(qMakePair(QString("ConditionNode"),QString("ConditionNode")),qMakePair(false,QString("ControlFlow"))) << qMakePair(qMakePair(QString("ConditionNode"),QString("FinalNode")),qMakePair(true,QString("ControlFlow"))) << qMakePair(qMakePair(QString("InitialNode"),QString("Action")),qMakePair(true,QString("ControlFlow"))) << qMakePair(qMakePair(QString("InitialNode"),QString("BlockFinalNode")),qMakePair(true,QString("ControlFlow"))) << qMakePair(qMakePair(QString("InitialNode"),QString("ConditionNode")),qMakePair(true,QString("ControlFlow")));
	return result;
}

//(-1) means "edge", (+1) means "node"
int BlockDiagramMetamodelPlugin::isNodeOrEdge(QString const &element) const
{
	if (element == "Action")
		return 1;
	else if (element == "BlockDiagramNode")
		return 1;
	else if (element == "BlockFinalNode")
		return 1;
	else if (element == "ConditionNode")
		return 1;
	else if (element == "ControlFlow")
		return -1;
	else if (element == "InitialNode")
		return 1;
	return 0;
}
QStringList BlockDiagramMetamodelPlugin::getEnumValues(QString name) const 
{
	QStringList result;
	if (name == "VisibilityKind")
		result << QString::fromUtf8("private") << QString::fromUtf8("protected") << QString::fromUtf8("public") << QString::fromUtf8("package");
	else if (name == "CallConcurrencyKind")
		result << QString::fromUtf8("sequential") << QString::fromUtf8("guarded") << QString::fromUtf8("concurrent");
	else if (name == "AggregationKind")
		result << QString::fromUtf8("none") << QString::fromUtf8("shared") << QString::fromUtf8("composite");
	else if (name == "Bool")
		result << QString::fromUtf8("false") << QString::fromUtf8("true");
	else if (name == "ParameterDirectionKind")
		result << QString::fromUtf8("in") << QString::fromUtf8("inout") << QString::fromUtf8("out") << QString::fromUtf8("return");
	return result;
}

QString BlockDiagramMetamodelPlugin::getPropertyType(QString const &element, QString const &property) const
{
	return propertyTypes[element][property];
}

QString BlockDiagramMetamodelPlugin::getPropertyDefaultValue(QString const &element, QString const &property) const
{
	return propertyDefault[element][property];
}

QList<qReal::ListenerInterface*> BlockDiagramMetamodelPlugin::listeners() const
{
	QList<qReal::ListenerInterface*> result;
	return result;
}
