#include "pluginInterface.h"

#include "elements.h"


Q_EXPORT_PLUGIN2(qreal_editors, Math_metamodelPlugin)

Math_metamodelPlugin::Math_metamodelPlugin()
{
	initPlugin();
}

void Math_metamodelPlugin::initPlugin()
{
	initNameMap();
	initMouseGestureMap();
	initPropertyMap();
	initPropertyDefaultsMap();
}

void Math_metamodelPlugin::initNameMap()
{
	diagramNameMap["MathDiagram"] = "Math Diagram ";
	diagramNodeNameMap["MathDiagram"] = "MathDiagramNode1";

	elementsNameMap["MathDiagram"]["Connection"] = "Connection";
	elementsNameMap["MathDiagram"]["ConstantValueNode"] = "ConstantValueNode";
	elementsNameMap["MathDiagram"]["FunctionNode"] = "FunctionNode";
	elementsNameMap["MathDiagram"]["IntegralNode"] = "IntegralNode";
	elementsNameMap["MathDiagram"]["MathDiagramNode1"] = "Math Diagram";
}

void Math_metamodelPlugin::initMouseGestureMap()
{
	elementMouseGesturesMap["MathDiagram"]["MathDiagramNode1"] = "10, 0 : 90, 0 : 90"
			", 100 : 10, 100 : 0, 90 : 0, 10 : 100, 10 : 100, 90 : ";
}

void Math_metamodelPlugin::initPropertyMap()
{
	propertyTypes["Connection"]["ownedComment"] = "Comment";
	propertyTypes["ConstantValueNode"]["ownedComment"] = "Comment";
	propertyTypes["FunctionNode"]["ownedComment"] = "Comment";
	propertyTypes["IntegralNode"]["ownedComment"] = "Comment";
	propertyTypes["MathDiagramNode1"]["isView"] = "Bool";
}

void Math_metamodelPlugin::initPropertyDefaultsMap()
{
}

QStringList Math_metamodelPlugin::diagrams() const
{
	return diagramNameMap.keys();
}

QStringList Math_metamodelPlugin::elements(QString const &diagram) const
{
	return elementsNameMap[diagram].keys();
}

QStringList Math_metamodelPlugin::getPropertiesWithDefaultValues(QString const &element) const
{
	return propertyDefault[element].keys();
}

QIcon Math_metamodelPlugin::getIcon(SdfIconEngineV2Interface *engine) const
{
	return QIcon(engine);
}

QString Math_metamodelPlugin::editorName() const
{
	 return "Math_metamodel Editor";
}

QString Math_metamodelPlugin::diagramName(QString const &diagram) const
{
	return diagramNameMap[diagram];
}

QString Math_metamodelPlugin::diagramNodeName(QString const &diagram) const
{
	return diagramNodeNameMap[diagram];
}

QString Math_metamodelPlugin::elementName(QString const &diagram, QString const &element) const
{
	return elementsNameMap[diagram][element];
}

QString Math_metamodelPlugin::elementMouseGesture(QString const &diagram, QString const &element) const
{
	return elementMouseGesturesMap[diagram][element];
}

UML::ElementImpl* Math_metamodelPlugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const
{
	if (element == "Connection")
		return new UML::Connection();
	else if (element == "ConstantValueNode")
		return new UML::ConstantValueNode();
	else if (element == "FunctionNode")
		return new UML::FunctionNode();
	else if (element == "IntegralNode")
		return new UML::IntegralNode();
	else if (element == "MathDiagramNode1")
		return new UML::MathDiagramNode1();
	else {
		Q_ASSERT(!"Request for creation of an element with unknown name");
		return NULL;
	}
}

QStringList Math_metamodelPlugin::getPropertyNames(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "Connection")
		result  << "ownedComment";
	else if (element == "ConstantValueNode")
		result  << "ownedComment" << "value";
	else if (element == "FunctionNode")
		result  << "function" << "ownedComment" << "variables";
	else if (element == "IntegralNode")
		result  << "integral function" << "integral variable" << "low limit" << "ownedComment" << "up limit";
	else if (element == "MathDiagramNode1")
		result  << "isView" << "language";
	return result;
}

QStringList Math_metamodelPlugin::getTypesContainedBy(QString const &element) const
{
	QStringList result;
	if (element == "MathDiagramNode1")
		result << "IntegralNode" << "ConstantValueNode" ;
	return result;
}

QStringList Math_metamodelPlugin::getConnectedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QStringList Math_metamodelPlugin::getUsedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > Math_metamodelPlugin::getPossibleEdges(QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	Q_UNUSED(element);
	return result;
}

QString Math_metamodelPlugin::getPortNames(qReal::Id const &id, double portNumber) const
{
	QString result;
	return result;
}

//(-1) means "edge", (+1) means "node"
int Math_metamodelPlugin::isNodeOrEdge(QString const &element) const
{
	if (element == "Connection")
		return -1;
	else if (element == "ConstantValueNode")
		return 1;
	else if (element == "FunctionNode")
		return 1;
	else if (element == "IntegralNode")
		return 1;
	else if (element == "MathDiagramNode1")
		return 1;
	return 0;
}
QStringList Math_metamodelPlugin::getEnumValues(QString name) const 
{
	QStringList result;
	if (name == "ParameterDirectionKind")
		result << "in" << "inout" << "out" << "return";
	else if (name == "AggregationKind")
		result << "none" << "shared" << "composite";
	else if (name == "VisibilityKind")
		result << "private" << "protected" << "public" << "package";
	else if (name == "Bool")
		result << "false" << "true";
	return result;
}

QString Math_metamodelPlugin::getPropertyType(QString const &element, QString const &property) const
{
	return propertyTypes[element][property];
}

QString Math_metamodelPlugin::getPropertyDefaultValue(QString const &element, QString const &property) const
{
	return propertyDefault[element][property];
}

QList<qReal::ListenerInterface*> Math_metamodelPlugin::listeners() const
{
	QList<qReal::ListenerInterface*> result;
	return result;
}
