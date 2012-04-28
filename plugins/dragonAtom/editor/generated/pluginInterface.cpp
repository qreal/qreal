#include "pluginInterface.h"

#include "elements.h"


Q_EXPORT_PLUGIN2(qreal_editors, DragonAtomMetamodelPlugin)

DragonAtomMetamodelPlugin::DragonAtomMetamodelPlugin()
{
	initPlugin();
}

void DragonAtomMetamodelPlugin::initPlugin()
{
	initNameMap();
	initMouseGestureMap();
	initPropertyMap();
	initPropertyDefaultsMap();
	initDescriptionMap();
	initParentsMap();
	initPaletteGroupsMap();
}

void DragonAtomMetamodelPlugin::initNameMap()
{
	diagramNameMap["DragonAtom"] = QString::fromUtf8("Dragon Atom");
	diagramNodeNameMap["DragonAtom"] = "DragonAtomNode";

	elementsNameMap["DragonAtom"]["Atom1Node"] = QString::fromUtf8("Atom1 Node");
	elementsNameMap["DragonAtom"]["Atom2Node"] = QString::fromUtf8("Atom2 Node");
	elementsNameMap["DragonAtom"]["Atom3Node"] = QString::fromUtf8("Atom3 Node");
	elementsNameMap["DragonAtom"]["Atom4Node"] = QString::fromUtf8("Atom4 Node");
	elementsNameMap["DragonAtom"]["DragonAtomNode"] = QString::fromUtf8("Dragon Atom");
}

void DragonAtomMetamodelPlugin::initPaletteGroupsMap()
{
}

void DragonAtomMetamodelPlugin::initMouseGestureMap()
{
	elementMouseGesturesMap["DragonAtom"]["DragonAtomNode"] = "10, 0 : 90, 0 : 90, 1"
			"00 : 10, 100 : 0, 90 : 0, 10 : 100, 10 : 100, 90 : ";
}

void DragonAtomMetamodelPlugin::initPropertyMap()
{
	propertyTypes["DragonAtomNode"]["isView"] = "Bool";
}

void DragonAtomMetamodelPlugin::initPropertyDefaultsMap()
{
}

void DragonAtomMetamodelPlugin::initDescriptionMap()
{
}

void DragonAtomMetamodelPlugin::initParentsMap()
{
	parentsMap["DragonAtom"]["DragonAtomNode"]
		<< qMakePair(QString("DragonAtom"), QString("Diagram"))
	;
}

QStringList DragonAtomMetamodelPlugin::diagrams() const
{
	return diagramNameMap.keys();
}

QStringList DragonAtomMetamodelPlugin::diagramPaletteGroups(QString const &diagram) const
{
	return paletteGroupsMap[diagram].keys();
}

QStringList DragonAtomMetamodelPlugin::diagramPaletteGroupList(QString const &diagram, QString const &group) const
{
	return paletteGroupsMap[diagram][group];
}

QStringList DragonAtomMetamodelPlugin::elements(QString const &diagram) const
{
	return elementsNameMap[diagram].keys();
}

QStringList DragonAtomMetamodelPlugin::getPropertiesWithDefaultValues(QString const &element) const
{
	return propertyDefault[element].keys();
}

QIcon DragonAtomMetamodelPlugin::getIcon(SdfIconEngineV2Interface *engine) const
{
	return QIcon(engine);
}

QString DragonAtomMetamodelPlugin::editorName() const
{
	 return "DragonAtomMetamodel Editor";
}

QString DragonAtomMetamodelPlugin::diagramName(QString const &diagram) const
{
	return diagramNameMap[diagram];
}

QString DragonAtomMetamodelPlugin::diagramNodeName(QString const &diagram) const
{
	return diagramNodeNameMap[diagram];
}

QString DragonAtomMetamodelPlugin::elementName(QString const &diagram, QString const &element) const
{
	return elementsNameMap[diagram][element];
}

QString DragonAtomMetamodelPlugin::elementDescription(QString const &diagram, QString const &element) const
{
	return elementsDescriptionMap[diagram][element];
}

QString DragonAtomMetamodelPlugin::propertyDescription(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDescriptionMap[diagram][element][property];
}

QString DragonAtomMetamodelPlugin::propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDisplayedNamesMap[diagram][element][property];
}

QString DragonAtomMetamodelPlugin::elementMouseGesture(QString const &diagram, QString const &element) const
{
	return elementMouseGesturesMap[diagram][element];
}

ElementImpl* DragonAtomMetamodelPlugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const
{
	if (element == "Atom1Node")
		return new Atom1Node();
	else if (element == "Atom2Node")
		return new Atom2Node();
	else if (element == "Atom3Node")
		return new Atom3Node();
	else if (element == "Atom4Node")
		return new Atom4Node();
	else if (element == "DragonAtomNode")
		return new DragonAtomNode();
	else {
		Q_ASSERT(!"Request for creation of an element with unknown name");
		return NULL;
	}
}

bool DragonAtomMetamodelPlugin::isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
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
QList<QPair<QString, QString> > DragonAtomMetamodelPlugin::getParentsOf(QString const &diagram, QString const &element) const
{
	return parentsMap[diagram][element];
}
QStringList DragonAtomMetamodelPlugin::getPropertyNames(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "Atom1Node")
		result  << "process";
	else if (element == "Atom2Node")
		result  << "process";
	else if (element == "Atom3Node")
		result  << "process";
	else if (element == "Atom4Node")
		result  << "process";
	else if (element == "DragonAtomNode")
		result  << "isView" << "language";
	return result;
}

QStringList DragonAtomMetamodelPlugin::getTypesContainedBy(QString const &element) const
{
	QStringList result;
	if (element == "DragonAtomNode")
		result << "Action" ;
	return result;
}

QStringList DragonAtomMetamodelPlugin::getConnectedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QStringList DragonAtomMetamodelPlugin::getUsedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > DragonAtomMetamodelPlugin::getPossibleEdges(QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	Q_UNUSED(element);
	return result;
}

//(-1) means "edge", (+1) means "node"
int DragonAtomMetamodelPlugin::isNodeOrEdge(QString const &element) const
{
	if (element == "Atom1Node")
		return 1;
	else if (element == "Atom2Node")
		return 1;
	else if (element == "Atom3Node")
		return 1;
	else if (element == "Atom4Node")
		return 1;
	else if (element == "DragonAtomNode")
		return 1;
	return 0;
}
QStringList DragonAtomMetamodelPlugin::getEnumValues(QString name) const 
{
	QStringList result;
	if (name == "VisibilityKind")
		result << QString::fromUtf8("private") << QString::fromUtf8("protected") << QString::fromUtf8("public") << QString::fromUtf8("package");
	else if (name == "AggregationKind")
		result << QString::fromUtf8("none") << QString::fromUtf8("shared") << QString::fromUtf8("composite");
	else if (name == "Bool")
		result << QString::fromUtf8("false") << QString::fromUtf8("true");
	else if (name == "ParameterDirectionKind")
		result << QString::fromUtf8("in") << QString::fromUtf8("inout") << QString::fromUtf8("out") << QString::fromUtf8("return");
	return result;
}

QString DragonAtomMetamodelPlugin::getPropertyType(QString const &element, QString const &property) const
{
	return propertyTypes[element][property];
}

QString DragonAtomMetamodelPlugin::getPropertyDefaultValue(QString const &element, QString const &property) const
{
	return propertyDefault[element][property];
}

QList<qReal::ListenerInterface*> DragonAtomMetamodelPlugin::listeners() const
{
	QList<qReal::ListenerInterface*> result;
	return result;
}
