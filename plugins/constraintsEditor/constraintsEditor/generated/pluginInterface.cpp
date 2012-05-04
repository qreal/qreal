#include "pluginInterface.h"

#include "elements.h"


Q_EXPORT_PLUGIN2(qreal_editors, ConstraintsEditorPlugin)

ConstraintsEditorPlugin::ConstraintsEditorPlugin()
{
	initPlugin();
}

void ConstraintsEditorPlugin::initPlugin()
{
	initNameMap();
	initMouseGestureMap();
	initPropertyMap();
	initPropertyDefaultsMap();
	initDescriptionMap();
	initParentsMap();
	initPaletteGroupsMap();
}

void ConstraintsEditorPlugin::initNameMap()
{
	diagramNameMap["ConstraintsEditor"] = QString::fromUtf8("Constraints Editor");
	diagramNodeNameMap["ConstraintsEditor"] = "metamodelConstraints";

	elementsNameMap["ConstraintsEditor"]["BeginNode"] = QString::fromUtf8("Begin Node");
	elementsNameMap["ConstraintsEditor"]["Childrens"] = QString::fromUtf8("Childrens");
	elementsNameMap["ConstraintsEditor"]["ConstraintsDiagram"] = QString::fromUtf8("Constraints Diagram");
	elementsNameMap["ConstraintsEditor"]["EdgeConstraint"] = QString::fromUtf8("Edge Constraint");
	elementsNameMap["ConstraintsEditor"]["EdgesConstraint"] = QString::fromUtf8("Edges Constraint");
	elementsNameMap["ConstraintsEditor"]["EndNode"] = QString::fromUtf8("End Node");
	elementsNameMap["ConstraintsEditor"]["IncomingLinks"] = QString::fromUtf8("Incoming Links");
	elementsNameMap["ConstraintsEditor"]["IncomingNodes"] = QString::fromUtf8("Incoming Nodes");
	elementsNameMap["ConstraintsEditor"]["MetamodelConstraints"] = QString::fromUtf8("Metamodel Constraints");
	elementsNameMap["ConstraintsEditor"]["NodeConstraint"] = QString::fromUtf8("Node Constraint");
	elementsNameMap["ConstraintsEditor"]["NodesConstraint"] = QString::fromUtf8("Nodes Constraint");
	elementsNameMap["ConstraintsEditor"]["Or"] = QString::fromUtf8("Or");
	elementsNameMap["ConstraintsEditor"]["OutgoingLinks"] = QString::fromUtf8("Outgoing Links");
	elementsNameMap["ConstraintsEditor"]["OutgoingNodes"] = QString::fromUtf8("Outgoing Nodes");
	elementsNameMap["ConstraintsEditor"]["Parent"] = QString::fromUtf8("Parent");
	elementsNameMap["ConstraintsEditor"]["PropertyNode"] = QString::fromUtf8("Property");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["BeginNode"]["exists"] = QString::fromUtf8("Exists");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["Childrens"]["count"] = QString::fromUtf8("Count");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["ConstraintsDiagram"]["languageName"] = QString::fromUtf8("Language Name");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["EdgeConstraint"]["errorText"] = QString::fromUtf8("Text of error");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["EdgeConstraint"]["errorType"] = QString::fromUtf8("Error Type");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["EdgesConstraint"]["errorText"] = QString::fromUtf8("Text of error");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["EdgesConstraint"]["errorType"] = QString::fromUtf8("Error Type");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["EdgesConstraint"]["selection"] = QString::fromUtf8("Selection");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["EndNode"]["exists"] = QString::fromUtf8("Exists");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["IncomingLinks"]["count"] = QString::fromUtf8("Count");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["MetamodelConstraints"]["metamodelName"] = QString::fromUtf8("Metamodel Name");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["MetamodelConstraints"]["outputDirPath"] = QString::fromUtf8("Output Dir Path");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["MetamodelConstraints"]["pathToQReal"] = QString::fromUtf8("Dir Path to QReal");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["NodeConstraint"]["errorText"] = QString::fromUtf8("Text of error");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["NodeConstraint"]["errorType"] = QString::fromUtf8("Error Type");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["NodesConstraint"]["errorText"] = QString::fromUtf8("Text of error");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["NodesConstraint"]["errorType"] = QString::fromUtf8("Error Type");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["NodesConstraint"]["selection"] = QString::fromUtf8("Selection");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["OutgoingLinks"]["count"] = QString::fromUtf8("Count");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["OutgoingNodes"]["count"] = QString::fromUtf8("Count");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["Parent"]["exists"] = QString::fromUtf8("Exists");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["PropertyNode"]["property"] = QString::fromUtf8("Property");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["PropertyNode"]["sign"] = QString::fromUtf8("Sign");
	propertiesDisplayedNamesMap["ConstraintsEditor"]["PropertyNode"]["value"] = QString::fromUtf8("Value");
}

void ConstraintsEditorPlugin::initPaletteGroupsMap()
{
}

void ConstraintsEditorPlugin::initMouseGestureMap()
{
}

void ConstraintsEditorPlugin::initPropertyMap()
{
	propertyTypes["BeginNode"]["exists"] = "ExistsType";
	propertyTypes["EdgeConstraint"]["errorType"] = "ErrorTypeType";
	propertyTypes["EdgesConstraint"]["errorType"] = "ErrorTypeType";
	propertyTypes["EndNode"]["exists"] = "ExistsType";
	propertyTypes["NodeConstraint"]["errorType"] = "ErrorTypeType";
	propertyTypes["NodesConstraint"]["errorType"] = "ErrorTypeType";
	propertyTypes["Parent"]["exists"] = "ExistsType";
}

void ConstraintsEditorPlugin::initPropertyDefaultsMap()
{
	propertyDefault["BeginNode"]["exists"] = QString::fromUtf8("true");
	propertyDefault["Childrens"]["count"] = QString::fromUtf8(">= 0");
	propertyDefault["EdgeConstraint"]["errorText"] = QString::fromUtf8("fail_text");
	propertyDefault["EdgeConstraint"]["errorType"] = QString::fromUtf8("warning");
	propertyDefault["EdgesConstraint"]["errorText"] = QString::fromUtf8("fail_text");
	propertyDefault["EdgesConstraint"]["errorType"] = QString::fromUtf8("warning");
	propertyDefault["EdgesConstraint"]["selection"] = QString::fromUtf8("all");
	propertyDefault["EndNode"]["exists"] = QString::fromUtf8("true");
	propertyDefault["IncomingLinks"]["count"] = QString::fromUtf8(">= 0");
	propertyDefault["NodeConstraint"]["errorText"] = QString::fromUtf8("fail_text");
	propertyDefault["NodeConstraint"]["errorType"] = QString::fromUtf8("warning");
	propertyDefault["NodesConstraint"]["errorText"] = QString::fromUtf8("fail_text");
	propertyDefault["NodesConstraint"]["errorType"] = QString::fromUtf8("warning");
	propertyDefault["NodesConstraint"]["selection"] = QString::fromUtf8("all");
	propertyDefault["OutgoingLinks"]["count"] = QString::fromUtf8(">= 0");
	propertyDefault["OutgoingNodes"]["count"] = QString::fromUtf8(">= 0");
	propertyDefault["Parent"]["exists"] = QString::fromUtf8("true");
}

void ConstraintsEditorPlugin::initDescriptionMap()
{
}

void ConstraintsEditorPlugin::initParentsMap()
{
	parentsMap["ConstraintsEditor"]["BeginNode"]
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractExistsNode"))
	;
	parentsMap["ConstraintsEditor"]["Childrens"]
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractNodeForNodeConstraint"))
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractListOfElementsNode"))
	;
	parentsMap["ConstraintsEditor"]["EdgeConstraint"]
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractErrorTypeNode"))
	;
	parentsMap["ConstraintsEditor"]["EdgesConstraint"]
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractSelectionNode"))
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractErrorTypeNode"))
	;
	parentsMap["ConstraintsEditor"]["EndNode"]
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractExistsNode"))
	;
	parentsMap["ConstraintsEditor"]["IncomingLinks"]
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractNodeForNodeConstraint"))
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractListOfElementsNode"))
	;
	parentsMap["ConstraintsEditor"]["IncomingNodes"]
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractNodeForNodeConstraint"))
	;
	parentsMap["ConstraintsEditor"]["NodeConstraint"]
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractErrorTypeNode"))
	;
	parentsMap["ConstraintsEditor"]["NodesConstraint"]
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractSelectionNode"))
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractErrorTypeNode"))
	;
	parentsMap["ConstraintsEditor"]["OutgoingLinks"]
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractNodeForNodeConstraint"))
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractListOfElementsNode"))
	;
	parentsMap["ConstraintsEditor"]["OutgoingNodes"]
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractNodeForNodeConstraint"))
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractListOfElementsNode"))
	;
	parentsMap["ConstraintsEditor"]["Parent"]
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractNodeForNodeConstraint"))
		<< qMakePair(QString("ConstraintsEditor"), QString("AbstractExistsNode"))
	;
}

QStringList ConstraintsEditorPlugin::diagrams() const
{
	return diagramNameMap.keys();
}

QStringList ConstraintsEditorPlugin::diagramPaletteGroups(QString const &diagram) const
{
	return paletteGroupsMap[diagram].keys();
}

QStringList ConstraintsEditorPlugin::diagramPaletteGroupList(QString const &diagram, QString const &group) const
{
	return paletteGroupsMap[diagram][group];
}

QStringList ConstraintsEditorPlugin::elements(QString const &diagram) const
{
	return elementsNameMap[diagram].keys();
}

QStringList ConstraintsEditorPlugin::getPropertiesWithDefaultValues(QString const &element) const
{
	return propertyDefault[element].keys();
}

QIcon ConstraintsEditorPlugin::getIcon(SdfIconEngineV2Interface *engine) const
{
	return QIcon(engine);
}

QString ConstraintsEditorPlugin::editorName() const
{
	 return "ConstraintsEditor Editor";
}

QString ConstraintsEditorPlugin::diagramName(QString const &diagram) const
{
	return diagramNameMap[diagram];
}

QString ConstraintsEditorPlugin::diagramNodeName(QString const &diagram) const
{
	return diagramNodeNameMap[diagram];
}

QString ConstraintsEditorPlugin::elementName(QString const &diagram, QString const &element) const
{
	return elementsNameMap[diagram][element];
}

QString ConstraintsEditorPlugin::elementDescription(QString const &diagram, QString const &element) const
{
	return elementsDescriptionMap[diagram][element];
}

QString ConstraintsEditorPlugin::propertyDescription(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDescriptionMap[diagram][element][property];
}

QString ConstraintsEditorPlugin::propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDisplayedNamesMap[diagram][element][property];
}

QString ConstraintsEditorPlugin::elementMouseGesture(QString const &diagram, QString const &element) const
{
	return elementMouseGesturesMap[diagram][element];
}

ElementImpl* ConstraintsEditorPlugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const
{
	if (element == "BeginNode")
		return new BeginNode();
	else if (element == "Childrens")
		return new Childrens();
	else if (element == "ConstraintsDiagram")
		return new ConstraintsDiagram();
	else if (element == "EdgeConstraint")
		return new EdgeConstraint();
	else if (element == "EdgesConstraint")
		return new EdgesConstraint();
	else if (element == "EndNode")
		return new EndNode();
	else if (element == "IncomingLinks")
		return new IncomingLinks();
	else if (element == "IncomingNodes")
		return new IncomingNodes();
	else if (element == "MetamodelConstraints")
		return new MetamodelConstraints();
	else if (element == "NodeConstraint")
		return new NodeConstraint();
	else if (element == "NodesConstraint")
		return new NodesConstraint();
	else if (element == "Or")
		return new Or();
	else if (element == "OutgoingLinks")
		return new OutgoingLinks();
	else if (element == "OutgoingNodes")
		return new OutgoingNodes();
	else if (element == "Parent")
		return new Parent();
	else if (element == "PropertyNode")
		return new PropertyNode();
	else {
		Q_ASSERT(!"Request for creation of an element with unknown name");
		return NULL;
	}
}

bool ConstraintsEditorPlugin::isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	if (childDiagram == parentDiagram && childElement == parentElement)
		return true;
	if (parentsMap[childDiagram][childElement].contains(qMakePair(parentDiagram, parentElement)))
		return true;
	typedef QPair<QString, QString> StringPair;
	foreach (StringPair const &pair, parentsMap[childDiagram][childElement])
		if (isParentOf(parentDiagram, parentElement, pair.first, pair.second))
			return true;
	return false;
}
QList<QPair<QString, QString> > ConstraintsEditorPlugin::getParentsOf(QString const &diagram, QString const &element) const
{
	return parentsMap[diagram][element];
}
QStringList ConstraintsEditorPlugin::getPropertyNames(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "BeginNode")
		result  << "exists";
	else if (element == "Childrens")
		result  << "count";
	else if (element == "ConstraintsDiagram")
		result  << "languageName";
	else if (element == "EdgeConstraint")
		result  << "errorText" << "errorType";
	else if (element == "EdgesConstraint")
		result  << "errorText" << "errorType" << "selection";
	else if (element == "EndNode")
		result  << "exists";
	else if (element == "IncomingLinks")
		result  << "count";
	else if (element == "IncomingNodes")
;
	else if (element == "MetamodelConstraints")
		result  << "metamodelName" << "outputDirPath" << "pathToQReal";
	else if (element == "NodeConstraint")
		result  << "errorText" << "errorType";
	else if (element == "NodesConstraint")
		result  << "errorText" << "errorType" << "selection";
	else if (element == "Or")
;
	else if (element == "OutgoingLinks")
		result  << "count";
	else if (element == "OutgoingNodes")
		result  << "count";
	else if (element == "Parent")
		result  << "exists";
	else if (element == "PropertyNode")
		result  << "property" << "sign" << "value";
	return result;
}

QStringList ConstraintsEditorPlugin::getTypesContainedBy(QString const &element) const
{
	QStringList result;
	if (element == "BeginNode")
		result << "PropertyNode" << "AbstractNodeForNodeConstraint" ;
	else if (element == "Childrens")
		result << "AbstractNodeForNodeConstraint" << "PropertyNode" ;
	else if (element == "EdgeConstraint")
		result << "PropertyNode" << "AbstractExistsNode" ;
	else if (element == "EdgesConstraint")
		result << "PropertyNode" << "AbstractExistsNode" ;
	else if (element == "EndNode")
		result << "PropertyNode" << "AbstractNodeForNodeConstraint" ;
	else if (element == "IncomingLinks")
		result << "PropertyNode" << "AbstractExistsNode" ;
	else if (element == "IncomingNodes")
		result << "PropertyNode" << "AbstractNodeForNodeConstraint" ;
	else if (element == "MetamodelConstraints")
		result << "ConstraintsDiagram" ;
	else if (element == "NodeConstraint")
		result << "PropertyNode" << "AbstractNodeForNodeConstraint" ;
	else if (element == "NodesConstraint")
		result << "PropertyNode" << "AbstractNodeForNodeConstraint" ;
	else if (element == "OutgoingLinks")
		result << "PropertyNode" << "AbstractExistsNode" ;
	else if (element == "OutgoingNodes")
		result << "PropertyNode" << "AbstractNodeForNodeConstraint" ;
	else if (element == "Parent")
		result << "AbstractNodeForNodeConstraint" << "PropertyNode" ;
	return result;
}

QStringList ConstraintsEditorPlugin::getConnectedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QStringList ConstraintsEditorPlugin::getUsedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > ConstraintsEditorPlugin::getPossibleEdges(QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	Q_UNUSED(element);
	return result;
}

//(-1) means "edge", (+1) means "node"
int ConstraintsEditorPlugin::isNodeOrEdge(QString const &element) const
{
	if (element == "AbstractErrorTypeNode")
		return 1;
	else if (element == "AbstractExistsNode")
		return 1;
	else if (element == "AbstractListOfElementsNode")
		return 1;
	else if (element == "AbstractNodeForNodeConstraint")
		return 1;
	else if (element == "AbstractSelectionNode")
		return 1;
	else if (element == "BeginNode")
		return 1;
	else if (element == "Childrens")
		return 1;
	else if (element == "ConstraintsDiagram")
		return 1;
	else if (element == "EdgeConstraint")
		return 1;
	else if (element == "EdgesConstraint")
		return 1;
	else if (element == "EndNode")
		return 1;
	else if (element == "ErrorTypeType")
		return 0;
	else if (element == "ExistsType")
		return 0;
	else if (element == "IncomingLinks")
		return 1;
	else if (element == "IncomingNodes")
		return 1;
	else if (element == "MetamodelConstraints")
		return 1;
	else if (element == "NodeConstraint")
		return 1;
	else if (element == "NodesConstraint")
		return 1;
	else if (element == "Or")
		return -1;
	else if (element == "OutgoingLinks")
		return 1;
	else if (element == "OutgoingNodes")
		return 1;
	else if (element == "Parent")
		return 1;
	else if (element == "PropertyNode")
		return 1;
	return 0;
}
QStringList ConstraintsEditorPlugin::getEnumValues(QString name) const 
{
	QStringList result;
	if (name == "ExistsType")
		result << QString::fromUtf8("false") << QString::fromUtf8("true") << QString::fromUtf8("doesn't matter");
	else if (name == "ErrorTypeType")
		result << QString::fromUtf8("warning") << QString::fromUtf8("critical") << QString::fromUtf8("verification");
	return result;
}

QString ConstraintsEditorPlugin::getPropertyType(QString const &element, QString const &property) const
{
	return propertyTypes[element][property];
}

QString ConstraintsEditorPlugin::getPropertyDefaultValue(QString const &element, QString const &property) const
{
	return propertyDefault[element][property];
}

QList<qReal::ListenerInterface*> ConstraintsEditorPlugin::listeners() const
{
	QList<qReal::ListenerInterface*> result;
	return result;
}
