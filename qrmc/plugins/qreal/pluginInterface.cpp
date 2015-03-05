#include "pluginInterface.h"
#include "../../../qrgui/editorPluginInterface/elementImpl.h"


#include "elements.h"


//Q_EXPORT_PLUGIN2(qreal_editors, QrealPlugin);

QrealPlugin::QrealPlugin()
{
	initPlugin();
}

void QrealPlugin::initPlugin()
{
	initNameMap();
	initMouseGestureMap();
	initPropertyMap();
	initPropertyDefaultsMap();
}

void QrealPlugin::initNameMap()
{
	diagramNameMap["Editor"] = QString::fromUtf8("3");

	diagramNodeNameMap["Editor"] = "Node";

	elementsNameMap["Editor"]["Edge"] = QString::fromUtf8("2");
	elementsNameMap["Editor"]["Node"] = QString::fromUtf8("1");


}

void QrealPlugin::initMouseGestureMap()
{

}

void QrealPlugin::initPropertyMap()
{

}

void QrealPlugin::initPropertyDefaultsMap()
{

}

void QrealPlugin::initParentsMap()
{

}

QStringList QrealPlugin::diagrams() const
{
	return diagramNameMap.keys();
}

QStringList QrealPlugin::elements(QString const &diagram) const
{
	return elementsNameMap[diagram].keys();
}

QString QrealPlugin::elementDescription(QString const &diagram, QString const &element) const
{
	return elementsDescriptionMap[diagram][element];
}

QString QrealPlugin::propertyDescription(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDescriptionMap[diagram][element][property];
}

QString QrealPlugin::propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDisplayedNamesMap[diagram][element][property];
}

bool QrealPlugin::isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
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

QList<QPair<QString, QString> > QrealPlugin::getParentsOf(QString const &diagram, QString const &element) const
{
	return parentsMap[diagram][element];
}

QStringList QrealPlugin::diagramPaletteGroups(QString const &diagram) const
{
	return paletteGroupsMap[diagram].keys();
}

QStringList QrealPlugin::diagramPaletteGroupList(QString const &diagram, QString const &group) const
{
	return paletteGroupsMap[diagram][group];
} 

QStringList QrealPlugin::getPropertiesWithDefaultValues(QString const &element) const
{
	return propertyDefault[element].keys();
}

QString QrealPlugin::diagramPaletteGroupDescription(QString const &diagram, QString const &group) const
{
	return paletteGroupsDescriptionMap[diagram][group];
}

//QIcon QrealPlugin::getIcon(SdfIconEngineV2Interface *engine) const
//{
//	return QIcon(engine);
//}

QString QrealPlugin::editorName() const
{
	 return "Qreal Editor";
}

QString QrealPlugin::diagramName(QString const &diagram) const
{
	return diagramNameMap[diagram];
}

QString QrealPlugin::diagramNodeName(QString const &diagram) const
{
	return diagramNodeNameMap[diagram];
}

QString QrealPlugin::elementName(QString const &diagram, QString const &element) const
{
	return elementsNameMap[diagram][element];
}

QString QrealPlugin::elementMouseGesture(QString const &diagram, QString const &element) const
{
	return elementMouseGesturesMap[diagram][element];
}

qReal::ElementImpl* QrealPlugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const
{
	if (element == "Edge")
		return new Edge();
	else if (element == "Node")
		return new Node();


	Q_ASSERT(!"Request for creation of an element with unknown name");
	return NULL;
}

QStringList QrealPlugin::getPropertyNames(QString const &/*diagram*/, QString const &element) const
{
	return propertyTypes[element].keys();
}

QStringList QrealPlugin::getReferenceProperties(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	Q_UNUSED(element)

	return result;
}

QStringList QrealPlugin::getTypesContainedBy(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element)

	return result;
}

QStringList QrealPlugin::getConnectedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element)

	return result;
}

QStringList QrealPlugin::getUsedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element)

	return result;
}

QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > QrealPlugin::getPossibleEdges(QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	Q_UNUSED(element)

	return result;
}

//(-1) means "edge", (+1) means "node"
int QrealPlugin::isNodeOrEdge(QString const &element) const
{
	if (element == "Edge")
		return -1;
	else if (element == "Node")
		return 1;

	return 0;
}
//QStringList QrealPlugin::getEnumValues(QString name) const
//{
//	QStringList result;
//
//	return result;
//}

QString QrealPlugin::getPropertyType(QString const &element, QString const &property) const
{
	return propertyTypes[element][property];
}

QString QrealPlugin::getPropertyDefaultValue(QString const &element, QString const &property) const
{
	return propertyDefault[element][property];
}

QList<qReal::ListenerInterface*> QrealPlugin::listeners() const
{
	QList<qReal::ListenerInterface*> result;
	return result;
}

QList<qReal::EditorInterface::ExplosionData>QrealPlugin::explosions(QString const &diagram, QString const &element) const
{
	return mExplosionsMap[diagram][element];
}

QStringList QrealPlugin::getPortTypes(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "Node") {
		result << "NonTyped";
	}
	return result;
}

QList<QPair<QString, QString>> QrealPlugin::getEnumValues(QString const &name) const 
{
	Q_UNUSED(name);
	return {};
}

bool QrealPlugin::isEnumEditable(QString const &name) const
{
	QStringList const editableEnums = {  };
	return editableEnums.contains(name);
}

QString QrealPlugin::getGroupsXML() const 
{
	return QString::fromUtf8("");
}

bool QrealPlugin::shallPaletteBeSorted(QString const &diagram) const
{
	return mShallPaletteBeSortedMap[diagram];
}
